# ESP32 Power Notes

## Power Requirements

- Operating voltage: 3.0V - 3.6V (3.3V typical)
- VIN/5V pin: 5V input to onboard regulator
- Current draw:
  - Active WiFi TX: ~240mA peak
  - Active WiFi RX: ~100mA
  - Light sleep: ~0.8mA
  - Deep sleep: ~10µA

## Power Input Options

### USB

- 5V from USB port
- Limited to ~500mA from most ports
- Good for development

### VIN/5V Pin

- External 5V supply
- Goes through onboard 3.3V regulator
- Can handle higher current loads

### 3.3V Pin

- Direct 3.3V input (bypasses regulator)
- Must be well-regulated
- Risk of damage if over 3.6V

## Recommended Power Supplies

- 5V 2A USB adapter for development
- 5V 1A minimum for WiFi-active projects
- Consider 5V 2A+ if powering sensors from same supply

## Brown-out Detection

- ESP32 has built-in brown-out detector
- Triggers reset if voltage drops below ~2.8V
- Can cause reset loops with weak power supplies
