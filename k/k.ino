#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  Serial.begin(9600);

  while (!Serial) {}

  Serial.println("Access Point Web Server");

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);

  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {

    Serial.println("Please upgrade the firmware");

  }
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    while (true);
  }
  delay(10000);
  server.begin();
}

void loop() {
  if (status != WiFi.status()) {
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      Serial.println("Device connected to AP");
    } else {
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            break;
          }
          else {
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;

        }
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(led, HIGH);
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(led, LOW);
        }
      }
    }
    client.stop();
  }
}
