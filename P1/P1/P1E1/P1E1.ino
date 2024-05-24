
void setup() 
{
  Serial.begin(9600);
  int entrada;
}

void loop()
 {
 
  int entrada = analogRead(A0);
  delay(1000);

  float voltage = entrada * (3.3 / 1023);

  Serial.println(voltage);
}