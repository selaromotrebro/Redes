#include <Arduino_LSM9DS1.h>

unsigned long previousMillis = 0;
const unsigned long interval = 2000;  

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.println("IMU initialized!");
}

void loop() {
  unsigned long currentMillis = micros();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() && IMU.magneticFieldAvailable()) {
      float ax, ay, az;
      float gx, gy, gz;
      float mx, my, mz;

      IMU.readAcceleration(ax, ay, az);
      IMU.readGyroscope(gx, gy, gz);
      IMU.readMagneticField(mx, my, mz);

     // Serial.print("Ace: ");
      Serial.print(ax);
      Serial.print(", ");
      Serial.print(ay);
      Serial.print(", ");
      Serial.print(az);
      Serial.println();
    

      //Serial.print("Gir: ");
      Serial.print(gx);
      Serial.print(", ");
      Serial.print(gy);
      Serial.print(", ");
      Serial.print(gz);
      Serial.println();

      //Serial.print("Mag: ");
     //Serial.print(mx);
      //Serial.print(", ");
     // Serial.print(my);
     //Serial.print(",");
      //Serial.println(mz);
    }
  }
}
