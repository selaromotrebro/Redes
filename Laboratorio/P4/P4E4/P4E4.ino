#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

BLEService accelerometerService("19B10000-E8F2-537E-4F6C-D104768A1214");  // Servicio personalizado GATT
BLECharacteristic accelerometerDataCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 6);
BLECharacteristic controlCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLEWrite, 1);

bool accelerometerEnabled = false;

void setup() {
  Serial.begin(9600);

    pinMode(LED_BUILTIN, OUTPUT);

    digitalWrite(LED_BUILTIN, LOW); 

  if (!BLE.begin()) {
    Serial.println("¡¡Error al iniciar Bluetooth® Low Energy!!");
    while (1);
  }

  // Inicializar el acelerómetro
  if (!IMU.begin()) {
    Serial.println("¡Error al inicializar Acelerómetro!");
    while (1);
  }

  // Definir el servicio y las características
  BLE.setLocalName("Arduino Nano 33 BLE");
  BLE.setAdvertisedService(accelerometerService);

  accelerometerService.addCharacteristic(accelerometerDataCharacteristic);
  accelerometerService.addCharacteristic(controlCharacteristic);
  BLE.addService(accelerometerService);

  // Iniciar la publicidad del dispositivo
  BLE.advertise();

  Serial.println("Periférico Bluetooth® - Acelerómetro");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Dispositivo Conectado: ");
    Serial.println(central.address());

    digitalWrite(LED_BUILTIN, HIGH);    
    
    while (central.connected()) {
      // Leer valores del acelerómetro si está habilitado
      if (accelerometerEnabled) {
        float x, y, z;
        IMU.readAcceleration(x, y, z);

        // Crear un arreglo de bytes para almacenar los valores del acelerómetro
        uint8_t data[6];
        memcpy(data, &x, sizeof(float));
        memcpy(data + sizeof(float), &y, sizeof(float));
        memcpy(data + 2 * sizeof(float), &z, sizeof(float));

        // Actualizar la característica con los nuevos valores
        accelerometerDataCharacteristic.writeValue(data, sizeof(data));

        // Mostrar los valores en el monitor serial
        Serial.print("Acelerómetro: X=");
        Serial.print(x);
        Serial.print(" Y=");
        Serial.print(y);
        Serial.print(" Z=");
        Serial.println(z);
      }

      // Verificar si hay datos escritos en la característica de control
      if (controlCharacteristic.written()) {
        uint8_t value = controlCharacteristic.value()[0];
        if (value == 1) {
          Serial.println("Acelerómetro: Activado");
          accelerometerEnabled = true;
        } else if (value == 2) {
          Serial.println("Acelerómetro: Desactivado");
          accelerometerEnabled = false;
        }
      }

      delay(500);
    }

    Serial.print("Dispositivo Desconectado: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, LOW);   
  }
}
