#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;

const char* ssid = "my_ssid";
const char* password = "123456789";

unsigned long startMillis;
unsigned long currentMillis;

int eyeblinkPin=2;

int prevBlinkState=0;
int currentBlinkState=0;

const int buzzerPeriod=5000;
const int motorStopPeriod=10000;

int buzzerReqSent=0;
int motorStopReqSent=0;

void setup()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi");
  Serial.begin(115200);
  
  startMillis = millis();
  pinMode(eyeblinkPin, INPUT);
}


void request(String requestRoute){
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, "http://192.168.4.1"+requestRoute);
  
    int httpResponseCode = http.GET();
    http.end();
  }else{
    Serial.println("Wifi not connected!");
  }
}


void loop()
{
  currentMillis = millis();
  currentBlinkState=digitalRead(eyeblinkPin);
  if(currentBlinkState && prevBlinkState && currentMillis-startMillis>buzzerPeriod){
    if(!buzzerReqSent){
    //    send post req to arduino to start buzzer
      request("/buzzer/start");
      buzzerReqSent=1;
    }
    if(currentMillis-startMillis>motorStopPeriod){
      if(!motorStopReqSent){
        //  send arduino HIGH req to stop motor
        request("/motor/stop");
        motorStopReqSent=1;
      }
    }
  }else if(buzzerReqSent){
    //    send post req to arduino to stop buzzer  
    request("/buzzer/stop");
    buzzerReqSent=0; 
  }else if(motorStopReqSent){
    motorStopReqSent=0;
  }

  if(!prevBlinkState){
    startMillis=currentMillis;
  }

  if(currentBlinkState && !prevBlinkState){
    prevBlinkState=1;
  }else if(!currentBlinkState && prevBlinkState){
    prevBlinkState=0;
  }
  
}
