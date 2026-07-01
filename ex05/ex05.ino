// Assignment 05: Three-speed touch-controlled breathing LED.
// Each touch on GPIO 4 cycles the breathing speed: slow -> medium -> fast.

const int ledPin = 2;
const int touchPin = 4;

const int pwmFrequency = 5000;
const int pwmResolution = 8;
const int maxDutyCycle = 255;

const unsigned long debounceMs = 250;
const float thresholdRatio = 0.75;
const unsigned long speedIntervalsMs[] = {18, 8, 3};

int touchThreshold = 0;
bool wasTouched = false;
unsigned long lastTouchMs = 0;

int speedLevel = 0;
int dutyCycle = 0;
int fadeDirection = 1;
unsigned long lastFadeMs = 0;

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
  ledcAttach(ledPin, pwmFrequency, pwmResolution);

  const int baseline = readTouchBaseline();
  touchThreshold = baseline * thresholdRatio;

  Serial.print("Touch baseline: ");
  Serial.println(baseline);
  Serial.println("Speed level: 1");
}

void loop() {
  const unsigned long now = millis();
  const int touchValue = touchRead(touchPin);
  const bool isTouched = touchValue < touchThreshold;

  if (isTouched && !wasTouched && now - lastTouchMs >= debounceMs) {
    speedLevel = (speedLevel + 1) % 3;
    lastTouchMs = now;

    Serial.print("Speed level: ");
    Serial.println(speedLevel + 1);
  }
  wasTouched = isTouched;

  if (now - lastFadeMs >= speedIntervalsMs[speedLevel]) {
    lastFadeMs = now;
    dutyCycle += fadeDirection;

    if (dutyCycle >= maxDutyCycle) {
      dutyCycle = maxDutyCycle;
      fadeDirection = -1;
    } else if (dutyCycle <= 0) {
      dutyCycle = 0;
      fadeDirection = 1;
    }

    ledcWrite(ledPin, dutyCycle);
  }
}
