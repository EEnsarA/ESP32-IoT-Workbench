#include <WiFi.h>
#include <WiFiClient.h>
#include <queue>
#include "secrets.h"

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

#define WORKER_COUNT 8
#define JOB_QUEUE_DEPTH 512
#define RESULT_QUEUE_DEPTH 128

const uint16_t discoveryPorts[] = {80, 443, 8080, 1883, 554, 5353, 22, 53};
const size_t discoveryPortsCount = sizeof(discoveryPorts) / sizeof(discoveryPorts[0]);

const uint16_t fullPorts[] = {
    22, 80, 443, 8080, 8888, 8000,
    53, 21, 23, 110, 445, 1883,
    554, 3306, 5432, 5900, 3389};
const size_t fullPortsCount = sizeof(fullPorts) / sizeof(fullPorts[0]);

typedef struct
{
  uint32_t ip;
  uint16_t port;
} Job_t;

typedef struct
{
  uint32_t ip;
  uint16_t port;
} Result_t;

QueueHandle_t jobQueue = nullptr;
QueueHandle_t resultQueue = nullptr;

static inline uint32_t ipToRaw(const IPAddress &ip)
{
  // IPAddress stores bytes in natural order; convert to uint32_t
  return ((uint32_t)ip[0] << 24) | ((uint32_t)ip[1] << 16) | ((uint32_t)ip[2] << 8) | ((uint32_t)ip[3]);
}
static inline IPAddress rawToIP(uint32_t raw)
{
  return IPAddress((raw >> 24) & 0xFF, (raw >> 16) & 0xFF, (raw >> 8) & 0xFF, raw & 0xFF);
}

void portScannerWorker(void *param)
{
  (void)param;
  Job_t job;
  for (;;)
  {
    if (xQueueReceive(jobQueue, &job, portMAX_DELAY) == pdTRUE)
    {
      IPAddress ip = rawToIP(job.ip);
      uint16_t port = job.port;

      WiFiClient client;
      client.setTimeout(60);

      if (client.connect(ip, port))
      {

        Result_t r;
        r.ip = job.ip;
        r.port = port;
        xQueueSend(resultQueue, &r, 0);
        client.stop();
      }

      vTaskDelay(pdMS_TO_TICKS(1));
    }
  }
}

bool enqueueDiscoveryAndCheck(IPAddress ip, uint32_t timeout_ms = 250)
{
  uint32_t raw = ipToRaw(ip);
  Job_t j;

  for (size_t i = 0; i < discoveryPortsCount; ++i)
  {
    j.ip = raw;
    j.port = discoveryPorts[i];
    xQueueSend(jobQueue, &j, 0);
  }

  uint32_t deadline = millis() + timeout_ms;
  Result_t res;
  bool found = false;
  while (millis() < deadline)
  {
    if (xQueueReceive(resultQueue, &res, pdMS_TO_TICKS(15)) == pdTRUE)
    {
      if (res.ip == raw)
      {
        Serial.printf("[KEŞİF] Cihaz %s -- açık port %d (discovery)\n", ip.toString().c_str(), res.port);
        found = true;
      }
      else
      {

        Serial.printf("[SONUÇ-BAŞKA] %s:%d\n", rawToIP(res.ip).toString().c_str(), res.port);
      }
    }
    else
    {

      vTaskDelay(pdMS_TO_TICKS(2));
    }
  }
  return found;
}

void enqueueFullScanForIP(IPAddress ip)
{
  Job_t j;
  uint32_t raw = ipToRaw(ip);
  for (size_t i = 0; i < fullPortsCount; ++i)
  {
    j.ip = raw;
    j.port = fullPorts[i];
    xQueueSend(jobQueue, &j, 0);
  }
}

void scanNetwork()
{
  Serial.println("IP taraması başlatılıyor...");
  IPAddress local = WiFi.localIP();
  IPAddress testIP(local[0], local[1], local[2], 0);

  for (int i = 1; i < 255; ++i)
  {
    testIP[3] = i;
    if (testIP == local)
      continue;

    Serial.printf("\n• Kontrol edilen IP: %s\n", testIP.toString().c_str());

    bool discovered = enqueueDiscoveryAndCheck(testIP, 220);
    if (discovered)
    {

      enqueueFullScanForIP(testIP);
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  Serial.println("WiFi bağlanıyor...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(2, HIGH);
    delay(300);
    digitalWrite(2, LOW);
  }
  Serial.println("\nWiFi bağlı: " + WiFi.localIP().toString());

  jobQueue = xQueueCreate(JOB_QUEUE_DEPTH, sizeof(Job_t));
  resultQueue = xQueueCreate(RESULT_QUEUE_DEPTH, sizeof(Result_t));
  if (!jobQueue || !resultQueue)
  {
    Serial.println("Kuyruk olusturma hatasi!");
    while (1)
      delay(1000);
  }

  for (int i = 0; i < WORKER_COUNT; ++i)
  {
    char name[12];
    snprintf(name, sizeof(name), "worker%d", i);
    xTaskCreatePinnedToCore(
        portScannerWorker,
        name,
        4096,
        NULL,
        1,
        NULL,
        1);
  }

  scanNetwork();

  Serial.println("\nTüm discovery işleri kuyruğa verildi. (Full tarama discovery sonuçlarına göre eklenecek.)");
}

void loop()
{

  Result_t r;
  if (xQueueReceive(resultQueue, &r, pdMS_TO_TICKS(50)) == pdTRUE)
  {
    Serial.printf("[AÇIK] %s:%d\n", rawToIP(r.ip).toString().c_str(), r.port);
  }
}
