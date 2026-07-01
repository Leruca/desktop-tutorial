// Assignment 07: Web slider dimmer.
// ESP32 creates a Wi-Fi AP. Open http://192.168.4.1 and move the slider.

#include <WebServer.h>
#include <WiFi.h>

const char* apSsid = "ESP32-Dimmer";
const char* apPassword = "12345678";

const int ledPin = 2;
const int pwmFrequency = 5000;
const int pwmResolution = 8;

WebServer server(80);
int brightness = 0;

const char indexHtml[] PROGMEM = R"rawliteral(
<!doctype html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>ESP32 Dimmer</title>
  <style>
    body{font-family:Arial,sans-serif;margin:0;min-height:100vh;display:grid;place-items:center;background:#111;color:#fff}
    main{width:min(520px,88vw)}
    h1{font-size:28px;font-weight:600}
    input{width:100%}
    .value{font-size:56px;margin:24px 0}
  </style>
</head>
<body>
  <main>
    <h1>ESP32 Web Dimmer</h1>
    <div class="value" id="value">0</div>
    <input id="slider" type="range" min="0" max="255" value="0">
  </main>
  <script>
    const slider = document.getElementById('slider');
    const value = document.getElementById('value');
    slider.addEventListener('input', () => {
      value.textContent = slider.value;
      fetch('/set?value=' + slider.value).catch(() => {});
    });
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", indexHtml);
}

void handleSet() {
  if (server.hasArg("value")) {
    brightness = constrain(server.arg("value").toInt(), 0, 255);
    ledcWrite(ledPin, brightness);
  }
  server.send(200, "text/plain", String(brightness));
}

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, pwmFrequency, pwmResolution);
  ledcWrite(ledPin, brightness);

  WiFi.softAP(apSsid, apPassword);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
}

void loop() {
  server.handleClient();
}
