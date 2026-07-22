# Enphase IQ PowerPack 1500 → Home Assistant

Local, cloud-free monitoring for the **Enphase IQ PowerPack 1500** portable power station,
via a small ESP32 that bridges the unit's Bluetooth telemetry into Home Assistant over ESPHome.

The PowerPack has no local WiFi API (it only talks outbound to Enphase's cloud/cellular), but it
**streams its full state as plaintext JSON over a BLE notify characteristic** — no pairing, no
encryption, no auth to read. This project subscribes to that stream and exposes ~18 HA sensors.

Built using:

- Any BLE-capable ESP32 devkit (WROOM `esp32dev`, or a BLE-5 C3/C6/S3 — see troubleshooting)
- USB power (no custom wiring — the ESP32 just needs to be within ~10 m of the unit)

## Contents

- `docs/` – overview, the reverse-engineered BLE protocol, telemetry field map, troubleshooting, BOM
- `firmware/` – ESPHome configuration (`enphase-powerpack.yaml`)
- `notes/` – experiments and logs

## Status

- [x] BLE protocol reverse-engineered (plaintext JSON telemetry)
- [x] ESPHome firmware (SOC + full telemetry)
- [ ] First-boot confirmation of `powerpack_ble_mac` + `json_service_uuid` on hardware
- [ ] Long-term reliability (BLE hold, reconnect behavior)
- [ ] Control (settings/outputs) via the encrypted provisioning channel — future
