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
<html lang="zh-CN">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>ESP32 可爱调光器</title>
  <style>
    html,body{
      margin:0;
      padding:0;
      width:100%;
      min-height:100%;
      overflow:hidden;
      font-family:Arial,"Microsoft YaHei",sans-serif;
      background:linear-gradient(135deg,#f8fbff,#ffeef8);
    }
    .content{
      position:relative;
      z-index:2;
      width:min(560px,86vw);
      margin:72px auto 0;
      text-align:center;
      color:#333;
      text-shadow:0 2px 8px rgba(255,255,255,.9);
    }
    h1{
      font-size:32px;
      color:#ff5fa2;
      margin:0 0 18px;
      font-weight:700;
    }
    .subtitle{
      font-size:16px;
      color:#6d6d78;
      margin-bottom:26px;
    }
    .value{
      display:inline-flex;
      align-items:center;
      justify-content:center;
      min-width:132px;
      min-height:72px;
      padding:0 22px;
      border-radius:999px;
      color:#fff;
      font-size:44px;
      font-weight:700;
      background:#ff7aa8;
      box-shadow:0 0 24px rgba(255,122,168,.62);
      margin-bottom:28px;
    }
    .slider-wrap{
      padding:22px 22px 26px;
      border-radius:18px;
      background:rgba(255,255,255,.55);
      box-shadow:0 12px 34px rgba(255,95,162,.18);
      backdrop-filter:blur(6px);
    }
    input[type=range]{
      width:100%;
      accent-color:#ff5fa2;
      cursor:pointer;
    }
    .hint{
      margin-top:16px;
      color:#666;
      font-size:15px;
    }
    .danmaku{
      position:absolute;
      white-space:nowrap;
      left:100%;
      animation-name:move;
      animation-timing-function:linear;
      animation-iteration-count:infinite;
      opacity:.85;
      pointer-events:none;
      z-index:1;
    }
    @keyframes move{
      from{transform:translateX(0)}
      to{transform:translateX(-140vw)}
    }
    .d1{top:5%;font-size:18px;color:#ff6b9d;animation-duration:9s;animation-delay:0s}
    .d2{top:12%;font-size:28px;color:#6c8cff;animation-duration:13s;animation-delay:-3s}
    .d3{top:20%;font-size:22px;color:#ffb347;animation-duration:11s;animation-delay:-5s}
    .d4{top:28%;font-size:34px;color:#7bd88f;animation-duration:16s;animation-delay:-8s}
    .d5{top:38%;font-size:16px;color:#c77dff;animation-duration:10s;animation-delay:-2s}
    .d6{top:48%;font-size:26px;color:#ff5c5c;animation-duration:15s;animation-delay:-7s}
    .d7{top:58%;font-size:20px;color:#00bcd4;animation-duration:12s;animation-delay:-4s}
    .d8{top:68%;font-size:36px;color:#ff8fab;animation-duration:18s;animation-delay:-9s}
    .d9{top:78%;font-size:24px;color:#4caf50;animation-duration:14s;animation-delay:-6s}
    .d10{top:88%;font-size:18px;color:#9c27b0;animation-duration:10s;animation-delay:-1s}
  </style>
</head>
<body>
  <main class="content">
    <h1>第七部分：可爱无极调光器</h1>
    <div class="subtitle">拖动滑条，板载 LED 会实时改变亮度</div>
    <div class="value" id="value">0</div>
    <div class="slider-wrap">
      <input id="slider" type="range" min="0" max="255" value="0">
      <div class="hint">亮度范围：0 - 255</div>
    </div>
  </main>
  <div class="danmaku d1">Ciallo~ (&lt;・ω&lt; )⌒★</div>
  <div class="danmaku d2">(๑&gt;◡&lt;๑)</div>
  <div class="danmaku d3">ヾ(≧▽≦*)o</div>
  <div class="danmaku d4">٩(ˊᗜˋ*)و</div>
  <div class="danmaku d5">(｡･ω･｡)</div>
  <div class="danmaku d6">(*≧ω≦)</div>
  <div class="danmaku d7">(ง •̀_•́)ง</div>
  <div class="danmaku d8">LED power!</div>
  <div class="danmaku d9">(＾▽＾)</div>
  <div class="danmaku d10">(￣▽￣)ノ</div>
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
