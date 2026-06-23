# Ex 02 - 1 Hz LED Blink With `millis()`

This assignment uses Arduino's `millis()` function to blink the ESP32 built-in LED at 1 Hz.

## `millis()` Notes

`millis()` returns the number of milliseconds since the board started running the current program. It can be used for timing without blocking the CPU with `delay()`.

## Timing

A 1 Hz blink cycle lasts 1000 ms:

- LED on: 500 ms
- LED off: 500 ms

The sketch toggles the LED every 500 ms using elapsed-time comparison:

```cpp
if (now - previousToggleMs >= toggleIntervalMs) {
  previousToggleMs = now;
  ledOn = !ledOn;
}
```

This subtraction form works correctly even when `millis()` overflows.
