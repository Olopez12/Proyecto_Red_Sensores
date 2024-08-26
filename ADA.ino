#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "AdafruitIO_WiFi.h"
#include <HardwareSerial.h>

// Configuración de Wi-Fi y Adafruit IO
#define WIFI_SSID       "ya configurados"
#define WIFI_PASS       "ya configurados"
#define IO_USERNAME    "ya configurados"
#define IO_KEY         "ya configurados"

// Instancia de Adafruit IO
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Feeds de Adafruit IO
AdafruitIO_Feed *tempFeed = io.feed("Temperatura");
AdafruitIO_Feed *humFeed = io.feed("Humedad");
AdafruitIO_Feed *lightFeed = io.feed("Luz");
AdafruitIO_Feed *doorFeed = io.feed("Puerta");

// Instancia de HardwareSerial para UART2
HardwareSerial MySerial(2);
const int MySerialRX = 16;
const int MySerialTX = 17;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.print("Conectando a Adafruit IO");

  // Iniciar la comunicación en el puerto UART2 (RX2 en el pin 25)
  MySerial.begin(9600, SERIAL_8N1, MySerialRX, MySerialTX);

  // Iniciar la conexión con Adafruit IO
  io.connect();

  // Esperar hasta que la conexión con Adafruit IO esté lista
  while(io.status() < AIO_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado a Adafruit IO!");
}

void loop() {
  // Mantener la conexión con Adafruit IO
  io.run();
  delay(1300);

  // Verificar si hay datos disponibles desde el ATmega328p
  if (MySerial.available() > 0) {
    char id = MySerial.read(); // Leer el identificador
    String value = MySerial.readStringUntil('\n'); // Leer el valor

    // Mostrar los datos recibidos en el puerto serial normal
    Serial.print("ID: ");
    Serial.print(id);
    Serial.print(" Valor: ");
    Serial.println(value);

    // Enviar los datos al feed correspondiente basado en el identificador
    if (id == 'T') {
      tempFeed->save(value.toFloat());
    } else if (id == 'H') {
      humFeed->save(value.toFloat());
    } else if (id == 'L') {
      lightFeed->save(value.toInt());
    } else if (id == 'P') {
      doorFeed->save(value.toInt());
    }
  }
}
