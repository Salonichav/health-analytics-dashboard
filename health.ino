#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

MAX30105 particleSensor;

#define IR_SENSOR_PIN 27
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);
// =========================
// BPM Variables
// =========================
const byte RATE_SIZE = 4;

byte rates[RATE_SIZE];
byte rateSpot = 0;

long lastBeat = 0;

float beatsPerMinute;
int beatAvg = 0;

volatile int bpm = 0;

volatile bool patientDetected = false;
volatile bool fingerDetected = false;

const char* ssid = "Wokwi-GUEST";
const char* password = "";

TimerHandle_t systemTimer;

volatile const char* healthStatus = "Idle";

// =========================
// Task Handles
// =========================
TaskHandle_t PatientTaskHandle;
TaskHandle_t HeartTaskHandle;
TaskHandle_t DisplayTaskHandle;
TaskHandle_t WebTaskHandle;
// =========================
// RTOS Objects
// =========================
QueueHandle_t bpmQueue;
SemaphoreHandle_t serialMutex;


void handleRoot()
{
  String html = "<html><head><meta http-equiv='refresh' content='2'/></head><body>";
  html += "<h1>ESP32 Heart Rate Monitor</h1>";
  html += "<p>Patient: " + String(patientDetected ? "Detected" : "Not Detected") + "</p>";
  html += "<p>Finger: " + String(fingerDetected ? "Detected" : "Not Detected") + "</p>";
  html += "<p>BPM: " + String(bpm) + "</p>";
  html += "<p>Health: " + String((const char*)healthStatus) + "</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleData()
{
  String json = "{";
  json += "\"patient\":" + String(patientDetected ? "true" : "false") + ",";
  json += "\"finger\":" + String(fingerDetected ? "true" : "false") + ",";
  json += "\"bpm\":" + String(bpm) + ",";
  json += "\"health\":\"" + String((const char*)healthStatus) + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

void timerCallback(TimerHandle_t xTimer)
{
  Serial.println("System Alive");
}


void WebTask(void *pvParameters)
{
  while (true)
  {
    server.handleClient();

    vTaskDelay(
      pdMS_TO_TICKS(20));
  }
}
// =========================
// Patient Detection Task
// =========================
void PatientTask(void *pvParameters)
{
  while (true)
  {
    patientDetected =
      (digitalRead(IR_SENSOR_PIN) == LOW);

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// =========================
// Heart Rate Task
// =========================
void HeartTask(void *pvParameters)
{
  while (true)
  {
    long irValue = particleSensor.getIR();

    fingerDetected = (irValue > 50000);

    if (fingerDetected)
    {
      if (checkForBeat(irValue))
      {
        if (lastBeat != 0)
        {
          long delta = millis() - lastBeat;
          beatsPerMinute = 60 / (delta / 1000.0);

          if (beatsPerMinute > 20 && beatsPerMinute < 255)
          {
            if (rates[0] == 0)
            {
              // Initialize all elements with first reading to avoid dilution with zeros
              for (byte x = 0; x < RATE_SIZE; x++)
              {
                rates[x] = (byte)beatsPerMinute;
              }
              rateSpot = 1;
            }
            else
            {
              rates[rateSpot++] = (byte)beatsPerMinute;
              rateSpot %= RATE_SIZE;
            }

            beatAvg = 0;
            for (byte x = 0; x < RATE_SIZE; x++)
            {
              beatAvg += rates[x];
            }
            beatAvg /= RATE_SIZE;

            bpm = beatAvg;

            // Send latest BPM to queue (copying to temp to avoid volatile warning)
            int tempBpm = bpm;
            xQueueOverwrite(bpmQueue, &tempBpm);
          }
        }
        lastBeat = millis();
      }
    }
    else
    {
      bpm = 0;
      
      // Send 0 to queue when finger is removed
      int tempBpm = 0;
      xQueueOverwrite(bpmQueue, &tempBpm);
      
      // Reset averages and beat timings for a fresh session
      rateSpot = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
      {
        rates[x] = 0;
      }
      lastBeat = 0;
    }

    if (bpm == 0)
    {
      healthStatus = "Measuring";
    }
    else if (bpm < 60)
    {
      healthStatus = "Low Heart Rate";
    }
    else if (bpm <= 100)
    {
      healthStatus = "Normal";
    }
    else
    {
      healthStatus = "High Heart Rate";
    }

    // VERY IMPORTANT
    // Keep delay tiny
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

// =========================
// Display Task
// =========================
void DisplayTask(void *pvParameters)
{
  while (true)
  {
    int displayBPM = 0;

    xQueuePeek(bpmQueue,
               &displayBPM,
               0);

    xSemaphoreTake(
      serialMutex,
      portMAX_DELAY);

    Serial.println();
    Serial.println("====================");

    Serial.print("Patient : ");

    if (patientDetected)
      Serial.println("Detected");
    else
      Serial.println("Not Detected");

    Serial.print("Finger  : ");

    if (fingerDetected)
      Serial.println("Detected");
    else
      Serial.println("Not Detected");

    Serial.print("BPM     : ");
    Serial.println(displayBPM);

    Serial.print("Health  : ");
    Serial.println((const char*)healthStatus);

    Serial.println("====================");

    xSemaphoreGive(serialMutex);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// =========================
// Setup
// =========================
void setup()
{
  Serial.begin(115200);

  pinMode(IR_SENSOR_PIN, INPUT);

  Wire.begin(21, 22);

  Serial.println("Initializing MAX30102...");

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    Serial.println("MAX30102 NOT FOUND");

    while (1);
  }

  Serial.println("MAX30102 FOUND");

  particleSensor.setup();

  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();

  // =========================
  // RTOS Objects
  // =========================
  bpmQueue = xQueueCreate(1, sizeof(int));

  serialMutex = xSemaphoreCreateMutex();

  if (bpmQueue == NULL || serialMutex == NULL)
  {
    Serial.println("Failed to create RTOS objects");
    while (1);
  }

  int initialBpm = 0;
  xQueueOverwrite(bpmQueue, &initialBpm);

  systemTimer = xTimerCreate(
    "HealthTimer",
    pdMS_TO_TICKS(1000),
    pdTRUE,
    NULL,
    timerCallback);

  if (systemTimer == NULL)
  {
    Serial.println("Failed to create Software Timer");
    while (1);
  }
  xTimerStart(systemTimer, 0);

  // Core 0
  xTaskCreatePinnedToCore(
    PatientTask,
    "PatientTask",
    2000,
    NULL,
    1,
    &PatientTaskHandle,
    0);

  // Core 1
  xTaskCreatePinnedToCore(
    HeartTask,
    "HeartTask",
    5000,
    NULL,
    2,
    &HeartTaskHandle,
    1);

  // Core 0
  xTaskCreatePinnedToCore(
    DisplayTask,
    "DisplayTask",
    3000,
    NULL,
    1,
    &DisplayTaskHandle,
    0);
  xTaskCreatePinnedToCore(
    WebTask,
    "WebTask",
    5000,
    NULL,
    1,
    &WebTaskHandle,
    0);
}

void loop()
{
}