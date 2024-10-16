#include <Arduino.h>
#include "setup.h"
#include "display.h"
#include "DataTransfer.h"

const int buttonPin = 4;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int lastButtonState = HIGH;

void setup()
{
  beginDisplay();
  initialSDCard();
  // init_esp();
  // removeCredentials();
  delay(2000);
  displayClear();
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

  // Debouncing the button
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading == LOW)
    {
      removeCredentials();
    }
  }

  lastButtonState = reading;
}

// #include <Wire.h>
// #include <Arduino.h>
// #include "setup.h"

// void setup()
// {
//   Wire.begin();
//   Serial.begin(115200);
//   while (!SD.begin(5))
//   {
//     Serial.println("SD Card failed");
//     delay(1000);
//   }
//   Serial.println("SD Card initialized");
// }

// void loop()
// {
//   // Empty loop
// }
