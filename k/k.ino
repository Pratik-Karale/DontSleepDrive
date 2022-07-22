#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
  #include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>


const int buzzerPin=0;
const int motorStopPin=2;

const char* ssid     = "takeMeToHeaven";
const char* password = "123456789";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


void setup(){
  pinMode(buzzerPin,OUTPUT);
  pinMode(motorStopPin,OUTPUT);
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP()); 

  // Route for root / web page
  server.on("/buzzer/start", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(buzzerPin,HIGH);
    request->send_P(200, "text/html","BUZZER STARTED");
  });
  server.on("/motor/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(motorStopPin,HIGH);
    request->send_P(200, "text/html","motor stoppped");
  });
  server.on("/motor/start", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(motorStopPin,LOW);
    request->send_P(200, "text/html","motor Started");
  });
  server.on("/buzzer/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(buzzerPin,LOW);
    (200, "text/plain","stopped buzzer");
  });

  // Start server
  server.begin();
}
 
void loop(){  

}
