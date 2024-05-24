#include <WiFi.h>
#include <WebServer.h>
#include <time.h>

const char* ssid = "iPhone de Roberto";
const char* password = "12345678";

WebServer server(80);

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  server.on("/", handleRoot);
  server.on("/resetTime", handleResetTime);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  time_t now = time(nullptr);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);

  char timeString[25];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);

  String html = "<!DOCTYPE html><html><head><title>ESP32 Time Server</title></head><body>";
  html += "<h1>Tiempo Actual: ";
  html += timeString;
  html += "</h1><button onclick=\"location.href='/resetTime'\">Restablecer Hora</button>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleResetTime() {
  struct tm timeinfo = {0};
  timeinfo.tm_year = 2024 - 1900; // Año menos 1900
  timeinfo.tm_mon = 0;  // Mes 0-11 (0 = enero)
  timeinfo.tm_mday = 1; // Día del mes 1-31
  time_t t = mktime(&timeinfo);
  timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);

  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}
