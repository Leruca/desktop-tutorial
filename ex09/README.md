# Ex 09 - Real-Time Touch Dashboard

ESP32 creates a Wi-Fi hotspot and serves a web dashboard that shows the live `touchRead()` value.

## Use

1. Upload `ex09.ino`.
2. Connect phone or computer Wi-Fi to `ESP32-Dashboard`.
3. Password: `12345678`.
4. Open `http://192.168.4.1`.
5. Move your finger close to GPIO 4. The displayed number should change in real time.

## Pin

- Touch input: GPIO 4
