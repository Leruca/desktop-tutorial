# Ex 08 - IoT Security Alarm

ESP32 creates a Wi-Fi hotspot and serves a web control page with Arm and Disarm buttons.

## Use

1. Upload `ex08.ino`.
2. Connect phone or computer Wi-Fi to `ESP32-Alarm`.
3. Password: `12345678`.
4. Open `http://192.168.4.1`.
5. Click Arm.
6. Touch GPIO 4 to trigger the latched alarm.
7. Click Disarm to stop the flashing LED.

## Pins

- LED: GPIO 2
- Touch input: GPIO 4
