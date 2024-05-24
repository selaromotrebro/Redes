#include <Arduino_LSM9DS1.h>

float x, y, z;
int gradosX = 0;
int gradosY = 0;
int gradosZ = 0;
int giroX = 0;
int giroY = 0;
int giroZ= 0;
int limitemayor = 30;
int limitemenor = -30;
int valx;
int valy;
int valz;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Iniciamos");

  if (!IMU.begin()) {
    Serial.println("¡Error al inicializar IMU!");
    while (1);
  }

  Serial.print("Frecuencia= ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
}

void loop() 
{

// Acelerómetro.

  if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);

  }

  if (x > 0.1) {
    x = 100 * x;
    gradosX = map(x, 0, 97, 0, 90);
    
  }
  if (x < -0.1) {
    x = 100 * x;
    gradosX = map(x, 0, -100, 0, 90);
    
  }
  if (y > 0.1) {
    y = 100 * y;
    gradosY = map(y, 0, 97, 0, 90);
   
  }
  if (y < -0.1) {
    y = 100 * y;
    gradosY = map(y, 0, -100, 0, 90);
   
  }
if (z > 0.1) {
    z = 100 * z;
    gradosZ = map(z, 0, 97, 0, 90);
    
  if (z < -0.1) {
    z = 100 * z;
    gradosZ = map(z, 0, -100, 0, 90);
   
  }
  
  //Serial.print("Acelerómetro: ");
 // Serial.print(" X: ");
  Serial.print(gradosX);
 // Serial.print(" Y: ");
  Serial.print(gradosY);
  //Serial.print(" Z: ");
  Serial.print(gradosZ);
  Serial.println();

  {
 // Giroscopio

  if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(x, y, z);
  }

  if(x < limitemenor)
  {
    
  }
    if(x > limitemayor)
  {
  
  }
  if(y > limitemayor)
  {
  
  }
  if(y < limitemenor)
  {
  
  }
  if(z < limitemenor)
  {
  
  }
    if(z > limitemayor)
  {
   
  }
  
}
//Serial.print("Giroscopio:");
  Serial.print(" X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.print(y);
  Serial.print(" Z: ");
  Serial.print(z);
  Serial.println();

{
// Magnetómetro

if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(x, y, z);
  
  {
  if(x < 0)
  {
    valx = -(x);
  }
  else{
    valx = x;
  }
 if(y < 0)
  {
    valy = -(y);
  }
  else{
    valy = y;
  }

if(z < 0)
  {
    valz = -(z);
  }
  else{
    valz = z;
  }
  }
  
 // Serial.print("Magnetómetro:");
  //Serial.print(" X: ");
  Serial.print(x);
  //Serial.print(" Y: ");
  Serial.print(y);
 // Serial.print(" Z: ");
  Serial.print(z);
  Serial.println();
  delay(200);
}

}
}
}