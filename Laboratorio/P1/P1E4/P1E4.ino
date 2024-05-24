
#include <Arduino.h>

const int pinEntradaADC = A0;  
const int pinSalidaPWM = 9;

String comando = "";
unsigned long tiempoAnterior = 0;
unsigned long intervaloADC = 0;
unsigned long intervaloPWM = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pinEntradaADC, INPUT);
  pinMode(pinSalidaPWM, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    char caracter = Serial.read();
    
    if (caracter == '\n') { 
      procesarComando(comando);
      comando = "";  
    } else {
      comando += caracter;  
    }
  }

  if (intervaloADC > 0 && millis() - tiempoAnterior >= intervaloADC)
   {
    tiempoAnterior = millis();
    enviarLecturaADC();
  }
}

void procesarComando(String comando) {
  if (comando.startsWith("ADC")) {
    if (comando.length() == 3) {
      
      enviarLecturaADC();
    } else if (comando.startsWith("ADC(") && comando.endsWith(")")) {
      
      int intervalo = comando.substring(4, comando.length() - 1).toInt();
      configurarIntervaloADC(intervalo);
    }
  } else if (comando.startsWith("PWM(") && comando.endsWith(")")) {
    
    int dutyCycle = comando.substring(4, comando.length() - 1).toInt();
    configurarPWM(dutyCycle);
  }
}

void enviarLecturaADC() {
  int valorADC = analogRead(pinEntradaADC);
  Serial.print("Valor ADC: ");
  Serial.println(valorADC);
}

void configurarIntervaloADC(int intervalo) {
  intervaloADC = intervalo * 1000;
  if (intervaloADC > 0) {
    tiempoAnterior = millis();
  }
}

void configurarPWM(int dutyCycle) {
  int valorPWM = map(dutyCycle, 0, 9, 0, 255);
  analogWrite(pinSalidaPWM, valorPWM);
}