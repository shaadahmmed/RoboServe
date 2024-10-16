#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "NewPing.h"
#include <Servo.h>

enum State : uint8_t
{
    TABLENO,
    ULTRASONICDATA,
    IRDATA,
    IRULTRA,
    ORDERPLACED,
    ORDERCONFIRMED,
    ORDERCOMPLETE,
    NOTFOUND,
    FOUND,
    NEWTABLE
};

struct DataPacket
{
    State request;
    State found;
    String tableNo;
    long ultrasonicData;
    bool leftIRData;
    bool rightIRData;
    String TableData;
};

DataPacket sendData, receiveData;

uint8_t carAddress[] = {0xD8, 0xF1, 0x5B, 0x08, 0x36, 0x55};
uint8_t sensorAddress[] = {0x50, 0x02, 0x91, 0xEB, 0x40, 0xDF};
uint8_t sdAddress[] = {0xB8, 0xD6, 0x1A, 0xA7, 0x98, 0xDC};

uint8_t leftIRpin = D2;
uint8_t rightIRpin = D3;
uint8_t ultrasonicTrigPin = D4;
uint8_t ultrasonicEchoPin = D5;
uint8_t servo1Pin = D6;
uint8_t servo2Pin = D7;
Servo servo1, servo2;

NewPing sonar(ultrasonicTrigPin, ultrasonicEchoPin, 200);

void calculateIR();
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len);
void openTray();
void closeTray();

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
    Serial.print("Last Packet Send Status: ");
    if (sendStatus == 0)
    {
        Serial.println("Delivery Success");
    }
    else
    {
        Serial.println("Delivery Failed");
    }
}

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
    receiveData = *reinterpret_cast<DataPacket *>(incomingData);
    if (receiveData.request == IRDATA)
    {
        calculateIR();
        sendData.request = IRDATA;
        sendData.found = FOUND;
        esp_now_send(mac, (uint8_t *)&sendData, sizeof(sendData));
    }
    else if (receiveData.request == ULTRASONICDATA)
    {
        sendData.request = ULTRASONICDATA;
        sendData.ultrasonicData = sonar.ping_cm();
        sendData.found = FOUND;
        esp_now_send(mac, (uint8_t *)&sendData, sizeof(sendData));
    }
    else if (receiveData.request == IRULTRA)
    {
        calculateIR();
        sendData.request = IRDATA;
        sendData.ultrasonicData = sonar.ping_cm();
        sendData.found = FOUND;
        esp_now_send(mac, (uint8_t *)&sendData, sizeof(sendData));
    }
    else if (receiveData.request == ORDERPLACED)
    {
        openTray();
    }
    else if (receiveData.request == ORDERCOMPLETE)
    {
        closeTray();
    }
    else
    {
        sendData.request = NOTFOUND;
        esp_now_send(mac, (uint8_t *)&sendData, sizeof(sendData));
    }
}

void setup()
{
    // Init Serial Monitor
    Serial.begin(9600);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // // Once ESPNow is successfully Init, we will register for Send CB to
    // // get the status of Trasnmitted packet
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);

    // Register peer
    esp_now_add_peer(carAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    esp_now_add_peer(sdAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin);
    closeTray();
}

void loop()
{
}

void calculateIR()
{
    sendData.leftIRData = digitalRead(leftIRpin);
    sendData.rightIRData = digitalRead(rightIRpin);
}

void openTray()
{
    servo1.write(180);
    servo2.write(0);
}
void closeTray()
{
    servo1.write(0);
    servo2.write(180);
}