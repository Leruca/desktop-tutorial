# Lab 03 - PWM Breathing LED

This sketch completes the PWM breathing LED task from lab03.

## Hardware

- ESP32 development board
- Built-in LED on GPIO 2
- USB cable

## Behavior

The LED smoothly becomes brighter, then darker, using ESP32 PWM.

## Upload

1. Open `lab03.ino` in Arduino IDE.
2. Select `ESP32 Dev Module`.
3. Select the correct COM port.
4. Upload and open Serial Monitor at `115200`.

The code uses the ESP32 Arduino core 3.x `ledcAttach()` API.
