#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <Wire.h>
#include "WEMOS_Motor.h"
#include <VL6180X.h>

VL6180X sensor;

#define BUZZER 13
#define BUZZFREQ 800
#define BUZZDURATION 300
unsigned long zeitAlt = 0;

/* Put your SSID & Password */
const char* ssid = "PMUTDemoCar";  // Enter SSID here
const char* password = "geheim123";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
ESP8266WebServer server(80);

//Motor shield default I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(0x30, _MOTOR_A, 800); //Motor A drive motor
Motor M2(0x30, _MOTOR_B, 500); //Motor B turning motor

// HTML content
const char* htmlPage = 
#include "webpage.h"
;

bool fMove=LOW;
bool bMove=LOW;
bool rightTurn=LOW;
bool leftTurn=LOW;

int fSpeed=60; // max drive speed
int bSpeed=30; // max reverse speed
int tSpeed=60; // max turn speed

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}
// Functions for each button's press and release actions
void handleForwardPress() {
  //Serial.println("Forward Pressed");
  server.send(200, "text/plain", "Forward Pressed");
  fMove=HIGH;
  bMove=LOW;
  delay(100);
}

void handleForwardRelease() {
  //Serial.println("Forward Released");
  server.send(200, "text/plain", "Forward Released");
  fMove=LOW;
  bMove=LOW;
  delay(100);
}

void handleBackwardPress() {
  //Serial.println("Backward Pressed");
  server.send(200, "text/plain", "Backward Pressed");
  bMove=HIGH;
  fMove=LOW;
  delay(100);
}

void handleBackwardRelease() {
  //Serial.println("Backward Released");
  server.send(200, "text/plain", "Backward Released");
  fMove=LOW;
  bMove=LOW;
  delay(100);
}

void handleLeftPress() {
  //Serial.println("Left Pressed");
  server.send(200, "text/plain", "Left Pressed");
  leftTurn=HIGH;
  rightTurn=LOW;
  delay(100);
}

void handleLeftRelease() {
  //Serial.println("Left Released");
  server.send(200, "text/plain", "Left Released");
  leftTurn=LOW;
  rightTurn=LOW;
  delay(100);
}

void handleRightPress() {
  //Serial.println("Right Pressed");
  server.send(200, "text/plain", "Right Pressed");
  leftTurn=LOW;
  rightTurn=HIGH;
  delay(100);
}

void handleRightRelease() {
  //Serial.println("Right Released");
  server.send(200, "text/plain", "Right Released");
  leftTurn=LOW;
  rightTurn=LOW;
  delay(100);
}

void setup() {

  pinMode(BUZZER, OUTPUT); //buzzer output
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  sensor.init();
  sensor.configureDefault();
  sensor.setTimeout(500);

  Serial.println("Connected to WiFi");

  // Handle root URL and buttons
  server.on("/", handleRoot);
  server.on("/forward-press", handleForwardPress);
  server.on("/forward-release", handleForwardRelease);
  server.on("/backward-press", handleBackwardPress);
  server.on("/backward-release", handleBackwardRelease);
  server.on("/left-press", handleLeftPress);
  server.on("/left-release", handleLeftRelease);
  server.on("/right-press", handleRightPress);
  server.on("/right-release", handleRightRelease);

  server.begin();
  Serial.println("HTTP server started");

  delay(1000);
}

void loop() {

  uint8_t range = sensor.readRangeSingleMillimeters();

  server.handleClient();
  if (fMove){M1.setmotor(_CW, fSpeed); delay(100);}
  else if (bMove){
    if (range>45){M1.setmotor(_CCW, bSpeed); delay(100);}
    else{{M1.setmotor(_STOP); delay(100);}} }
  else {M1.setmotor(_STOP); delay(100);}
  
  if (rightTurn){M2.setmotor(_CW, tSpeed); delay(100);}
  else if (leftTurn){M2.setmotor(_CCW, tSpeed); delay(100);}
  else {M2.setmotor(_STOP); delay(100);}
  
  
  Serial.print(range);
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  if (range < 170){
  if (millis() - zeitAlt >= range * 3) {
      zeitAlt = millis();
      tone(BUZZER, BUZZFREQ, BUZZDURATION);
      }}
  Serial.println();

}