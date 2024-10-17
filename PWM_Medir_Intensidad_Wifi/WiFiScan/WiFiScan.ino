#include "WiFi.h"

// Configuración del LED
const int ledPin = 4; // Cambia este valor según el pin que uses para el LED
String targetSSID = "Ignacio's Galaxy S21 5G"; // Cambia por el nombre de la red WiFi que deseas buscar

void setup() {
  Serial.begin(115200);

  // Configura el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0); // Asegurarse de que el LED comience apagado

  // Configura el modo WiFi en estación (STA)
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Desconectar de cualquier red anterior
  delay(100);

  Serial.println("Configuración lista, comenzando escaneo.");
}

void loop() {
  Serial.println("Iniciando escaneo de redes WiFi...");

  // Realiza el escaneo de redes WiFi
  int n = WiFi.scanNetworks();
  Serial.println("Escaneo finalizado");

  bool found = false;
  long rssi = -100;  // Valor inicial de RSSI muy bajo, indicando mala señal

  // Busca únicamente la red especificada
  for (int i = 0; i < n; ++i) {
    String ssid = WiFi.SSID(i);

    if (ssid == targetSSID) {
      found = true; // Marca que ha encontrado la red
      rssi = WiFi.RSSI(i);
      int channel = WiFi.channel(i);
      wifi_auth_mode_t encryption = WiFi.encryptionType(i);

      // Imprime la información de la red encontrada
      Serial.println("¡Red WiFi objetivo encontrada!");
      Serial.print("SSID: ");
      Serial.println(ssid);
      Serial.print("RSSI: ");
      Serial.println(rssi);
      Serial.print("Canal: ");
      Serial.println(channel);
      Serial.print("Encriptación: ");
      switch (encryption) {
        case WIFI_AUTH_OPEN:            Serial.println("Abierta"); break;
        case WIFI_AUTH_WEP:             Serial.println("WEP"); break;
        case WIFI_AUTH_WPA_PSK:         Serial.println("WPA"); break;
        case WIFI_AUTH_WPA2_PSK:        Serial.println("WPA2"); break;
        case WIFI_AUTH_WPA_WPA2_PSK:    Serial.println("WPA+WPA2"); break;
        case WIFI_AUTH_WPA2_ENTERPRISE: Serial.println("WPA2-EAP"); break;
        case WIFI_AUTH_WPA3_PSK:        Serial.println("WPA3"); break;
        case WIFI_AUTH_WPA2_WPA3_PSK:   Serial.println("WPA2+WPA3"); break;
        case WIFI_AUTH_WAPI_PSK:        Serial.println("WAPI"); break;
        default:                        Serial.println("Desconocido"); break;
      }
      break; // Sal del bucle una vez que se encuentre la red objetivo
    }
  }

  if (found) {
    // Mapear el valor de RSSI (señal) al rango del brillo del LED (0-255)
    // RSSI típicamente varía entre -100 (muy lejos) a -40 (muy cerca)
    int brightness = map(rssi, -50, -8, 0, 255);
    brightness = constrain(brightness, 0, 255); // Asegura que el valor esté en el rango correcto

    // Ajustar el brillo del LED
    analogWrite(ledPin, brightness);

    Serial.print("Brillo del LED ajustado a: ");
    Serial.println(brightness);
  } else {
    Serial.println("La red WiFi objetivo no fue encontrada.");
    analogWrite(ledPin, 0); // Si no se encuentra la red, apaga el LED
  }

  // Borrar los resultados del escaneo para liberar memoria
  WiFi.scanDelete();

  // Espera 5 segundos antes de volver a escanear
  delay(50);
}