# Trashcan Fullness Sensor – Overview

This project uses an ESP32 and dual HC-SR04 ultrasonic sensors mounted inside
the lid of a trashcan to estimate how full the can is.

## Goals

- Detect approximate trashcan fullness as a percentage.
- Expose the reading to Home Assistant via ESPHome.
- Optionally trigger notifications or automations when the can is "full".

## Hardware Summary

- Microcontroller:
  - ELEGOO ESP-32 Development Board (USB-C, CP2102)
- Sensors:
  - 2x HC-SR04 ultrasonic distance sensors
- Level shifting:
  - KeeYees 4-channel I2C logic level converter (3.3V <-> 5V)
- Power:
  - Arkare 5V 2A 10W DC power adapter (USB Type C) feeding the ESP32 5V/VIN pin
