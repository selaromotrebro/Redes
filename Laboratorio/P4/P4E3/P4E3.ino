#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

BLEService accelerometerService("19B10000-E8F2-537E-4F6C-D104768A1214");  // Servicio personalizado GATT
BLECharacteristic accelerometerCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 6);

void setup() {
  Serial.begin(9600);

  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }

  // Inicializar el acelerómetro
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // Definir el servicio y la característica
  BLE.setLocalName("Arduino Nano 33 BLE");
  BLE.setAdvertisedService(accelerometerService);

  accelerometerService.addCharacteristic(accelerometerCharacteristic);
  BLE.addService(accelerometerService);

  // Iniciar la publicidad del dispositivo
  BLE.advertise();

  Serial.println("BLE Peripheral - Acelerómetro");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Conectado a: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Leer valores del acelerómetro
      float x, y, z;
      IMU.readAcceleration(x, y, z);

      // Crear un arreglo de bytes para almacenar los valores del acelerómetro
      uint8_t data[6];
      memcpy(data, &x, sizeof(float));
      memcpy(data + sizeof(float), &y, sizeof(float));
      memcpy(data + 2 * sizeof(float), &z, sizeof(float));

      // Actualizar la característica con los nuevos valores
      accelerometerCharacteristic.writeValue(data, sizeof(data));

      delay(100);
    }

    Serial.print("Desconectado de: ");
    Serial.println(central.address());
  }
}
