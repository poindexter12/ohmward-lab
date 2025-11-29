# Resistor Divider Level Shifting

## Overview

Simple unidirectional level shifting using resistors. Only works for
high-to-low conversion (e.g., 5V → 3.3V).

## Formula

```
Vout = Vin × (R2 / (R1 + R2))
```

## Common Values for 5V → 3.3V

- R1 = 1kΩ, R2 = 2kΩ → Vout ≈ 3.33V
- R1 = 10kΩ, R2 = 20kΩ → Vout ≈ 3.33V (lower current draw)

## Limitations

- Unidirectional only (5V → 3.3V)
- Adds latency for fast signals
- Not suitable for I2C (needs bidirectional)
- Works well for slow signals like sensor outputs
