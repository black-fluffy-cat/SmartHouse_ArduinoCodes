#include  <ESP8266WiFi.h>  
 
const char* ssid = "TP-LINK_E6565C"; // WiFi name
const char* password = ""; // WiFi password

int ledPin = 2; // D4
int ledPin2 = 12; // D6
int iterator = 0;
WiFiServer server(80);
 
void setup() {
Serial.begin(9600);
delay(10);
 
pinMode(ledPin, OUTPUT);
digitalWrite(ledPin, LOW);
pinMode(ledPin2, OUTPUT);
digitalWrite(ledPin2, LOW);
 
// Connect to WiFi network
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
 
Serial.println("");
Serial.println("WiFi connected");
 
// Start the server
server.begin();
Serial.println("Server started");
 
// Print the IP address
Serial.print("Use this URL to connect: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/"); 
}

void loop() {
iterator++;
// Check if a client has connected
WiFiClient client = server.available();
if (!client) {
  return;
}
 
// Wait until the client sends some data
Serial.println("new client");
int timewate = 0;
while(!client.available()){
  delay(1);
  timewate = timewate +1;
  if(timewate>1800)
  {
    Serial.println(">>> Client Timeout !");
    client.stop();
    return;
  }
}
 
// Read the first line of the request
String request = client.readStringUntil('\r');
Serial.println(request);
client.flush();
 
// Match the request 
int value = LOW;
if (request.indexOf("/LED=ON") != -1)  {
  digitalWrite(ledPin, HIGH);
  value = HIGH;
}
 
if (request.indexOf("/LED=OFF") != -1)  {
  digitalWrite(ledPin, LOW);
  value = LOW;
}
if (request.indexOf("/LED2=ON") != -1)  {
  digitalWrite(ledPin2, HIGH);
  value = HIGH;
}
if (request.indexOf("/LED2=OFF") != -1)  {
  digitalWrite(ledPin2, LOW);
  value = LOW;
}
 
// Return the response
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println(""); //  do not forget this one
client.println("");
client.println("");
client.print("Led pin is now: "); client.print(iterator);
 
client.println("");
client.println("");
client.println("");
client.println("");
client.println("");
client.println("");
delay(1);
Serial.println("Client disonnected");
Serial.println("");
}
