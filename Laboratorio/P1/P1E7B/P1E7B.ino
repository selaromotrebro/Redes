#include <Wire.h>

const int ledPin = 13; // Pin del LED

float x_acc, y_acc, z_acc; // Acelerómetro
float x_gyro, y_gyro, z_gyro; // Giroscopio
float x_mag, y_mag, z_mag; // Magnetómetro

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
  Wire.begin(9); // Establece este dispositivo como esclavo I2C con dirección 9
  Wire.onReceive(receiveEvent); // Configura la función para manejar eventos de recepción I2C
}

void loop() {

}

void receiveEvent(int bytes) {
  if (bytes >= 9 * sizeof(float)) { // Asegura recibir suficientes bytes para los datos de los sensores
    digitalWrite(ledPin, HIGH); // Enciende el LED

    // Acelerómetro
    Wire.readBytes((byte*)&x_acc, sizeof(float));
    Wire.readBytes((byte*)&y_acc, sizeof(float));
    Wire.readBytes((byte*)&z_acc, sizeof(float));

    // Giroscopio
    Wire.readBytes((byte*)&x_gyro, sizeof(float));
    Wire.readBytes((byte*)&y_gyro, sizeof(float));
    Wire.readBytes((byte*)&z_gyro, sizeof(float));

    // Magnetómetro
    Wire.readBytes((byte*)&x_mag, sizeof(float));
    Wire.readBytes((byte*)&y_mag, sizeof(float));
    Wire.readBytes((byte*)&z_mag, sizeof(float));

    Serial.print("Acelerómetro: ");
    Serial.print(x_acc);
    Serial.print(",");
    Serial.print(y_acc);
    Serial.print(",");
    Serial.println(z_acc);

    Serial.print("Giroscopio: ");
    Serial.print(x_gyro);
    Serial.print(",");
    Serial.print(y_gyro);
    Serial.print(",");
    Serial.println(z_gyro);

    Serial.print("Magnetómetro: ");
    Serial.print(x_mag);
    Serial.print(",");
    Serial.print(y_mag);
    Serial.print(",");
    Serial.println(z_mag);

    delay(1000);               
    digitalWrite(ledPin, LOW); 
  }
}
