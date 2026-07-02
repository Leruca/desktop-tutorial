// Assignment 07: Web RGB/Y three-channel dimmer.
// ESP32 creates a Wi-Fi AP. Open http://192.168.4.1 to control GPIO 4, 16, and 17.

#include <WebServer.h>
#include <WiFi.h>

const char* apSsid = "ESP32-12574";
const char* apPassword = "12345678";

const int redPin = 4;
const int greenPin = 16;
const int yellowPin = 17;

const int pwmFrequency = 5000;
const int pwmResolution = 8;

WebServer server(80);

int redValue = 0;
int greenValue = 0;
int yellowValue = 0;

const char indexHtml[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="zh-CN">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>ESP32 Color Dimmer</title>
  <style>
    html,body{
      margin:0;
      padding:0;
      min-height:100%;
      font-family:Arial,"Microsoft YaHei",sans-serif;
      background:linear-gradient(135deg,#f8fbff,#ffeef8);
      color:#333;
    }
    body{
      overflow-x:hidden;
    }
    .content{
      position:relative;
      z-index:2;
      width:min(960px,92vw);
      margin:42px auto;
      text-align:center;
      text-shadow:0 2px 8px rgba(255,255,255,.9);
    }
    h1{
      font-size:32px;
      color:#ff5fa2;
      margin:0 0 10px;
    }
    .subtitle{
      color:#666;
      margin-bottom:24px;
    }
    .panel{
      display:grid;
      grid-template-columns:minmax(280px,1fr) minmax(280px,1fr);
      gap:22px;
      align-items:start;
    }
    .box{
      border-radius:18px;
      background:rgba(255,255,255,.62);
      box-shadow:0 12px 34px rgba(255,95,162,.18);
      backdrop-filter:blur(6px);
      padding:22px;
    }
    .preview{
      width:112px;
      height:112px;
      margin:0 auto 18px;
      border-radius:50%;
      background:#000;
      box-shadow:0 0 26px rgba(255,122,168,.55);
      border:5px solid rgba(255,255,255,.72);
    }
    .channel{
      display:grid;
      grid-template-columns:54px 1fr 76px;
      gap:12px;
      align-items:center;
      margin:16px 0;
      text-align:left;
    }
    .label{
      font-weight:700;
    }
    .red{color:#f44336}
    .green{color:#2eaf50}
    .yellow{color:#d49b00}
    input[type=range]{
      width:100%;
      cursor:pointer;
      accent-color:#ff7aa8;
    }
    input[type=number]{
      width:76px;
      box-sizing:border-box;
      border:2px solid #ff9ac1;
      border-radius:10px;
      padding:9px 8px;
      font-size:17px;
      text-align:center;
      outline:none;
      background:rgba(255,255,255,.92);
    }
    input[type=number]:focus{
      border-color:#ff5fa2;
      box-shadow:0 0 0 4px rgba(255,95,162,.18);
    }
    .palette-row{
      display:grid;
      grid-template-columns:1fr 116px;
      gap:18px;
      align-items:center;
    }
    .canvas-wrap{
      position:relative;
      width:min(360px,100%);
      aspect-ratio:1 / .88;
      margin:0 auto;
    }
    canvas{
      width:100%;
      height:100%;
      cursor:crosshair;
      filter:drop-shadow(0 10px 18px rgba(255,95,162,.18));
    }
    .marker{
      position:absolute;
      width:16px;
      height:16px;
      border:3px solid white;
      border-radius:50%;
      box-shadow:0 0 0 2px rgba(0,0,0,.35);
      transform:translate(-50%,-50%);
      pointer-events:none;
      left:50%;
      top:34%;
    }
    .palette-inputs{
      display:grid;
      gap:10px;
      text-align:left;
    }
    .palette-inputs label{
      display:grid;
      gap:5px;
      font-weight:700;
    }
    .hint{
      margin-top:16px;
      color:#666;
      font-size:14px;
      line-height:1.55;
    }
    .danmaku{
      position:absolute;
      white-space:nowrap;
      left:100%;
      animation-name:move;
      animation-timing-function:linear;
      animation-iteration-count:infinite;
      opacity:.75;
      pointer-events:none;
      z-index:1;
    }
    @keyframes move{
      from{transform:translateX(0)}
      to{transform:translateX(-140vw)}
    }
    .d1{top:5%;font-size:18px;color:#ff6b9d;animation-duration:9s;animation-delay:0s}
    .d2{top:15%;font-size:26px;color:#6c8cff;animation-duration:13s;animation-delay:-3s}
    .d3{top:28%;font-size:22px;color:#ffb347;animation-duration:11s;animation-delay:-5s}
    .d4{top:74%;font-size:28px;color:#7bd88f;animation-duration:16s;animation-delay:-8s}
    .d5{top:88%;font-size:18px;color:#c77dff;animation-duration:10s;animation-delay:-2s}
    @media (max-width:760px){
      .panel{grid-template-columns:1fr}
      .palette-row{grid-template-columns:1fr}
      .palette-inputs{grid-template-columns:repeat(3,1fr)}
      .channel{grid-template-columns:48px 1fr 70px}
    }
  </style>
</head>
<body>
  <main class="content">
    <h1>Three LED Color Dimmer</h1>
    <div class="subtitle">GPIO4 = Red, GPIO16 = Green, GPIO17 = Yellow</div>
    <div class="panel">
      <section class="box">
        <div class="preview" id="preview"></div>
        <div class="channel">
          <div class="label red">Red</div>
          <input id="redRange" type="range" min="0" max="255" value="0">
          <input id="redInput" type="number" min="0" max="255" value="0">
        </div>
        <div class="channel">
          <div class="label green">Green</div>
          <input id="greenRange" type="range" min="0" max="255" value="0">
          <input id="greenInput" type="number" min="0" max="255" value="0">
        </div>
        <div class="channel">
          <div class="label yellow">Yellow</div>
          <input id="yellowRange" type="range" min="0" max="255" value="0">
          <input id="yellowInput" type="number" min="0" max="255" value="0">
        </div>
        <div class="hint">Move sliders or type values from 0 to 255. Values are sent to the ESP32 immediately.</div>
      </section>

      <section class="box">
        <div class="palette-row">
          <div class="canvas-wrap" id="paletteWrap">
            <canvas id="palette" width="420" height="360"></canvas>
            <div class="marker" id="marker"></div>
          </div>
          <div class="palette-inputs">
            <label class="red">R <input id="redPalette" type="number" min="0" max="255" value="0"></label>
            <label class="green">G <input id="greenPalette" type="number" min="0" max="255" value="0"></label>
            <label class="yellow">Y <input id="yellowPalette" type="number" min="0" max="255" value="0"></label>
          </div>
        </div>
        <div class="hint">Click or drag inside the triangle. The position is converted to Red, Green, and Yellow brightness and synced to the sliders.</div>
      </section>
    </div>
  </main>

  <div class="danmaku d1">Ciallo~</div>
  <div class="danmaku d2">RGBY LED</div>
  <div class="danmaku d3">Color power!</div>
  <div class="danmaku d4">ESP32</div>
  <div class="danmaku d5">PWM</div>

  <script>
    const channels = {
      red: {
        range: document.getElementById('redRange'),
        input: document.getElementById('redInput'),
        palette: document.getElementById('redPalette'),
        value: 0
      },
      green: {
        range: document.getElementById('greenRange'),
        input: document.getElementById('greenInput'),
        palette: document.getElementById('greenPalette'),
        value: 0
      },
      yellow: {
        range: document.getElementById('yellowRange'),
        input: document.getElementById('yellowInput'),
        palette: document.getElementById('yellowPalette'),
        value: 0
      }
    };

    const preview = document.getElementById('preview');
    const canvas = document.getElementById('palette');
    const ctx = canvas.getContext('2d');
    const marker = document.getElementById('marker');
    const vertices = {
      red: {x: 210, y: 24},
      green: {x: 32, y: 326},
      yellow: {x: 388, y: 326}
    };

    function clamp(raw) {
      const parsed = Number.parseInt(raw, 10);
      if (Number.isNaN(parsed)) return 0;
      return Math.min(255, Math.max(0, parsed));
    }

    function colorFromValues() {
      const r = Math.min(255, channels.red.value + channels.yellow.value);
      const g = Math.min(255, channels.green.value + channels.yellow.value);
      return `rgb(${r},${g},0)`;
    }

    function syncUi() {
      for (const name of Object.keys(channels)) {
        const ch = channels[name];
        ch.range.value = ch.value;
        ch.input.value = ch.value;
        ch.palette.value = ch.value;
      }
      preview.style.background = colorFromValues();
      updateMarkerFromValues();
    }

    function sendValues() {
      const r = channels.red.value;
      const g = channels.green.value;
      const y = channels.yellow.value;
      fetch(`/set?r=${r}&g=${g}&y=${y}`).catch(() => {});
    }

    function setChannel(name, raw, send = true) {
      channels[name].value = clamp(raw);
      syncUi();
      if (send) sendValues();
    }

    function setAll(red, green, yellow, send = true) {
      channels.red.value = clamp(red);
      channels.green.value = clamp(green);
      channels.yellow.value = clamp(yellow);
      syncUi();
      if (send) sendValues();
    }

    function barycentric(px, py) {
      const a = vertices.red;
      const b = vertices.green;
      const c = vertices.yellow;
      const denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
      const wRed = ((b.y - c.y) * (px - c.x) + (c.x - b.x) * (py - c.y)) / denom;
      const wGreen = ((c.y - a.y) * (px - c.x) + (a.x - c.x) * (py - c.y)) / denom;
      const wYellow = 1 - wRed - wGreen;
      return {red: wRed, green: wGreen, yellow: wYellow};
    }

    function insideTriangle(weights) {
      return weights.red >= 0 && weights.green >= 0 && weights.yellow >= 0;
    }

    function drawPalette() {
      const image = ctx.createImageData(canvas.width, canvas.height);
      for (let y = 0; y < canvas.height; y++) {
        for (let x = 0; x < canvas.width; x++) {
          const weights = barycentric(x, y);
          const offset = (y * canvas.width + x) * 4;
          if (insideTriangle(weights)) {
            const red = Math.round(255 * weights.red);
            const green = Math.round(255 * weights.green);
            const yellow = Math.round(255 * weights.yellow);
            image.data[offset] = Math.min(255, red + yellow);
            image.data[offset + 1] = Math.min(255, green + yellow);
            image.data[offset + 2] = 0;
            image.data[offset + 3] = 255;
          } else {
            image.data[offset + 3] = 0;
          }
        }
      }
      ctx.putImageData(image, 0, 0);
      ctx.lineWidth = 4;
      ctx.strokeStyle = 'rgba(255,255,255,.9)';
      ctx.beginPath();
      ctx.moveTo(vertices.red.x, vertices.red.y);
      ctx.lineTo(vertices.green.x, vertices.green.y);
      ctx.lineTo(vertices.yellow.x, vertices.yellow.y);
      ctx.closePath();
      ctx.stroke();
    }

    function updateMarkerFromValues() {
      const total = channels.red.value + channels.green.value + channels.yellow.value;
      let x;
      let y;
      if (total === 0) {
        x = (vertices.red.x + vertices.green.x + vertices.yellow.x) / 3;
        y = (vertices.red.y + vertices.green.y + vertices.yellow.y) / 3;
      } else {
        x = (vertices.red.x * channels.red.value + vertices.green.x * channels.green.value + vertices.yellow.x * channels.yellow.value) / total;
        y = (vertices.red.y * channels.red.value + vertices.green.y * channels.green.value + vertices.yellow.y * channels.yellow.value) / total;
      }
      marker.style.left = `${(x / canvas.width) * 100}%`;
      marker.style.top = `${(y / canvas.height) * 100}%`;
    }

    function setFromPaletteEvent(event) {
      const rect = canvas.getBoundingClientRect();
      const x = (event.clientX - rect.left) * canvas.width / rect.width;
      const y = (event.clientY - rect.top) * canvas.height / rect.height;
      const weights = barycentric(x, y);
      if (!insideTriangle(weights)) return;
      setAll(
        Math.round(weights.red * 255),
        Math.round(weights.green * 255),
        Math.round(weights.yellow * 255)
      );
    }

    for (const name of Object.keys(channels)) {
      const ch = channels[name];
      ch.range.addEventListener('input', () => setChannel(name, ch.range.value));
      ch.input.addEventListener('change', () => setChannel(name, ch.input.value));
      ch.input.addEventListener('keydown', (event) => {
        if (event.key === 'Enter') setChannel(name, ch.input.value);
      });
      ch.palette.addEventListener('change', () => setChannel(name, ch.palette.value));
      ch.palette.addEventListener('keydown', (event) => {
        if (event.key === 'Enter') setChannel(name, ch.palette.value);
      });
    }

    let dragging = false;
    canvas.addEventListener('pointerdown', (event) => {
      dragging = true;
      canvas.setPointerCapture(event.pointerId);
      setFromPaletteEvent(event);
    });
    canvas.addEventListener('pointermove', (event) => {
      if (dragging) setFromPaletteEvent(event);
    });
    canvas.addEventListener('pointerup', () => dragging = false);
    canvas.addEventListener('pointercancel', () => dragging = false);

    drawPalette();
    syncUi();
  </script>
</body>
</html>
)rawliteral";

void applyLedValues() {
  ledcWrite(redPin, redValue);
  ledcWrite(greenPin, greenValue);
  ledcWrite(yellowPin, yellowValue);
}

void handleRoot() {
  server.send(200, "text/html", indexHtml);
}

void handleSet() {
  if (server.hasArg("r")) {
    redValue = constrain(server.arg("r").toInt(), 0, 255);
  }
  if (server.hasArg("g")) {
    greenValue = constrain(server.arg("g").toInt(), 0, 255);
  }
  if (server.hasArg("y")) {
    yellowValue = constrain(server.arg("y").toInt(), 0, 255);
  }

  applyLedValues();

  String response = "{\"r\":";
  response += redValue;
  response += ",\"g\":";
  response += greenValue;
  response += ",\"y\":";
  response += yellowValue;
  response += "}";
  server.send(200, "application/json", response);
}

void setup() {
  Serial.begin(115200);

  ledcAttach(redPin, pwmFrequency, pwmResolution);
  ledcAttach(greenPin, pwmFrequency, pwmResolution);
  ledcAttach(yellowPin, pwmFrequency, pwmResolution);
  applyLedValues();

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
