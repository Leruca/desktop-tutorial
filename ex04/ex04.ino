// Assignment 04: Touch self-locking LED switch.
// Touch GPIO 4 once to toggle the LED on, touch again to toggle it off.

const int ledPin = 2;
const int touchPin = 4;

const unsigned long debounceMs = 250;
const float thresholdRatio = 0.75;

int touchThreshold = 0;
bool ledState = false;
bool wasTouched = false;
unsigned long lastToggleMs = 0;

int readTouchBaseline() {
  long sum = 0;
  for (int i = 0; i < 30; i++) {
    sum += touchRead(touchPin);
    delay(20);
  }
  return sum / 30;
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  const int baseline = readTouchBaseline();
  touchThreshold = baseline * thresholdRatio;

  Serial.print("Touch baseline: ");
  Serial.println(baseline);
  Serial.print("Touch threshold: ");
  Serial.println(touchThreshold);
}

void loop() {
  const unsigned long now = millis();
  const int touchValue = touchRead(touchPin);
  const bool isTouched = touchValue < touchThreshold;

  if (isTouched && !wasTouched && now - lastToggleMs >= debounceMs) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);
    lastToggleMs = now;

    Serial.print("LED state: ");
    Serial.println(ledState ? "ON" : "OFF");
  }

  wasTouched = isTouched;
}
