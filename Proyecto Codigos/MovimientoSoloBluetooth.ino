#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

enum Estado {
  DETENIDO,
  SUBIENDO,
  ARRIBA,
  BAJANDO
};

class MaquinaEstado {
  public:
    MaquinaEstado() : estado_actual(DETENIDO) {}

    void actualizar_estado(float acelerometro, float giroscopio) {
      if (acelerometro > 0.2) { // Umbral reducido para mayor sensibilidad
        estado_actual = SUBIENDO;
      } else if (acelerometro < -0.2) { // Umbral reducido para mayor sensibilidad
        estado_actual = BAJANDO;
      } else if (giroscopio > 0.2) { // Umbral reducido para mayor sensibilidad
        estado_actual = ARRIBA;
      } else {
        estado_actual = DETENIDO;
      }
      estados_historicos[historico_index] = estado_actual;
      historico_index = (historico_index + 1) % MAX_HISTORICO;
    }

    Estado obtener_estado_actual() {
      return estado_actual;
    }

    const char* estado_a_texto(Estado Estado) {
      switch (estado) {
        case DETENIDO: return "DETENIDO";
        case SUBIENDO: return "SUBIENDO";
        case ARRIBA: return "ARRIBA";
        case BAJANDO: return "BAJANDO";
        default: return "DESCONOCIDO";
      }
    }

  private:
    Estado estado_actual;
    static const int MAX_HISTORICO = 100;
    Estado estados_historicos[MAX_HISTORICO];
    int historico_index = 0;
};

MaquinaEstado maquina_estado;

// Definir el servicio y la característica BLE
BLEService estadoService("180A"); // UUID del servicio
BLEStringCharacteristic estadoCharacteristic("2A57", BLERead | BLENotify, 20); // UUID de la característica, con capacidad de notificación

void setup() {
  Serial.begin(9600);
  // while (!Serial); // Comentar o eliminar esta línea

  if (!IMU.begin()) {
    Serial.println("¡Error al inicializar IMU!"");
    while (1);
  }

  if (!BLE.begin()) {
    Serial.println("¡Error al iniciar BLE!");
    while (1);
  }

  // Configurar el dispositivo BLE
  BLE.setLocalName("Nano33BLE");
  BLE.setAdvertisedService(estadoService);
  estadoService.addCharacteristic(estadoCharacteristic);
  BLE.addService(estadoService);

  // Iniciar la publicidad BLE
  BLE.advertise();

  Serial.println("El dispositivo BLE, Esperando Conexiones.");
}

void loop() {
  // Mantener la conexión BLE
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Conectado Dispositivo: ");
    Serial.println(central.address());

    while (central.connected()) {
      float x, y, z;
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);
        maquina_estado.actualizar_estado(y, 0);  // Usamos el eje Y del acelerómetro
      }

      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(x, y, z);
        maquina_estado.actualizar_estado(0, y); // Usamos el eje Y del giroscopio
      }

      const char* estadoTexto = maquina_estado.estado_a_texto(maquina_estado.obtener_estado_actual());
      estadoCharacteristic.writeValue(estadoTexto);

      Serial.print("Estado Actual: ");
      Serial.println(estadoTexto);

      delay(500); // Espera 0.5 segundos antes de la siguiente lectura para mayor sensibilidad
    }

    Serial.print("Desconectado Dispositivo: ");
    Serial.println(central.address());
  }
}