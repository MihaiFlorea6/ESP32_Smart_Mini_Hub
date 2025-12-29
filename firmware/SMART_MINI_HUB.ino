/*-----------------------------------
 Project: ESP32 Smart Mini Hub
 Architecture: Modular C++ with FSM
 Author: Florea Mihai
 ----------------------------------*/


#include <WiFi.h>
#include <WebServer.h>

#include "Credential.h"
#include "WebPage.h"
#include "Hardware.h"

int LIGHT_THRESHOLD = 3000;

WebServer server(80);

enum SystemState { STATE_AUTO, STATE_MANUAL_ON, STATE_MANUAL_OFF };
SystemState currentState = STATE_AUTO;

int lightValue = 0;
bool lastButtonState = HIGH;

void handleData();
void handleCommand();

// Setup
void setup(){
  Serial.begin(115200);

  initHardware();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("\nSe conecteza la WiFi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n Conectat la WiFi!");
  Serial.print(" Acceseaza aceasta adresa in browser: http://");
  Serial.println(WiFi.localIP());

  beep(2500,1000);

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", index_html);
  });

  server.on("/status", HTTP_GET, handleData);
  server.on("/set", HTTP_GET, handleCommand);

  server.begin();
}

// Main Loop
void loop(){
  server.handleClient();

  lightValue = readLDR();

  bool currentBtnState = readButton();
  if(lastButtonState == HIGH && currentBtnState == LOW){
    beep(2500, 500);

    if (currentState == STATE_AUTO) currentState = STATE_MANUAL_ON;
    else if (currentState == STATE_MANUAL_ON) currentState = STATE_MANUAL_OFF;
    else currentState = STATE_AUTO;

    delay(500);
  }
  lastButtonState = currentBtnState;

  switch(currentState) {
    case STATE_MANUAL_ON:
       setLedState(true);
       break;

    case STATE_MANUAL_OFF:
       setLedState(false);
       break;

    case STATE_AUTO:
       if(lightValue < LIGHT_THRESHOLD && lightValue > 0) {
          setLedState(true);
       } else {
          setLedState(false);
       }
       break;

  }
}

void handleData(){
  String stateStr;
  switch(currentState) {
    case STATE_AUTO: stateStr = "AUTOMAT"; break;
    case STATE_MANUAL_ON: stateStr = "MANUAL ON"; break;
    case STATE_MANUAL_OFF: stateStr = "MANUAL OFF"; break;
  }
 
  String json = "{";
  json += "\"ldr\":" + String(lightValue) + ",";
  json += "\"stateStr\":\"" + stateStr + "\",";
  json += "\"stateCode\":" + String(currentState);
  json += "}";

  server.send(200, "application/json", json);
}

void handleCommand(){
  if (server.hasArg("mode")){
    String m = server.arg("mode");

    if (m == "auto") currentState = STATE_AUTO;
    else if (m == "on") currentState = STATE_MANUAL_ON;
    else if (m == "off") currentState = STATE_MANUAL_OFF;

    beep(2500, 500);
  }
  server.send(200, "text/plain", "OK");
}