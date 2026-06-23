// Assignment 02: Blink an LED at 1 Hz using millis().
// No delay() is used, so the timing remains stable and non-blocking.

const int ledPin = 2;
const unsigned long toggleIntervalMs = 500;

unsigned long previousToggleMs = 0;
bool ledOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  const unsigned long now = millis();

  if (now - previousToggleMs >= toggleIntervalMs) {
    previousToggleMs = now;
    ledOn = !ledOn;

    digitalWrite(ledPin, ledOn ? HIGH : LOW);
    Serial.println(ledOn ? "LED ON" : "LED OFF");
  }
}
