#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "wifi_secrets.h"

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
const int micPin = 34;

WebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Rasne Gorsel Telsiz</title>
<style>
  body { background-color: #111; color: #0f0; font-family: monospace; text-align: center; margin:0; padding: 20px; }
  canvas { border: 1px solid #333; background-color: #000; width: 100%; max-width: 500px; height: 200px; }
  button { padding: 15px 30px; font-size: 20px; background-color: #d00; color: white; border: none; border-radius: 5px; margin-top: 20px; cursor: pointer; }
  #status { color: #888; margin-bottom: 10px; }
</style>
</head>
<body>
  <h2>WI-FI GORSEL TELSİZ</h2>
  <div id="status">Baglanti Bekleniyor...</div>
  
  <canvas id="visualizer"></canvas>
  
  <audio id="player" crossorigin="anonymous" style="display:none"></audio>
  
  <br>
  <button id="btn" onclick="baslat()">YAYINI BASLAT</button>

  <script>
    var audioCtx, analyser, dataArray, canvas, ctx;
    var player = document.getElementById('player');
    var btn = document.getElementById('btn');
    var isPlaying = false;

    function baslat() {
      if (isPlaying) return;
      isPlaying = true;
      btn.innerText = "BAGLANIYOR...";
      btn.style.backgroundColor = "#555";
      
      audioCtx = new (window.AudioContext || window.webkitAudioContext)();
      analyser = audioCtx.createAnalyser();
      analyser.fftSize = 256; // Detay seviyesi
      
      player.src = "/stream";
      player.play();

 
      var source = audioCtx.createMediaElementSource(player);
      source.connect(analyser);
      analyser.connect(audioCtx.destination);

      
      canvas = document.getElementById("visualizer");
      ctx = canvas.getContext("2d");
      dataArray = new Uint8Array(analyser.frequencyBinCount);
      
      document.getElementById('status').innerText = "YAYINDA - Dinleniyor...";
      btn.style.display = 'none'; 
      
      ciz();
    }

    function ciz() {
      requestAnimationFrame(ciz);

      
      analyser.getByteTimeDomainData(dataArray);

  
      ctx.fillStyle = 'rgb(0, 0, 0)';
      ctx.fillRect(0, 0, canvas.width, canvas.height);
      ctx.lineWidth = 2;
      ctx.strokeStyle = 'rgb(0, 255, 0)';
      ctx.beginPath();

      var sliceWidth = canvas.width * 1.0 / dataArray.length;
      var x = 0;

      for(var i = 0; i < dataArray.length; i++) {
        var v = dataArray[i] / 128.0; // Veriyi normalize et
        var y = v * canvas.height / 2;

        if(i === 0) ctx.moveTo(x, y);
        else ctx.lineTo(x, y);

        x += sliceWidth;
      }

      ctx.lineTo(canvas.width, canvas.height / 2);
      ctx.stroke();
    }
  </script>
</body>
</html>
)rawliteral";

// 16 bitlik (0x)
/*
  BYTE 0–3   : "RIFF"
  BYTE 4–7   : ChunkSize
  BYTE 8–11  : "WAVE"
  BYTE 12–15 : "fmt "
  BYTE 16–19 : Subchunk1Size
  BYTE 20–21 : AudioFormat
  BYTE 22–23 : NumChannels
  BYTE 24–27 : SampleRate
  BYTE 28–31 : ByteRate
  BYTE 32–33 : BlockAlign
  BYTE 34–35 : BitsPerSample
  BYTE 36–39 : "data"
  BYTE 40–43 : Subchunk2Size
*/

const uint8_t wavHeader[44] = {
    0x52, 0x49, 0x46, 0x46, 0xFF, 0xFF, 0xFF, 0xFF,
    0x57, 0x41, 0x56, 0x45, 0x66, 0x6d, 0x74, 0x20,
    0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
    0x40, 0x1F, 0x00, 0x00, 0x40, 0x1F, 0x00, 0x00,
    0x01, 0x00, 0x08, 0x00, 0x64, 0x61, 0x74, 0x61,
    0xFF, 0xFF, 0xFF, 0xFF};

void streamAudio()
{
  WiFiClient client = server.client();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: audio/wav");
  client.println("Connection: close");
  client.println();
  client.write(wavHeader, 44);

  uint8_t buffer[64];

  while (client.connected())
  {

    for (int i = 0; i < 64; i++)
    {
      int val = analogRead(micPin);

      buffer[i] = map(val, 0, 4095, 0, 255);
      delayMicroseconds(32); // 8kHz örnekleme hızı için bekleme
    }
    client.write(buffer, 64);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(micPin, INPUT);

  WiFi.softAP(ssid, password);
  Serial.println("Gorsel Telsiz Basladi!");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []()
            { server.send(200, "text/html", index_html); });
  server.on("/stream", streamAudio);

  server.begin();
}

void loop()
{
  server.handleClient();
}
