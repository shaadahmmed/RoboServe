#include <Arduino.h>
#include "setup.h"
#include "display.h"

const int buttonPin = D3;
int buttonState = 0;
unsigned long previousMillis = 0;
const long interval = 100; // Set a debounce interval

void setup()
{
  beginDisplay();
  initialSDCard();
  displayPrintCenter("Welcome to", 1);
  displayPrintCenter("Robo Serve", 2);
  delay(2000);
  displayClear();
  beginSetup();
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop()
{
  server.handleClient();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW)
      removeCredentials();
  }
}
