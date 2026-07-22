# Bill of materials

Minimal — this is a firmware/BLE bridge, no custom electronics.

| Qty | Item | Notes |
|----:|------|-------|
| 1 | BLE-capable ESP32 devkit | WROOM `esp32dev` works if it holds the link; a **BLE-5** board (C3/C6/S3) is the reliable choice — see `troubleshooting.md` |
| 1 | USB power supply + data cable | Any 5 V USB source near the unit; a *data* cable (not charge-only) for the first flash |
| — | Enclosure | Optional; the board just needs to sit within ~10 m of the PowerPack |

No wiring, sensors, or level shifters. The ESP32 talks to the PowerPack entirely over Bluetooth.
