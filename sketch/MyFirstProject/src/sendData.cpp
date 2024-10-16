// #include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <espnow.h>
// #include "newping.h"

// enum State : uint8_t
// {
//     IRDATA,
//     ULTRASONICDATA,
//     COMBINE
// };

// typedef struct Data
// {
//     int leftIR;
//     int rightIR;
//     double ultrasonic;
// } Data;

// Data data;

// uint8_t broadcastAddress[] = {0xD8, 0xF1, 0x5B, 0x08, 0x36, 0x55};

// uint8_t leftIRpin = D2;
// uint8_t rightIRpin = D3;
// uint8_t trigPin = D4;
// uint8_t echoPin = D5;
// NewPing ultrasonic(trigPin, echoPin, 200);

// void CalculateIR();
// void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
// void OnRecieveData(uint8_t *mac, uint8_t *incomingData, uint8_t len);

// void setup()
// {
//     Serial.begin(9600);
//     WiFi.mode(WIFI_STA);
//     if (esp_now_init() != 0)
//     {
//         Serial.println("Error initializing ESP-NOW");
//         return;
//     }
//     esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
//     esp_now_register_recv_cb(OnRecieveData);
//     esp_now_register_send_cb(OnDataSent);
//     esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
// }

// void loop()
// {
//     // delay(1000);
//     // esp_now_send(broadcastAddress, (uint8_t *)&data, sizeof(Data));
// }

// void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
// {
//     Serial.print("Last Packet Send Status: ");
//     if (sendStatus == 0)
//     {
//         Serial.println("Requesting Distance");
//     }
//     else
//     {
//         Serial.println("Request Failed");
//     }
// }

// void OnRecieveData(uint8_t *mac, uint8_t *incomingData, uint8_t len)
// {
//     Serial.println("Data Recieved");
//     State state;
//     memcpy(&state, incomingData, len);
//     Serial.println(state);
//     if (state == IRDATA)
//     {
//         CalculateIR();
//         esp_now_send(broadcastAddress, (uint8_t *)&data, sizeof(Data));
//     }
//     else if (state == ULTRASONICDATA)
//     {
//         data.ultrasonic = ultrasonic.ping_cm();
//         esp_now_send(broadcastAddress, (uint8_t *)&data, sizeof(Data));
//     }
//     else if (state == COMBINE)
//     {
//         CalculateIR();
//         data.ultrasonic = ultrasonic.ping_cm();
//         esp_now_send(broadcastAddress, (uint8_t *)&data, sizeof(Data));
//     }
// }

// void CalculateIR()
// {
//     data.leftIR = digitalRead(leftIRpin);
//     data.rightIR = digitalRead(rightIRpin);
// }