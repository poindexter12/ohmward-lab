#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/esp32_ble/ble_uuid.h"

#include <string>
#include <vector>
#include <utility>

#ifdef USE_ESP32

namespace esphome {
namespace powerpack_ble {

// Reads the Enphase IQ PowerPack 1500's plaintext-JSON telemetry over BLE by subscribing
// DIRECTLY to the one known notify characteristic — instead of the stock ble_client's full
// GATT enumeration, which the device's huge (100+ char) database and short connection tolerance
// defeat. Data starts within ~1s, which keeps the link alive.
class PowerPackBLE : public Component, public ble_client::BLEClientNode {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::AFTER_BLUETOOTH; }

  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) override;

  void set_service_uuid(const std::string &s) { this->service_uuid_str_ = s; }
  void set_characteristic_uuid(const std::string &s) { this->char_uuid_str_ = s; }
  void add_field(const std::string &json_key, sensor::Sensor *s) {
    this->fields_.emplace_back(json_key, s);
  }

 protected:
  void subscribe_();
  void process_buffer_();
  void publish_frame_(const std::string &json);

  std::string service_uuid_str_;
  std::string char_uuid_str_;
  uint16_t char_handle_{0};
  std::string buf_;
  std::vector<std::pair<std::string, sensor::Sensor *>> fields_;
};

}  // namespace powerpack_ble
}  // namespace esphome

#endif  // USE_ESP32
