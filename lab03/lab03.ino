// Lab 03: PWM breathing LED.
// Board: ESP32 Dev Module
// Uses ESP32 Arduino core 3.x ledcAttach/ledcWrite API.

const int ledPin = 2;
const int pwmFrequency = 5000;
const int pwmResolution = 8;
const int maxDutyCycle = 255;

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, pwmFrequency, pwmResolution);
}

void loop() {
  for (int dutyCycle = 0; dutyCycle <= maxDutyCycle; dutyCycle++) {
    ledcWrite(ledPin, dutyCycle);
    delay(10);
  }

  for (int dutyCycle = maxDutyCycle; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(ledPin, dutyCycle);
    delay(10);
  }

  Serial.println("Breathing cycle completed");
}
