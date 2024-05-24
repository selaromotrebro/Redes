#include <WiFi.h>
#include <ArduinoJson.h>
#include <ESP32_FTPClient.h>
#include <time.h>

// Configura tu red Wi-Fi
const char* ssid = "iPhone de Roberto";
const char* password = "12345678";

// Configura el servidor FTP
const char* ftp_server = "172.20.10.3";
const char* ftp_user = "rsense";
const char* ftp_pass = "rsense";
const int ftp_port = 21;

// Función para conectarse a Wi-Fi
void connectToWiFi() {
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

// Función para obtener la hora actual
time_t getTime() {
  time_t now;
  struct tm timeinfo;
  
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No se pudo obtener la hora");
    return(0);
  }
  time(&now);
  return now;
}

// Función para generar datos de temperatura inventados
float getFakeTemperature() {
  return random(200, 350) / 10.0; // Genera una temperatura entre 20.0 y 35.0
}

void setup() {
  Serial.begin(115200);
  
  // Conectar a Wi-Fi
  connectToWiFi();
  
  // Configurar la hora
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
}

void loop() {
  // Crear un documento JSON de 200 bytes
  StaticJsonDocument<200> doc;
  
  // Configurar los datos de SenML
  doc["bn"] = "urn:dev:mac:0024befffe804ff1"; // Base Name: Identificador único del dispositivo
  doc["bt"] = getTime();                      // Base Time: Marca temporal base
  
  // Añadir un conjunto de datos (elemento)
  JsonObject rec = doc.createNestedObject("e");
  rec["n"] = "temperatura";                   // Nombre del dato
  rec["u"] = "Grados";                        // Unidad del dato
  rec["v"] = getFakeTemperature();            // Valor del dato
  
  // Serializar el JSON
  char buffer[200];
  size_t n = serializeJson(doc, buffer);
  
  // Imprimir el JSON
  Serial.println(buffer);
  
  // Conectar al servidor FTP y subir el archivo
  ESP32_FTPClient ftp(const_cast<char*>(ftp_server), const_cast<char*>(ftp_user), const_cast<char*>(ftp_pass), ftp_port);
  ftp.OpenConnection();
  
  // Crear la carpeta "895340" en el servidor FTP
  ftp.InitFile("TYPE A");
  ftp.ChangeWorkDir("/rsense/895340");
  ftp.NewFile("data.json");
  ftp.WriteData(reinterpret_cast<unsigned char*>(buffer), n);
  ftp.CloseFile();
  
  // Cerrar la conexión FTP
  ftp.CloseConnection();
  
  // Esperar 10 segundos
  delay(10000);
}