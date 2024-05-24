const int pinEntradaADC = A0; 
const int pinSalidaPWM = 9;   

void setup() {

  pinMode(pinEntradaADC, INPUT);
  pinMode(pinSalidaPWM, OUTPUT);

  Serial.begin(9600); 
}

void loop() {

  int valorADC = analogRead(pinEntradaADC);


  int valorPWM = map(valorADC, 0, 1023, 0, 255);

  analogWrite(pinSalidaPWM, valorPWM);

  Serial.print("Valor ADC: ");
  Serial.print(valorADC);
  Serial.print("   Valor PWM: ");
  Serial.println(valorPWM);

  delay(100); 
}
