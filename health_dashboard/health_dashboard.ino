#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "MAX30105.h"

const char* ssid = "RASHMI";
const char* password = "Rashmi@1980";

WebServer server(80);
MAX30105 particleSensor;

long irValue = 0;
int bpm = 0;
String fingerStatus = "Not Detected";
String healthStatus = "No Data";

void handleRoot()
{
  String html = R"====(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta http-equiv="refresh" content="2">
<meta name="viewport" content="width=device-width, initial-scale=1">

<title>Health Analytics Dashboard</title>

<style>
body{
    margin:0;
    font-family:Arial, sans-serif;
    background:#eef2f7;
    text-align:center;
}

.header{
    background:#2c3e50;
    color:white;
    padding:20px;
    font-size:28px;
    font-weight:bold;
}

.container{
    display:flex;
    flex-wrap:wrap;
    justify-content:center;
    margin-top:20px;
}

.card{
    background:white;
    width:280px;
    margin:15px;
    padding:20px;
    border-radius:15px;
    box-shadow:0 4px 10px rgba(0,0,0,0.15);
}

.title{
    font-size:20px;
    color:#555;
}

.value{
    font-size:40px;
    font-weight:bold;
    color:#3498db;
    margin-top:10px;
}

.status{
    font-size:28px;
    font-weight:bold;
    color:#27ae60;
}

.footer{
    margin-top:30px;
    color:#666;
}
</style>
</head>

<body>

<div class="header">
Real-Time Health Analytics Dashboard
</div>

<div class="container">

<div class="card">
<div class="title">Heart Rate</div>
<div class="value">
)====";

  html += String(bpm);

  html += R"====(
 BPM
</div>
</div>

<div class="card">
<div class="title">IR Signal</div>
<div class="value">
)====";

  html += String(irValue);

  html += R"====(
</div>
</div>

<div class="card">
<div class="title">Finger Status</div>
<div class="status">
)====";

  html += fingerStatus;

  html += R"====(
</div>
</div>

<div class="card">
<div class="title">Health Status</div>
<div class="status">
)====";

  html += healthStatus;

  html += R"====(
</div>
</div>

</div>

<div class="footer">
ESP32 Web Dashboard | Auto Refresh: 2s
</div>

</body>
</html>
)====";

  server.send(200, "text/html", html);
}

void setup()
{
  Serial.begin(115200);

  Wire.begin(21,22);

  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD))
  {
    Serial.println("MAX30102 not found!");
    while(1);
  }

  particleSensor.setup();

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();

  Serial.println("Server Started");
}

void loop()
{
  irValue = particleSensor.getIR();

  if(irValue > 50000)
  {
    fingerStatus = "Detected";

    // Temporary BPM estimation for dashboard demo
    bpm = map(irValue, 50000, 120000, 60, 100);

    if(bpm < 60)
      healthStatus = "Low";
    else if(bpm <= 100)
      healthStatus = "Normal";
    else
      healthStatus = "High";
  }
  else
  {
    fingerStatus = "Not Detected";
    bpm = 0;
    healthStatus = "No Data";
  }

  server.handleClient();
}