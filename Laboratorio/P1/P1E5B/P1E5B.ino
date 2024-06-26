#include <Wire.h>

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // function that executes whenever data is received from writer
}

void loop() {
  delay(100);
}

void receiveEvent(int howMany) {
   char c = Wire.read(); // receive a character
   if(c == '0'){
     digitalWrite(LED_BUILTIN, LOW);   // Apaga el LED 
  Serial.println("Led : Apagado");   
   }
   if(c == '1'){
     digitalWrite(LED_BUILTIN, HIGH);  // Encienda el LED
     Serial.println("Led : Encenido");  
   }
}