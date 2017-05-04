
#include <FS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>
#include "RestClient.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

WiFiManager wifiManager;
ESP8266WebServer server(80);

int mosfet = LED_BUILTIN;//new int variable named "mosfet", represents the mosfet pin, the value is 0
const String shouldOpen ="SHOULD_OPEN";
const char* fingerprint = "A9 E5 6D E5 9F 55 3E F4 1C F7 1A BC 17 70 24 83 68 26 52 18";
const char* apiHost = "my.milan.maison";
const char* devApiHost = "192.168.10.114";
const int apiPort = 443;
char path[50];

RestClient client = RestClient(apiHost, apiPort, fingerprint);//new RestClient variable named "client"
String response;//new String variable named "response", will hold the server's answer

void setup() {
Serial.begin(115200);
  pinMode(mosfet, OUTPUT);
  digitalWrite(mosfet, LOW);
  delay(1000);  
  
  if (!wifiManager.autoConnect("ConfigDomotics", "12345678")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  sprintf(path, "/api/smart_gates/%08X/status", ESP.getChipId());
  Serial.println(path);
}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        t_httpUpdate_return ret = ESPhttpUpdate.update("http://robertosanto.altervista.org/firmware.bin");
        //t_httpUpdate_return  ret = ESPhttpUpdate.update("https://server/file.bin");

        switch(ret) {
            case HTTP_UPDATE_FAILED:
                Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                break;

            case HTTP_UPDATE_NO_UPDATES:
                Serial.println("HTTP_UPDATE_NO_UPDATES");
                break;

            case HTTP_UPDATE_OK:
                Serial.println("HTTP_UPDATE_OK");
                break;
        }
    }
}

