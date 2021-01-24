
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>

const char* ssid = "TP-LINK_E6565C"; // WiFi name
const char* password = ""; // WiFi password

IPAddress ip (170,194,156,208);




int avg_ms, x;
String avg_ms_str;

void setup() {
  Serial.begin(9600);
  Serial.println();

  Serial.println();
  Serial.println("Connecting");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  
  Serial.println("Connected");
  delay(1000);
}


void loop() {
  Ping.ping(ip, 2);
  avg_ms = Ping.averageTime();
  Serial.println(String(avg_ms));
}
