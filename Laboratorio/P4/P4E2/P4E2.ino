#include <ArduinoBLE.h>

// Cambia el UUID a "Arduino"
BLEService ledService("Arduino"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // configurar el pin del LED en modo de salida
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(LED_BUILTIN, LOW);         // cuando el dispositivo se desconecte, apague el LED
  digitalWrite(LEDR, HIGH);               // apagará el LED
  digitalWrite(LEDG, HIGH);               // apagará el LED
 
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("¡Error al iniciar Bluetooth® Low Energy!!");

    while (1);
  }

  // establecer el nombre local anunciado y el UUID del servicio:
  BLE.setLocalName("Arduino");
  BLE.setAdvertisedService(ledService);

  // agregar la característica al servicio
  ledService.addCharacteristic(switchCharacteristic);

  // agregar servicio
  BLE.addService(ledService);

  // establezca el valor inicial para la característica:
  switchCharacteristic.writeValue(0);

  // inicia advertising
  BLE.advertise();

  Serial.println("Periférico Bluetooth® - LED ");
}

void loop() {
  // listen si se conectan los periféricos Bluetooth® Low Energy:
  BLEDevice central = BLE.central();

  // Si una equipo está conectada a un periférico::
  if (central) {
    Serial.print("Dispositivo Conectado: ");
    // imprimir la dirección MAC :
    Serial.println(central.address());

     // enciende el LED para indicar la conexión
    digitalWrite(LED_BUILTIN, HIGH);            

    // while la central sigue conectada a los periféricos:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        switch (switchCharacteristic.value()) {
          case 1:
            Serial.println("LED Rojo: Encendido");
            digitalWrite(LEDR, LOW);            // encenderá el LED
            digitalWrite(LEDG, HIGH);           // apagará el LED
            break;
          case 2:
            Serial.println("LED Verde: Encendido");
            digitalWrite(LEDR, HIGH);           // apagará el LED
            digitalWrite(LEDG, LOW);            // encenderá el LED
            break;
          default:
            Serial.println(F("LEDS :Apagados"));
            digitalWrite(LEDR, HIGH);           // apagará el LED
            digitalWrite(LEDG, HIGH);           // apagará el LED
            break;
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Dispositivo Desconectado: "));
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, LOW);         // Cuando el dispositivo se desconecta, apagará el LED
    digitalWrite(LEDR, HIGH);          // apagará el LED
    digitalWrite(LEDG, HIGH);        // apagará el LED
  }
}
