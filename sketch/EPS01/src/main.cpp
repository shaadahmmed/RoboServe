#include <Arduino.h>
#include <ESP8266WiFi.h>

const char *ssid = "ARKO";
const char *password = "arkobecm16";

WiFiServer server(80);

void setup()
{
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }

    server.begin();
    Serial.println("Server started");
}

void loop()
{
    WiFiClient client = server.available();
    if (client)
    {
        String request = client.readStringUntil('\r');
        client.flush();

        request.substring(6);

        Serial.println(request);
    }
}