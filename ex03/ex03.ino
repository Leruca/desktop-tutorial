// Assignment 03: SOS LED signal using millis().
// Pattern: short-short-short, long-long-long, short-short-short, then a long pause.

const int ledPin = 2;

const unsigned long shortFlashMs = 200;
const unsigned long longFlashMs = 600;
const unsigned long symbolGapMs = 200;
const unsigned long letterGapMs = 600;
const unsigned long messageGapMs = 2000;

struct SosStep {
  bool ledOn;
  unsigned long durationMs;
};

const SosStep sosPattern[] = {
  {true, shortFlashMs}, {false, symbolGapMs},
  {true, shortFlashMs}, {false, symbolGapMs},
  {true, shortFlashMs}, {false, letterGapMs},

  {true, longFlashMs}, {false, symbolGapMs},
  {true, longFlashMs}, {false, symbolGapMs},
  {true, longFlashMs}, {false, letterGapMs},

  {true, shortFlashMs}, {false, symbolGapMs},
  {true, shortFlashMs}, {false, symbolGapMs},
  {true, shortFlashMs}, {false, messageGapMs},
};

const int sosStepCount = sizeof(sosPattern) / sizeof(sosPattern[0]);

int currentStep = 0;
unsigned long stepStartedMs = 0;

void applyCurrentStep() {
  digitalWrite(ledPin, sosPattern[currentStep].ledOn ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  applyCurrentStep();
  Serial.println("SOS pattern started");
}

void loop() {
  const unsigned long now = millis();
  const SosStep step = sosPattern[currentStep];

  if (now - stepStartedMs >= step.durationMs) {
    currentStep = (currentStep + 1) % sosStepCount;
    stepStartedMs = now;
    applyCurrentStep();

    if (currentStep == 0) {
      Serial.println("SOS pattern repeated");
    }
  }
}
