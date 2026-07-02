# Ex 07 - Web Three-LED Color Dimmer

ESP32 creates a Wi-Fi hotspot and serves a webpage with three brightness sliders, three text inputs, and a triangular color area.

## Use

1. Upload `ex07.ino`.
2. Connect phone or computer Wi-Fi to `ESP32-12574`.
3. Password: `12345678`.
4. Open `http://192.168.4.1`.
5. Drag the Red, Green, and Yellow sliders or type values from `0` to `255`.
6. Click or drag inside the triangle to sync the three channel values automatically.

## Pins

- Red LED: GPIO 4
- Green LED: GPIO 16
- Yellow LED: GPIO 17
