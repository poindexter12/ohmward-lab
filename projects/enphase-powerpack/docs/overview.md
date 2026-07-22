# Overview

## What this is
A read-only telemetry bridge: an ESP32 connects to the PowerPack over Bluetooth LE, subscribes
to its telemetry stream, and republishes the values to Home Assistant via ESPHome's native API.
Fully local — no Enphase cloud, no phone app in the loop.

## How it works
1. The unit advertises over BLE as `PES_<serial>`.
2. It exposes a **custom notify characteristic** that pushes its complete state as a JSON
   document (~109 fields) roughly 1–2×/second, fragmented across BLE packets.
3. The ESP32 (ESPHome `ble_client`) connects **unbonded**, subscribes, reassembles the JSON
   fragments in a lambda, parses them, and publishes template sensors (SOC, battery power,
   temps, AC output, backup time, cycles, …).
4. A standard **Battery Service** (`0x2A19`) also exposes SOC as a plain 0–100% byte — a simple
   fallback and a "BLE is connected" signal.

See `ble-protocol.md` for the protocol and `telemetry-fields.md` for the field map.

## Why BLE (not WiFi)
The PowerPack joins WiFi but exposes **no local service** — every TCP port is closed; it only
makes outbound connections to Enphase's cloud (and has its own cellular modem). So there is no
local HTTP/REST/MQTT surface. BLE is the only local interface, and happily it's plaintext.

## Hardware
No custom hardware or wiring. A BLE-capable ESP32 devkit on USB power, within Bluetooth range
(~10 m) of the unit. A classic WROOM (BT 4.2) may or may not hold the link; a BLE-5 chip
(C3/C6/S3) is the reliable choice (see `troubleshooting.md`).

## Scope
Monitoring is complete. **Control** (changing settings, toggling outputs) would require the
unit's encrypted provisioning channel (Espressif protocomm, Security1) and is a later phase.
