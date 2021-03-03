#include <Adafruit_NeoPixel.h> //Dołączenie biblioteki

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define NUM_LEDS 8
#define LED_PIN D5
#define colorPower 255

Adafruit_NeoPixel ledBoard = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

ESP8266WebServer server;

const char* networkSSID = "TP-LINK_E6565C"; // WiFi name
const char* networkPwd = "38886300"; // WiFi password

String deviceName = "NodeMCU_+_LED_strip";

int red = 0;
int green = 0;
int blue = 0;

void setup() {
  Serial.begin(115200);
      
  WiFi.begin(networkSSID, networkPwd);
  Serial.println("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected, my IP address is: ");
  Serial.println(WiFi.localIP());

  server.on("/red", setRed);
  server.on("/green", setGreen);
  server.on("/blue", setBlue);
  server.on("/yellow", setYellow);
  server.on("/white", setWhite);
  server.on("/purple", setPurple);
  server.on("/cyan", setCyan);
  server.on("/handleJson", handleJson);
  
  server.begin();
 
  ledBoard.begin();
  ledBoard.show();
}

void handleJson() {
  DynamicJsonDocument processedJson(500);
  deserializeJson(processedJson, server.arg("plain"));
  String message = "Received: " + server.arg("plain") + "\n";
  Serial.println(message);

  String serverUrl = processedJson["serverUrl"];
   message = "argument serverUrl: " + serverUrl;
  Serial.println(message);
  
  server.send(200, "text/json", "{\"result\":\"OK\"}");
}

void loop() {
  server.handleClient();
}

void setRed() {
  red = 1 * colorPower;
  green = 0;
  blue = 0;
  uploadColorToLeds();
}

void setGreen() {
  red = 0;
  green = 1 * colorPower;
  blue = 0;
  uploadColorToLeds();
}

void setBlue() {
  red = 0;
  green = 0;
  blue = 1 * colorPower;
  uploadColorToLeds();
}

void setYellow() {
  red = 1 * colorPower;
  green = 1 * colorPower;
  blue = 0;
  uploadColorToLeds();
}

void setWhite() {
  red = 1 * colorPower;
  green = 1 * colorPower;
  blue = 1 * colorPower;
  uploadColorToLeds();
}

void setPurple() {
  red = 1 * colorPower;
  green = 0;
  blue = 1 * colorPower;
  uploadColorToLeds();
}

void setCyan() {
  red = 0;
  green = 1 * colorPower;
  blue = 1 * colorPower;
  uploadColorToLeds();
}

void uploadColorToLeds() {
  for(int i = 0; i < NUM_LEDS; i++) {
      ledBoard.setPixelColor(i, ledBoard.Color(red, green, blue));
  }
  ledBoard.show();
}
