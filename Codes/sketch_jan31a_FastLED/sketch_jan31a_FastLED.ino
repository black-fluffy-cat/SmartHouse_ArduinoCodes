#include <Adafruit_NeoPixel.h> //Dołączenie biblioteki

#define NUM_LEDS 8
#define LED_PIN 5
#define colorPower 10

Adafruit_NeoPixel ledBoard = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

int red = 0;
int green = 0;
int blue = 0;

void setup() {
  ledBoard.begin();
  ledBoard.show(); 

}

void loop() {

  for (int i = 0; i < NUM_LEDS; i++) {
    setRed();
    ledBoard.setPixelColor(i, ledBoard.Color(red, green, blue));
    setGreen();
    ledBoard.setPixelColor((i + 1) % NUM_LEDS, ledBoard.Color(red, green, blue));
    setBlue();
    ledBoard.setPixelColor((i + 2) % NUM_LEDS, ledBoard.Color(red, green, blue));
    setYellow();
    ledBoard.setPixelColor((i + 3) % NUM_LEDS, ledBoard.Color(red, green, blue));
    setWhite();
    ledBoard.setPixelColor((i + 4) % NUM_LEDS, ledBoard.Color(red, green, blue));
    setPurple();
    ledBoard.setPixelColor((i + 5) % NUM_LEDS, ledBoard.Color(red, green, blue));
    setCyan();
    ledBoard.setPixelColor((i + 6) % NUM_LEDS, ledBoard.Color(red, green, blue));
    setYellow();
    ledBoard.setPixelColor((i + 7) % NUM_LEDS, ledBoard.Color(red, green, blue));
    
    ledBoard.show();
    delay(500);
  }
}

void setRed() {
  red = 1 * colorPower;
  green = 0;
  blue = 0;
}

void setGreen() {
  red = 0;
  green = 1 * colorPower;
  blue = 0;
}

void setBlue() {
  red = 0;
  green = 0;
  blue = 1 * colorPower;
}

void setYellow() {
  red = 1 * colorPower;
  green = 1 * colorPower;
  blue = 0;
}

void setWhite() {
  red = 1 * colorPower;
  green = 1 * colorPower;
  blue = 1 * colorPower;
}

void setPurple() {
  red = 1 * colorPower;
  green = 0;
  blue = 1 * colorPower;
}

void setCyan() {
  red = 0;
  green = 1 * colorPower;
  blue = 1 * colorPower;
}
