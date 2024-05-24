#include <WiFi.h>
#include <ESP32_FTPClient.h>

// WiFi credentials
const char* ssid = "iPhone de Roberto";
const char* password = "12345678";

// FTP Server credentials
char ftp_server[] = "172.20.10.3";
char ftp_user[] = "rsense";
char ftp_pass[] = "rsense";
int ftp_port = 21;

// Initialize FTP Client
ESP32_FTPClient ftp(ftp_server, ftp_user, ftp_pass, ftp_port);

void setup() {
  Serial.begin(115200);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to FTP Server
  Serial.println("Connecting to FTP server");
  ftp.OpenConnection();
  
  // Initialize file transfer type
  ftp.InitFile("Type A");
  
  // List the files in the directory (if such a function exists)
  // If ListFiles does not exist, you may need to implement your own method to list files
  // For example, you could use ftp.ChangeWorkDir() and ftp.GetFile() to navigate and retrieve files
  
  // Close the FTP connection
  ftp.CloseConnection();
}

void loop() {
  // Nothing to do here
}