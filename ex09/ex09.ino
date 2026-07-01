// Assignment 09: Real-time touch sensor web dashboard.
// ESP32 serves a webpage that repeatedly fetches and displays touchRead(GPIO 4).

#include <WebServer.h>
#include <WiFi.h>

const char* apSsid = "ESP32-Dashboard";
const char* apPassword = "12345678";

const int touchPin = 4;

WebServer server(80);

const char indexHtml[] PROGMEM = R"rawliteral(
<!doctype html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>ESP32 Touch Dashboard</title>
  <style>
    body{font-family:Arial,sans-serif;margin:0;min-height:100vh;display:grid;place-items:center;background:#0f1720;color:#fff}
    main{text-align:center}
    .number{font-size:82px;font-weight:700;margin:18px 0}
    .label{font-size:18px;color:#9fb3c8}
  </style>
</head>
<body>
  <main>
    <div class="label">Touch sensor value</div>
    <div class="number" id="value">--</div>
    <div class="label">Move your finger near GPIO 4</div>
  </main>
  <script>
    async function refresh(){
      const r = await fetch('/value');
      document.getElementById('value').textContent = await r.text();
    }
    setInterval(refresh, 150);
    refresh();
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", indexHtml);
}

void handleValue() {
  server.send(200, "text/plain", String(touchRead(touchPin)));
}

void setup() {
  Serial.begin(115200);

  WiFi.softAP(apSsid, apPassword);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/value", handleValue);
  server.begin();
}

void loop() {
  server.handleClient();
}
