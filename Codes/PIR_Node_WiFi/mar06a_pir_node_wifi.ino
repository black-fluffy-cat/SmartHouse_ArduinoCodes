#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define PIR_INPUT_PIN D5
#define LED_INPUT_PIN D7
#define LED_OUTPUT_PIN D8

const char* networkSSID = ""; // WiFi name
const char* networkPwd = ""; // WiFi password

String deviceName = "NodeMCU_+_PIR";

String serverUrl = "http://192.168.1.10:8080";
String alertEndpoint = serverUrl + "/pirAlert";
String heartbeatEndpoint = serverUrl+ "/heartbeat";

unsigned long lastHeartbeatTime = 0;
unsigned int heartbeatSendIntervalMs = 30000;
unsigned long startTime = 0;

bool movementAlreadyDetected = false;

void setup() {
  Serial.begin(115200);
  pinMode(PIR_INPUT_PIN, INPUT);
  pinMode(LED_INPUT_PIN, OUTPUT);
  pinMode(LED_OUTPUT_PIN, OUTPUT);
    
  WiFi.begin(networkSSID, networkPwd);
  Serial.println("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected, my IP address is: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  checkForPirInput();

  if (shouldSendHeartbeat()) {
    lastHeartbeatTime = millis();
    sendHeartbeat();
  }
}

void checkForPirInput() {
  if (isPirInputHigh() && !movementAlreadyDetected) {
    onPirMovementDetected();
  } else if (isPirInputLow() && movementAlreadyDetected) {
    onPirMovementGone();
  }
}

bool isPirInputHigh() {
  return isPinHigh(PIR_INPUT_PIN);
}

bool isPirInputLow() {
  return isPinLow(PIR_INPUT_PIN);
}

bool isPinHigh(int pinNumber) {
  return digitalRead(pinNumber) == HIGH;
}

bool isPinLow(int pinNumber) {
  return !isPinHigh(pinNumber);
}

void onPirMovementDetected() {
  movementAlreadyDetected = true;
  lightLed();
  sendAlert();
  Serial.println("Motion detected");
}

void onPirMovementGone() {
  movementAlreadyDetected = false;
  lightDownLed();
  Serial.println("Motion gone");
}

void lightLed() {
  digitalWrite(LED_INPUT_PIN, HIGH);
  digitalWrite(LED_OUTPUT_PIN, LOW);
}

void lightDownLed() {
    digitalWrite(LED_INPUT_PIN, LOW);
    digitalWrite(LED_OUTPUT_PIN, HIGH);
}

void sendAlert() {
  sendRequest(alertEndpoint, getDataJson());    
}

bool shouldSendHeartbeat() {
  return (millis() - lastHeartbeatTime) > heartbeatSendIntervalMs;
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

String getDataJson() {
  return "{}";
}

String getHeartbeatJson() {
  unsigned long currentTime = millis();
  unsigned long timeFromStart = currentTime - startTime;
  String firstPart = "{\"deviceName\":\"";
  String secondPart = "\",\"timeFromStart\":\"";
  String timeFromStartString = String(timeFromStart);
  String thirdPart = "\",\"timeFromAlert\":\"";
  String timeFromAlertString = String(-1);
  String fourthPart = "\"}";
  return firstPart + deviceName + secondPart + timeFromStartString + thirdPart + timeFromAlertString + fourthPart;
}
