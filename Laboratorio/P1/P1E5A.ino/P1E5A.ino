#include <Wire.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Wire.begin(); // Inicia el I2C
}

void loop() {
  Serial.print("Ingrese el Valor: ");
  char ledVal[0];
  readSerial(ledVal);
  Serial.println(ledVal);
  Wire.beginTransmission(8); // transmitir al dispositivo#8
  Wire.write(ledVal);        // envía el valor dado
  Wire.endTransmission();    // detiene transmitir
  delay(500);
}

/* Read input serial */
int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (inChar == '\n') {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}