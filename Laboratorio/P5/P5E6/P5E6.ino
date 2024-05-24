#include <WiFi.h>
#include <WiFiUdp.h>
#include "time.h"
#include "esp_random.h"

const char* ssid = "iPhone de Roberto";
const char* password = "12345678";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

const char* serverIP = "172.20.10.10";
const int serverPort = 21;

WiFiUDP udp;

bool sendRandomNumber = false; // Variable para controlar el envío del número aleatorio

void sendRandomNumberToServer(int randomNumber) {
  char buffer[10];
  snprintf(buffer, sizeof(buffer), "%d", randomNumber);
  udp.beginPacket(serverIP, serverPort);
  udp.write((uint8_t*)buffer, strlen(buffer));
  udp.endPacket();
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
      sendRandomNumber = true; // Inicia el envío del número aleatorio
      Serial.println("Envío de número aleatorio iniciado.");
    } else if (command.equals("stop")) {
      sendRandomNumber = false; // Detiene el envío del número aleatorio
      Serial.println("Envío de número aleatorio detenido.");
    }
  }
  
  delay(1000);
  if (sendRandomNumber) {
    int randomNumber = esp_random() % 100; // Genera un número aleatorio entre 0 y 99
    Serial.println("Número aleatorio generado: " + String(randomNumber));
    sendRandomNumberToServer(randomNumber);
  }
}
