# 4-Channel I2C Level Shifter

## Overview

Bidirectional level shifter for converting between 3.3V and 5V logic levels.

## Common Module: KeeYees 4-Channel

- LV side: 3.3V reference
- HV side: 5V reference
- Bidirectional on all 4 channels
- Works for I2C, SPI, or general GPIO

## Wiring

| Pin | Connect To |
|-----|------------|
| LV | 3.3V from MCU |
| HV | 5V supply |
| GND | Common ground |
| LV1-LV4 | 3.3V logic side |
| HV1-HV4 | 5V logic side |

## Notes

- Always connect GND first
- LV and HV must be powered for shifting to work
- For I2C, use channels in pairs (SDA + SCL)
