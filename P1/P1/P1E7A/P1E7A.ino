#include <Wire.h>
#include <Arduino_LSM9DS1.h>

float x_acc, y_acc, z_acc; // Acelerómetro
float x_gyro, y_gyro, z_gyro; // Giroscopio
float x_mag, y_mag, z_mag; // Magnetómetro

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  if (!IMU.begin()) {
    Serial.println("¡Error al inicializar IMU!");
    while (1);
  }
}

void loop() {
  // Acelerómetro
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x_acc, y_acc, z_acc);
    Serial.print("Acelerometro: ");
  
    Serial.print(x_acc);
    Serial.print(", ");
    Serial.print(y_acc);
    Serial.print(",");
    Serial.println(z_acc);
  }

  // Giroscopio
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x_gyro, y_gyro, z_gyro);
    Serial.print("Giroscopio: ");
    Serial.print(x_gyro);
    Serial.print(",");
    Serial.print(y_gyro);
    Serial.print(",");
    Serial.println(z_gyro);
  }

  // Magnetómetro
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(x_mag, y_mag, z_mag);
    Serial.print("Magnetometro: ");
    
    Serial.print(x_mag);
    Serial.print(",");
    Serial.print(y_mag);
    Serial.print(",");
    Serial.println(z_mag);
  }

  // Envío de datos por I2C
  Wire.beginTransmission(9); // Dirección del dispositivo receptor I2C

  // Acelerómetro
  Wire.write((byte*)&x_acc, sizeof(float));
  Wire.write((byte*)&y_acc, sizeof(float));
  Wire.write((byte*)&z_acc, sizeof(float));

  // Giroscopio
  Wire.write((byte*)&x_gyro, sizeof(float));
  Wire.write((byte*)&y_gyro, sizeof(float));
  Wire.write((byte*)&z_gyro, sizeof(float));

  // Magnetómetro
  Wire.write((byte*)&x_mag, sizeof(float));
  Wire.write((byte*)&y_mag, sizeof(float));
  Wire.write((byte*)&z_mag, sizeof(float));

  Wire.endTransmission();

  delay(1000); // Espera antes de la próxima transmisión
}
