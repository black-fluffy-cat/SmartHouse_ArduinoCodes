#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

int D0_INPUTPIN = 16;
int D1_OUTPUTLED = 5;

const char* networkSSID = "TP-LINK_E6565C"; // WiFi name
const char* networkPwd = ""; // WiFi password

String deviceName = "NodeMCU + IR sensor";

String serverUrl = "http://192.168.0.108:8080";
String alertEndpoint = serverUrl + "/alert";
String heartbeatEndpoint = serverUrl+ "/heartbeat";

unsigned long lastAlertTime = 0;
unsigned long lastHeartbeatTime = 0;

unsigned long startTime = 0;
unsigned long timerDelayMs = 1000;

unsigned int alertSendIntervalMs = 5000;
unsigned int heartbeatSendIntervalMs = 30000;

void setup() {
  Serial.begin(115200);
  startTime = millis();
  
  pinMode(D0_INPUTPIN, INPUT);
  pinMode(D1_OUTPUTLED, OUTPUT);
  
  WiFi.begin(networkSSID, networkPwd);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected, my IP address is: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  bool movementDetected = !readIRSensor();
  if (movementDetected) {
    lightLedOn();
    if (shouldSendAlert()) {
      lastAlertTime = millis();
      sendAlert();
    }
  } else {
    lightLedOff();
  }

  if (shouldSendHeartbeat()) {
    lastHeartbeatTime = millis();
    sendHeartbeat();
  }
}

void lightLedOn() {
    digitalWrite(D1_OUTPUTLED, HIGH);
}

void lightLedOff() {
    digitalWrite(D1_OUTPUTLED, LOW);  
}

bool readIRSensor() {
  return digitalRead(D0_INPUTPIN);
}

bool shouldSendAlert() {
  return (millis() - lastAlertTime) > alertSendIntervalMs;
}

bool shouldSendHeartbeat() {
  return (millis() - lastHeartbeatTime) > heartbeatSendIntervalMs;
}

void sendAlert() {
  sendRequest(alertEndpoint, getAlertJson());  
}

void sendHeartbeat() {
  sendRequest(heartbeatEndpoint, getHeartbeatJson());  
}

void sendRequest(String url, String json) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      
      http.begin(url);
      
      http.addHeader("Content-Type", "application/json");
      Serial.print("Sending JSON: ");
      Serial.println(json);
      int httpResponseCode = http.POST(json);

      Serial.print("Response code: ");
      Serial.println(httpResponseCode);
      http.end();
    } else {
      Serial.println("Disconnected");
    }
}

String getAlertJson() {
  unsigned long timeFromStart = millis() - startTime;
  String firstPart = "{\"deviceName\":\"";
  String secondPart = "\",\"timeFromStart\":\"";
  String timeFromStartString = String(timeFromStart);
  String thirdPart = "\",\"alertState\":\"true\"}";
  return firstPart + deviceName + secondPart + timeFromStartString + thirdPart;
}

String getHeartbeatJson() {
  unsigned long currentTime = millis();
  unsigned long timeFromStart = currentTime - startTime;
  unsigned long timeFromAlert = currentTime - lastAlertTime;
  String firstPart = "{\"deviceName\":\"";
  String secondPart = "\",\"timeFromStart\":\"";
  String timeFromStartString = String(timeFromStart);
  String thirdPart = "\",\"timeFromAlert\":\"";
  String timeFromAlertString = String(timeFromAlert);
  String fourthPart = "\"}";
  return firstPart + deviceName + secondPart + timeFromStartString + thirdPart + timeFromAlertString + fourthPart;
}
