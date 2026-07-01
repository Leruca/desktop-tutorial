// Assignment 06: Two-channel PWM alternating fade.
// Connect two LEDs to two different GPIO pins. Their brightness is reversed.

const int ledPinA = 18;
const int ledPinB = 19;

const int pwmFrequency = 5000;
const int pwmResolution = 8;
const int maxDutyCycle = 255;
const unsigned long fadeIntervalMs = 8;

int dutyCycleA = 0;
int fadeDirection = 1;
unsigned long lastFadeMs = 0;

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPinA, pwmFrequency, pwmResolution);
  ledcAttach(ledPinB, pwmFrequency, pwmResolution);
}

void loop() {
  const unsigned long now = millis();

  if (now - lastFadeMs >= fadeIntervalMs) {
    lastFadeMs = now;
    dutyCycleA += fadeDirection;

    if (dutyCycleA >= maxDutyCycle) {
      dutyCycleA = maxDutyCycle;
      fadeDirection = -1;
    } else if (dutyCycleA <= 0) {
      dutyCycleA = 0;
      fadeDirection = 1;
    }

    ledcWrite(ledPinA, dutyCycleA);
    ledcWrite(ledPinB, maxDutyCycle - dutyCycleA);
  }
}
