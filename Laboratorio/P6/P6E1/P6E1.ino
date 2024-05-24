#include <Arduino.h>

// Define el pin del LED
const int ledPin = 2; // Usualmente el pin 2 en los ESP32 está conectado a un LED integrado

void setup() {
  // Inicializa la comunicación serial
  Serial.begin(115200);
  delay(1000);
  
  // Configura el pin del LED como salida
  pinMode(ledPin, OUTPUT);

  // Crea las tareas
  xTaskCreate(Tarea1, "Tarea1", 10000, NULL, 1, NULL);
  xTaskCreate(Tarea2, "Tarea2", 10000, NULL, 1, NULL);
}

void loop() {
  // El loop principal no hace nada, las tareas se ejecutan independientemente
  delay(1000);
}

void Tarea1(void * parameter) {
  // Tarea 1: Imprimir "Hola Mundo" cada segundo
  while (true) {
    Serial.println("Hola Mundo");
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Espera 1 segundo
    
  }
  
  vTaskDelete(NULL); // Esto nunca se alcanzará, pero es buena práctica incluirlo
}

void Tarea2(void * parameter) {
  // Tarea 2: Parpadear el LED cada 200 ms
  while (true) {
    digitalWrite(ledPin, HIGH); // Enciende el LED
    vTaskDelay(200 / portTICK_PERIOD_MS); // Espera 200 ms
    digitalWrite(ledPin, LOW);  // Apaga el LED
    vTaskDelay(200 / portTICK_PERIOD_MS); // Espera 200 ms
    
  }
  
  vTaskDelete(NULL); // Esto nunca se alcanzará, pero es buena práctica incluirlo
}