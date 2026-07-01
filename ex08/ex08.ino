// Assignment 08: IoT security alarm simulation.
// Web buttons arm/disarm the system. Touch GPIO 4 triggers a latched alarm.

#include <WebServer.h>
#include <WiFi.h>

const char* apSsid = "ESP32-Alarm";
const char* apPassword = "12345678";

const int ledPin = 2;
const int touchPin = 4;

const unsigned long alarmBlinkMs = 80;
const float thresholdRatio = 0.75;

WebServer server(80);

int touchThreshold = 0;
bool armed = false;
bool alarmActive = false;
bool ledOn = false;
unsigned long lastBlinkMs = 0;

const char indexHtml[] PROGMEM = R"rawliteral(
<!doctype html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>ESP32 Security</title>
  <style>
    body{font-family:Arial,sans-serif;margin:0;min-height:100vh;display:grid;place-items:center;background:#161616;color:#fff}
    main{width:min(520px,88vw);text-align:center}
    .status{font-size:32px;margin:24px 0}
    button{font-size:22px;padding:14px 22px;margin:8px;border:0;border-radius:6px}
  </style>
</head>
<body>
  <main>
    <h1>Security Alarm</h1>
    <div class="status" id="status">Loading</div>
    <button onclick="send('/arm')">Arm</button>
    <button onclick="send('/disarm')">Disarm</button>
  </main>
  <script>
    async function send(path){ await fetch(path); refresh(); }
    async function refresh(){
      const r = await fetch('/status');
      document.getElementById('status').textContent = await r.text();
    }
    setInterval(refresh, 500);
    refresh();
  </script>
</body>
</html>
)rawliteral";

int readTouchBaseline() {
  long sum = 0;
  for (int i = 0; i < 30; i++) {
    sum += touchRead(touchPin);
    delay(20);
  }
  return sum / 30;
}

void handleRoot() {
  server.send(200, "text/html", indexHtml);
}

void handleArm() {
  armed = true;
  alarmActive = false;
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "ARMED");
}

void handleDisarm() {
  armed = false;
  alarmActive = false;
  ledOn = false;
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "DISARMED");
}

void handleStatus() {
  if (alarmActive) {
    server.send(200, "text/plain", "ALARM");
  } else if (armed) {
    server.send(200, "text/plain", "ARMED");
  } else {
    server.send(200, "text/plain", "DISARMED");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  const int baseline = readTouchBaseline();
  touchThreshold = baseline * thresholdRatio;

  WiFi.softAP(apSsid, apPassword);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.on("/status", handleStatus);
  server.begin();
}

void loop() {
  server.handleClient();

  if (armed && touchRead(touchPin) < touchThreshold) {
    alarmActive = true;
  }

  if (alarmActive) {
    const unsigned long now = millis();
    if (now - lastBlinkMs >= alarmBlinkMs) {
      lastBlinkMs = now;
      ledOn = !ledOn;
      digitalWrite(ledPin, ledOn ? HIGH : LOW);
    }
  }
}
