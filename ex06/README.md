# Ex 06 - Dual PWM Alternating Fade

Two LEDs fade in opposite directions. When LED A becomes brighter, LED B becomes darker.

## Pins

- LED A: GPIO 18
- LED B: GPIO 19
- Each LED must use a 220 ohm or 330 ohm series resistor to GND.

## Wiring

For each LED:

`GPIO -> resistor -> LED long leg -> LED short leg -> GND`

If the LED does not light, reverse the LED direction.
