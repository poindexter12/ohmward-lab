# ESP32 Overview

## Key Specs

- Dual-core 240 MHz Xtensa LX6
- 520 KB SRAM
- WiFi 802.11 b/g/n
- Bluetooth 4.2 + BLE
- 34 GPIO pins (varies by board)
- 12-bit ADC (18 channels)
- 8-bit DAC (2 channels)
- Touch sensors (10 channels)

## Common Development Boards

### ELEGOO ESP-32 (USB-C, CP2102)

- USB-C connector
- CP2102 USB-to-serial
- 30-pin layout
- Good for prototyping

### ESP32-DevKitC

- Official Espressif board
- Micro-USB
- 38-pin layout

### ESP32-WROOM-32

- Module only (needs carrier board)
- FCC/CE certified

## GPIO Notes

- GPIO6-11: Connected to flash, do not use
- GPIO34-39: Input only, no pull-up/down
- GPIO0: Boot mode selection (avoid for general use)
- GPIO2: Often connected to onboard LED
