#include <Arduino.h>

// Define el pin del LED
const int ledPin = 2; // Usualmente el pin 2 en los ESP32 está conectado a un LED integrado

// Variable global para almacenar el número aleatorio
volatile int randomNumber = 0;

// Manejador de la tarea 2
TaskHandle_t Task2Handle = NULL;

void setup() {
  // Inicializa la comunicación serial
  Serial.begin(115200);
  delay(1000);
  
  // Configura el pin del LED como salida
  pinMode(ledPin, OUTPUT);

  // Crea las tareas
  xTaskCreate(Tarea1, "Tarea1", 10000, NULL, 1, NULL);
  xTaskCreate(Tarea2, "Tarea2", 10000, NULL, 1, &Task2Handle);
}

void loop() {
  // El loop principal no hace nada, las tareas se ejecutan independientemente
  delay(1000);
}

void Tarea1(void * parameter) {
  // Tarea 1: Generar números aleatorios y enviarlos vía UART cada segundo
  while (true) {
    randomNumber = random(0, 100); // Genera un número aleatorio entre 0 y 99
    Serial.println(randomNumber);  // Envía el número por UART
    
    // Notifica a la tarea 2 que se ha enviado un número
    xTaskNotifyGive(Task2Handle);
    
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Espera 1 segundo
  }
  vTaskDelete(NULL); 
}

void Tarea2(void * parameter) {
  // Tarea 2: Encender el LED durante 200 ms cuando se envíe un número
  while (true) {
    // Espera una notificación de la tarea 1
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    
    digitalWrite(ledPin, HIGH); // Enciende el LED
    vTaskDelay(200 / portTICK_PERIOD_MS); // Espera 200 ms
    digitalWrite(ledPin, LOW);  // Apaga el LED
  }
  vTaskDelete(NULL);
}
