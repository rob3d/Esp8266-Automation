#include <FS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>
#include "RestClient.h"

WiFiManager wifiManager;
ESP8266WebServer server(80);

int mosfet = LED_BUILTIN;
const String shouldOpen ="SHOULD_OPEN";
const char* fingerprint = "A9 E5 6D E5 9F 55 3E F4 1C F7 1A BC 17 70 24 83 68 26 52 18";
const char* apiHost = "my.milan.maison";
const char* devApiHost = "192.168.10.114";
const int apiPort = 443;
char path[50];

// Testing git client
RestClient client = RestClient(apiHost, apiPort, fingerprint);
//RestClient client = RestClient(devApiHost, 3000);
 
String response;

void setup(void) { 
  Serial.begin(115200);
  pinMode(mosfet, OUTPUT);
  digitalWrite(mosfet, LOW);
  delay(1000);  

  wifiManager.setConfigPortalTimeout(180);

  
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

void loop(void){
  Serial.println(path);
  client.setHeader("X-Secret-Token: ffab1234abab");
  response = "";
  int statusCode = client.get(path, &response);
  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  Serial.print("Response body from server: ");
  Serial.println(response);
  if(response.substring(25,36)==shouldOpen){
         Serial.println("APRO SUBBBBITO");
         digitalWrite(mosfet,HIGH);
         delay(5000);
         digitalWrite(mosfet,LOW);
    }
  delay(5000);
}

