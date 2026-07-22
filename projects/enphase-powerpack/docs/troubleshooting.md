# Troubleshooting

## BLE won't connect at all (or `0x3E` establishment errors)
A classic ESP32-**WROOM** is Bluetooth 4.2 and may fail to establish the link where BLE-5
centrals succeed. Options, in order:
1. Switch the firmware to `framework: type: esp-idf` (often more reliable for BLE than arduino).
2. Use a **BLE-5 board** — C3 / C6 / S3. The config is otherwise identical (change `board:` and,
   for C6, use esp-idf). A phone or a Mac (both BLE 5) connect fine, so this is the sure fix.

## Connects, then drops after ~30 seconds
Expected if the client hasn't done anything — the unit's provisioning stack drops idle/unknown
clients. The firmware subscribes immediately, so a healthy session keeps notifying. If it still
drops, another central (a phone) is likely stealing the single BLE slot — close the Enphase app.

## SOC shows up but the detailed sensors stay blank
The `SOC (battery service)` sensor uses standard UUIDs and works as soon as BLE connects. If the
telemetry sensors stay empty, the `json_service_uuid` substitution is wrong. Set `logger: DEBUG`,
watch the GATT tree printed on connect, and set `json_service_uuid` to whichever
`6d1b33f8-000X-…` service contains the `…-0010` characteristic (best guess `…-0000-…`).

## Do not pair / bond
Bonding is unnecessary (telemetry is plaintext) and fragile: a device power-cycle invalidates
the bond and then the central refuses to reconnect (encryption timeout). Keep it unbonded.

## Wrong / rotating MAC
`powerpack_ble_mac` must match the address advertised by `PES_<serial>` (visible in the debug
log). If the address ever rotates, we'd match by name via a small custom component instead.

## Values look wrong / units off
Cross-check field names and units against `telemetry-fields.md` — the JSON keys are terse
(`uiBatP`, `storage_W`, `bat_avg_temp`, `ac_out_f`, …).
