#include <ArduinoBLE.h>

BLEService customService("19B10000-E8F2-537E-4F6C-D104768A1214"); // UUID del servicio personalizado
BLEStringCharacteristic nameCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 20); // UUID de la característica para el nombre

void setup() {
  Serial.begin(9600);

  Serial.println("UUID- Bluetooth");

  // Inicializar el módulo BLE
  if (!BLE.begin()) {
    Serial.println("Error iniciando Bluetooth");
    while (1);
  }

  // Configurar el servicio
  BLE.setAdvertisedService(customService);

  // Configurar la característica con el nombre
  nameCharacteristic.setValue("Roberto");

  // Añadir la característica al servicio
  customService.addCharacteristic(nameCharacteristic);

  // Anunciar el servicio
  BLE.addService(customService);

  // Cambiar el nombre del dispositivo
  BLE.setDeviceName("Roberto Nano BLE");

  // Iniciar el advertising
  BLE.advertise();

  Serial.println("Esperando Conexiones Bluetooth");
}

void loop() {

  // Esperar a que se conecte un dispositivo BLE
  BLEDevice central = BLE.central();
  if (central) {

    Serial.print("Dispositivo Conectado: ");
    Serial.println(central.address());

    // Esperar a que se desconecte el dispositivo
    while (central.connected()) {
      delay(1000);
    }

    Serial.print("Dispositivo Desconectado: ");
    Serial.println(central.address());
  }
}
