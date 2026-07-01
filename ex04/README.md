# Ex 04 - Touch Self-Locking Switch

Touch GPIO 4 once to turn the LED on and keep it on. Release and touch again to turn the LED off.

## Pins

- LED: GPIO 2
- Touch input: GPIO 4

## Notes

The sketch uses edge detection and software debounce. It toggles only when the touch signal changes from not touched to touched.
