#include <WiFi.h>
#include <ArduinoJson.h>
#include <ESP32_FTPClient.h>
#include <time.h>

// Configura tu red Wi-Fi
const char* ssid = "iPhone de Roberto";
const char* password = "12345678";

// Configura el servidor FTP
char ftp_server[] = "155.210.150.77";
char ftp_user[] = "rsense";
char ftp_pass[] = "rsense";
int ftp_port = 21;  // Puerto FTP

// Crear una instancia de FTPClient
ESP32_FTPClient ftp(ftp_server, ftp_user, ftp_pass, ftp_port);

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

// Función para obtener el tiempo en formato ddmmss
String getFormattedTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No se pudo obtener la hora");
    return "000000";
  }
  char buffer[7];
  strftime(buffer, sizeof(buffer), "%d%m%S", &timeinfo);
  return String(buffer);
}

// Función para reconectar al servidor FTP si se pierde la conexión
void reconnectFTP() {
  Serial.println("Intentando reconectar al servidor FTP...");
  ftp.OpenConnection();
}

void setup() {
  Serial.begin(115200);
  
  // Conectar a Wi-Fi
  connectToWiFi();
  
  // Configurar la hora
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  
  // Conectar al servidor FTP
  ftp.OpenConnection();
  
  // Crear la carpeta 895340
  ftp.ChangeWorkDir("/");
  ftp.InitFile("TYPE I");
  ftp.MakeDir("/rsense/895340");
}

void loop() {
  // Verificar si la conexión con el servidor FTP está activa
  if (!ftp.isConnected()) {
    reconnectFTP(); // Intentar reconectar si se pierde la conexión
  }
  
  // Crear un documento JSON de 200 bytes
  StaticJsonDocument<200> doc;
  
  // Configurar los datos de SenML
  doc["bn"] = "urn:dev:mac:0024befffe804ff1"; // Base Name: Identificador único del dispositivo
  doc["bt"] = getTime();                      // Base Time: Marca temporal base
  
  // Añadir un conjunto de datos (elemento)
  JsonObject rec = doc.createNestedObject("e");
  rec["n"] = "temperature";                   // Nombre del dato
  rec["u"] = "degC";                          // Unidad del dato
  rec["v"] = getFakeTemperature();            // Valor del dato
  
  // Serializar el JSON
  char buffer[200];
  size_t n = serializeJson(doc, buffer);
  
  // Imprimir el JSON
  Serial.println(buffer);
  
  // Preparar el nombre del archivo
  String filename = "grupoXX_" + getFormattedTime() + ".json";
  
  // Convertir String a char* para la función InitFile
  char char_filename[30];
  filename.toCharArray(char_filename, 30);
  
  // Subir el archivo al servidor FTP
  ftp.ChangeWorkDir("/rsense/895340");
  ftp.InitFile(char_filename);
  ftp.WriteData((unsigned char*)buffer, n);
  ftp.CloseFile();
  
  // Esperar 10 segundos
  delay(10000);
}
