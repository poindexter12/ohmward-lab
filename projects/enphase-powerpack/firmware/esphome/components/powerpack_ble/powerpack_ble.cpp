#include "powerpack_ble.h"

#ifdef USE_ESP32

#include "esphome/core/log.h"
#include <cmath>
#include <cstdlib>

namespace esphome {
namespace powerpack_ble {

static const char *const TAG = "powerpack_ble";

void PowerPackBLE::setup() {}

void PowerPackBLE::dump_config() {
  ESP_LOGCONFIG(TAG, "Enphase PowerPack BLE:");
  ESP_LOGCONFIG(TAG, "  service:        %s", this->service_uuid_str_.c_str());
  ESP_LOGCONFIG(TAG, "  characteristic: %s", this->char_uuid_str_.c_str());
  ESP_LOGCONFIG(TAG, "  fields:         %u", (unsigned) this->fields_.size());
}

// Find "key": in the JSON text and parse the number after it. Leading quote prevents
// suffix collisions (e.g. "ac_i" won't match inside "pcu_ac_i"). NAN if absent.
static float extract_num(const std::string &o, const std::string &key) {
  std::string pat = "\"" + key + "\":";
  size_t p = o.find(pat);
  if (p == std::string::npos)
    return NAN;
  p += pat.size();
  while (p < o.size() && o[p] == ' ')
    p++;
  return strtof(o.c_str() + p, nullptr);
}

void PowerPackBLE::subscribe_() {
  auto svc = esp32_ble::ESPBTUUID::from_raw(this->service_uuid_str_);
  auto chr = esp32_ble::ESPBTUUID::from_raw(this->char_uuid_str_);
  auto *characteristic = this->parent()->get_characteristic(svc, chr);
  if (characteristic == nullptr) {
    ESP_LOGW(TAG, "telemetry characteristic %s not found in service %s",
             this->char_uuid_str_.c_str(), this->service_uuid_str_.c_str());
    return;
  }
  this->char_handle_ = characteristic->handle;
  ESP_LOGI(TAG, "found telemetry characteristic, handle 0x%04x — registering for notify",
           this->char_handle_);
  auto status = esp_ble_gattc_register_for_notify(this->parent()->get_gattc_if(),
                                                  this->parent()->get_remote_bda(),
                                                  this->char_handle_);
  if (status != ESP_OK)
    ESP_LOGW(TAG, "register_for_notify failed, status=%d", status);
}

void PowerPackBLE::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                       esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      // ESPHome's ble_client finished its (fast, cached) discovery; grab our one char.
      this->subscribe_();
      break;
    }
    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      ESP_LOGD(TAG, "REG_FOR_NOTIFY status=%d handle=0x%04x", param->reg_for_notify.status,
               param->reg_for_notify.handle);
      // Enable notifications by writing 0x0001 to the char's CCCD (0x2902).
      auto *descr = this->parent()->get_config_descriptor(this->char_handle_);
      if (descr == nullptr) {
        ESP_LOGW(TAG, "no CCCD for handle 0x%04x", this->char_handle_);
        break;
      }
      ESP_LOGI(TAG, "CCCD descriptor: handle=0x%04x uuid=%s", descr->handle,
               descr->uuid.to_string().c_str());
      uint16_t enable = 0x0001;
      auto status = esp_ble_gattc_write_char_descr(
          this->parent()->get_gattc_if(), this->parent()->get_conn_id(), descr->handle,
          sizeof(enable), (uint8_t *) &enable, ESP_GATT_WRITE_TYPE_RSP, ESP_GATT_AUTH_REQ_NONE);
      if (status != ESP_OK)
        ESP_LOGW(TAG, "CCCD write failed to queue, status=%d", status);
      else
        ESP_LOGI(TAG, "CCCD write queued — awaiting WRITE_DESCR_EVT for device status");
      break;
    }
    case ESP_GATTC_WRITE_DESCR_EVT: {
      // The device's ACTUAL response to the CCCD write. queue-OK above means nothing
      // if this reports e.g. insufficient authentication (0x05) or encryption (0x0f).
      ESP_LOGI(TAG, "WRITE_DESCR handle=0x%04x status=0x%02x", param->write.handle,
               param->write.status);
      if (param->write.status == ESP_GATT_OK && this->char_handle_ != 0) {
        // Some firmwares only start the notify stream after a read of the char.
        auto status = esp_ble_gattc_read_char(gattc_if, this->parent()->get_conn_id(),
                                              this->char_handle_, ESP_GATT_AUTH_REQ_NONE);
        ESP_LOGD(TAG, "post-subscribe kick read queued, status=%d", status);
      }
      break;
    }
    case ESP_GATTC_READ_CHAR_EVT: {
      ESP_LOGI(TAG, "READ_CHAR handle=0x%04x status=0x%02x len=%d", param->read.handle,
               param->read.status, param->read.value_len);
      if (param->read.status == ESP_GATT_OK && param->read.handle == this->char_handle_ &&
          param->read.value_len > 0) {
        this->buf_.append((const char *) param->read.value, param->read.value_len);
        this->process_buffer_();
      }
      break;
    }
    case ESP_GATTC_NOTIFY_EVT: {
      ESP_LOGD(TAG, "notify handle=0x%04x len=%d (want 0x%04x)", param->notify.handle,
               param->notify.value_len, this->char_handle_);
      if (param->notify.handle != this->char_handle_)
        break;
      this->buf_.append((const char *) param->notify.value, param->notify.value_len);
      if (this->buf_.size() > 16384)
        this->buf_.clear();
      this->process_buffer_();
      break;
    }
    case ESP_GATTC_DISCONNECT_EVT:
    case ESP_GATTC_CLOSE_EVT: {
      this->buf_.clear();
      this->char_handle_ = 0;
      break;
    }
    default:
      break;
  }
}

void PowerPackBLE::process_buffer_() {
  // Extract every complete top-level {...} object from the growing buffer.
  while (true) {
    int depth = 0, start = -1, endp = -1;
    for (size_t i = 0; i < this->buf_.size(); i++) {
      char c = this->buf_[i];
      if (c == '{') {
        if (depth == 0)
          start = (int) i;
        depth++;
      } else if (c == '}') {
        depth--;
        if (depth == 0) {
          endp = (int) i;
          break;
        }
      }
    }
    if (endp < 0 || start < 0)
      break;
    std::string obj = this->buf_.substr(start, endp - start + 1);
    this->buf_.erase(0, endp + 1);
    this->publish_frame_(obj);
  }
}

void PowerPackBLE::publish_frame_(const std::string &json) {
  ESP_LOGD(TAG, "telemetry frame (%u bytes): %.60s...", (unsigned) json.size(), json.c_str());
  for (auto &field : this->fields_) {
    float v = extract_num(json, field.first);
    if (!std::isnan(v))
      field.second->publish_state(v);
  }
}

}  // namespace powerpack_ble
}  // namespace esphome

#endif  // USE_ESP32
