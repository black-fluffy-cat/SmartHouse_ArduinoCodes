char c;
 
#include <SoftwareSerial.h> 
SoftwareSerial BTserial(18, 19); // RX, TX
//D18 is A4
//D19 is A5
 
void setup() 
{
     Serial.begin(9600);
     Serial.println("Serial started at 9600");
 
     BTserial.begin(9600);  
     Serial.println("BTserial started at 9600");
}
 
void loop() 
{
 
    if (Serial.available() > 0) 
    {
       c = Serial.read();
       BTserial.write(c);
    }
 
 
    if (BTserial.available() > 0) 
    {
       c = BTserial.read();
       Serial.write(c);
    }
}
