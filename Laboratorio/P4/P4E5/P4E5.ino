#include "BluetoothSerial.h"


// Objeto para Bluetooth Serial
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);

  // Iniciar Bluetooth con nombre del dispositivo
  if (!SerialBT.begin("ESP32Chat")) {
    Serial.println("Error al iniciar Bluetooth");
    while (1);
  }
  Serial.println("El Dispositivo ha Iniciado");

  // Configurar visibilidad de Bluetooth
  esp_bt_dev_set_device_name("ESP32Chat"); // Nombre del dispositivo
  esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);

  Serial.println("El ESP32 es visible para emparejamiento.");
}

void loop() {
  if (Serial.available()) {
    // Si hay datos desde el puerto serie del monitor
    String data = Serial.readString();
    SerialBT.println(data); // Enviar datos al dispositivo Bluetooth
    Serial.print("Enviado: ");
    Serial.println(data);
  }

  if (SerialBT.available()) {
    // Si hay datos desde el dispositivo Bluetooth
    String data = SerialBT.readString();
    Serial.print("Datos Recibidos: ");
    Serial.println(data);
  }
}
