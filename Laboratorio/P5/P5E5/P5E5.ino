#include <WiFi.h>
#include <WiFiUdp.h>
#include "time.h"

const char* ssid = "iPhone de Roberto";
const char* password = "12345678";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

const char* serverIP = "10.211.55.3";
const int serverPort = 21;

WiFiUDP udp;

bool sendTime = false; // Variable para controlar el envío de la hora

void sendTimeToServer(time_t timeinfo) {
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%ld", timeinfo);
  udp.beginPacket(serverIP, serverPort);
  udp.write((uint8_t*)buffer, strlen(buffer));
  udp.endPacket();
}

void printLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  char formattedTime[50]; // Ajustamos el tamaño del buffer para contener toda la información
  strftime(formattedTime, sizeof(formattedTime), "%A, %B %d %Y %H:%M", &timeinfo); // Modificamos la cadena de formato
  Serial.println(formattedTime);

  // Envía la hora local al servidor
  sendTimeToServer(timeinfo.tm_sec);
}


void setup() {
  Serial.begin(115200);
  
  // Conectar a WiFi
  Serial.printf("Conectando a %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONECTADO");
  
  // Inicializar y obtener la hora
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Elimina espacios en blanco al principio y al final
    
    if (command.equals("start")) {
      sendTime = true; // Inicia el envío de la hora
      Serial.println("Envío de hora iniciado.");
    } else if (command.equals("stop")) {
      sendTime = false; // Detiene el envío de la hora
      Serial.println("Envío de hora detenido.");
    }
  }
  
  delay(1000);
  if (sendTime) {
    printLocalTime();
  }
}
