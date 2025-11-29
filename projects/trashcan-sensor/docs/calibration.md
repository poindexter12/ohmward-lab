# Calibration Notes

## Trashcan Height

- Measure internal height from sensor face to the bottom of the can.
- Record the "empty" distance in meters (for both sensors if needed).
- Decide what distance counts as "full" (for example, 10 cm from the lid).

Example:

- Empty distance: 0.70 m
- Full distance: 0.10 m

These values are used in the ESPHome template sensors to convert raw distance
to percentage fullness.

## Suggested Process

1. Deploy firmware and verify both ultrasonic sensors are reading stable
   distances with an empty can.
2. Put bags in and measure, record, and adjust the EMPTY and FULL constants
   in the ESPHome template.
3. Test several fill levels (25, 50, 75, 100 percent) and see how the
   reported values compare to reality.
