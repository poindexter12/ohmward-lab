# Troubleshooting

## Common Issues

### Sensor readings are zero or NAN

- Check that VCC and GND are correctly wired for both HC-SR04 sensors.
- Verify that the ECHO lines are going through the level shifter, not
  directly to the ESP32.
- Confirm shared TRIG on GPIO5 matches the ESPHome configuration.

### Readings are very noisy or jump around

- Use ESPHome filters such as `median` or `moving_average` to smooth values.
- Reduce `update_interval` if necessary (e.g., 500 ms to 1–2 s).
- Make sure the sensors are not seeing the lid or side walls of the can
  due to mounting angle.

### ESP32 keeps resetting

- Check that the wall adapter is a stable 5V supply.
- Make sure you are not shorting 5V to GND or drawing too much current from
  additional hardware.
