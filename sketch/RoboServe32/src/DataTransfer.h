// #ifndef DATA_TRANSFER_H
// #define DATA_TRANSFER_H

// #include <WiFi.h>
// #include <WebServer.h>
// #include <SD.h>
// #include <SPI.h>
// #include <Wire.h>
// #include <esp_now.h>
// #include "setup.h"

// enum State : uint8_t
// {
//     TABLENO,
//     ULTRASONICDATA,
//     IRDATA,
//     IRULTRA,
//     ORDERPLACED,
//     ORDERCONFIRMED,
//     ORDERCOMPLETE,
//     NOTFOUND,
//     FOUND,
//     NEWTABLE
// };

// struct DataPacket
// {
//     State request;
//     State found;
//     String tableNo;
//     long ultrasonicData;
//     bool leftIRData;
//     bool rightIRData;
//     String tableData;
// };

// DataPacket sendData, receiveData;
// esp_now_peer_info_t peerInfo;
// State recieveState;

// uint8_t carAddress[] = {0xD8, 0xF1, 0x5B, 0x08, 0x36, 0x55};
// uint8_t sensorAddress[] = {0x50, 0x02, 0x91, 0xEB, 0x40, 0xDF};
// uint8_t sdAddress[] = {0xB8, 0xD6, 0x1A, 0xA7, 0x98, 0xDC};

// void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
// {
//     Serial.print("Last Packet Send Status: ");
//     if (status == ESP_NOW_SEND_SUCCESS)
//     {
//         Serial.println("Delivery Success");
//     }
//     else
//     {
//         Serial.println("Delivery Fail");
//     }
// }

// void onDataRecv(const uint8_t *mac, const uint8_t *data, int len)
// {
//     memcpy(&receiveData, data, len);
//     if (receiveData.request == TABLENO)
//     {
//         String table = "/table/" + receiveData.tableNo + ".txt";
//         File tableData = SD.open((table), FILE_READ);
//         if (!tableData)
//         {
//             sendData.request = TABLENO;
//             sendData.found = NOTFOUND;
//             sendData.tableNo = receiveData.tableNo;
//             esp_now_send(mac, (uint8_t *)&sendData, sizeof(sendData));
//             Serial.println("Table not found");
//             return;
//         }
//         sendData.request = TABLENO;
//         sendData.found = FOUND;
//         sendData.tableNo = receiveData.tableNo;
//         esp_now_send(mac, (uint8_t *)&sendData, sizeof(sendData));
//         Serial.println("Table found and Send");
//     }
//     else if (receiveData.request == ORDERCOMPLETE)
//         orderList.pop();
//     Serial.println("Data received");
// }

// void init_esp()
// {
//     if (esp_now_init() != ESP_OK)
//     {
//         Serial.println("Error initializing ESP-NOW");
//         return;
//     }
//     memcpy(peerInfo.peer_addr, carAddress, 6);
//     peerInfo.channel = 0;
//     peerInfo.encrypt = false;
//     if (esp_now_add_peer(&peerInfo) != ESP_OK)
//     {
//         Serial.println("Failed to add peer");
//         return;
//     }
//     memcpy(peerInfo.peer_addr, sensorAddress, 6);
//     if (esp_now_add_peer(&peerInfo) != ESP_OK)
//     {
//         Serial.println("Failed to add peer");
//         return;
//     }
// }

// #endif