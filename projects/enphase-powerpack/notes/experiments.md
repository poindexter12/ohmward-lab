# Experiments / log

## Reverse-engineering (summary)
- WiFi recon: the unit exposes **no** local service (all TCP ports closed) — it's an outbound
  cloud/cellular client. WiFi path abandoned.
- BLE recon: a raw GATT crawler on a WROOM identified the unit (`PES_<serial>`, custom service
  `021a9004-…`) but couldn't hold a connection — turned out to be the ~30 s provisioning-session
  timeout for clients that don't speak the handshake (not a hardware fault).
- Captured the Enphase app provisioning the unit via **Android HCI snoop** → identified the BLE
  stack as **ESP-IDF Unified Provisioning** (Security1, `no_pop`).
- Direct GATT probing (bleak, from a BLE-5 host) found the payoff: a **plaintext JSON telemetry
  stream** on `6d1b33f8-0010` (notify) and SOC on the standard Battery Service — no auth needed.

## Gotchas discovered
- **Bonding poisons reconnects.** After pairing, a device power-cycle invalidates the bond and
  the central then refuses to reconnect (encryption timeout). Connect unbonded.
- **One BLE central at a time** — the phone app and the bridge contend for the single slot.
- **WROOM (BT 4.2) vs BLE 5** — a Mac/phone connected where the WROOM's raw stack didn't; ESPHome
  may fare better, but a BLE-5 chip is the safe bet.

## TODO on hardware
- [ ] Confirm `powerpack_ble_mac` and `json_service_uuid` from the first-boot GATT log.
- [ ] Verify a WROOM holds the link (else move to C3/C6/S3).
- [ ] Watch reconnect behavior over days.
