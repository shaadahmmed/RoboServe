// #include <Arduino.h>
// #include <L298NX2.h>
// // #include <espnow.h>
// // #include <ESP8266WiFi.h>

// // enum State : uint8_t
// // {
// //     IRDATA,
// //     ULTRASONICDATA,
// //     COMBINE
// // };

// // typedef struct Data
// // {
// //     int leftIR;
// //     int rightIR;
// //     double ultrasonic;
// // } Data;

// // Data data;

// // uint8_t broadcastAddress[] = {0x08, 0xF9, 0xE0, 0x6C, 0x8B, 0x6B};

// uint8_t leftIRpin = D1;
// L298NX2 motor(D2, D3, D4, D5, D7, D6);
// // int isDataRecieved = 0;

// // void onDataRecieve(uint8_t *broadcastAddress, uint8_t *incomingData, uint8_t size);
// // void onDataSend(uint8_t *broadcastAddress, uint8_t status);
// void reset();

// void forward(int, int);
// void backward(int, int);
// void turnRight(int);
// void turnLeft(int);
// void turn(int);
// void stop();

// void setup()
// {
//     Serial.begin(9600);
//     motor.stop();
//     pinMode(leftIRpin, INPUT);
//     // WiFi.mode(WIFI_STA);
//     // if (esp_now_init() != 0)
//     // {
//     //     Serial.println("Error initializing ESP-NOW");
//     //     return;
//     // }
//     // esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
//     // esp_now_register_recv_cb(onDataRecieve);
//     // esp_now_register_send_cb(onDataSend);
//     // esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
// }

// void loop()
// {
//     forward(6, 100);
//     turnRight(150);
//     forward(6, 100);
//     turnRight(150);
//     forward(6, 100);
//     turnRight(150);
//     forward(6, 100);
//     motor.stop();
//     while (true)
//         ;
//     // State state = IRDATA;
//     // bool lastState = digitalRead(leftIRpin);
//     // while (sum < 16)
//     // {
//     //     Serial.println("Moving");
//     //     bool leftIR = digitalRead(leftIRpin);
//     //     motor.setSpeed(150);
//     //     motor.forward();
//     //     // esp_now_send(broadcastAddress, (uint8_t *)&state, sizeof(State));
//     //     // while (!isDataRecieved)
//     //     //     ;
//     //     if (leftIR == HIGH && lastState == LOW)
//     //     {
//     //         sum++;
//     //     }
//     //     Serial.println(sum);
//     //     lastState = leftIR;
//     //     // isDataRecieved = false;
//     // }
//     // reset();
//     // while (sum < 4)
//     // {
//     //     Serial.println("Turning");
//     //     bool leftIR = digitalRead(leftIRpin);
//     //     motor.setSpeed(200);
//     //     motor.forwardA();
//     //     motor.backwardB();
//     //     // esp_now_send(broadcastAddress, (uint8_t *)&state, sizeof(State));
//     //     // while (!isDataRecieved)
//     //     //     ;
//     //     if (leftIR == HIGH && lastState == LOW)
//     //     {
//     //         sum++;
//     //     }
//     //     Serial.println(sum);
//     //     lastState = leftIR;
//     //     // isDataRecieved = false;
//     // }
//     // reset();
//     // count++;
// }

// // void onDataSend(uint8_t *broadcastAddress, uint8_t status)
// // {
// //     if (status == 0)
// //         Serial.println("Request Send Successful");
// //     else
// //         Serial.println("Request Failed");
// // }

// // void onDataRecieve(uint8_t *broadcastAddress, uint8_t *incomingData, uint8_t size)
// // {
// //     memcpy(&data, incomingData, size);
// //     Serial.println(data.leftIR);
// //     Serial.println(data.rightIR);
// //     Serial.println(data.ultrasonic);
// //     isDataRecieved = true;
// // }

// void forward(int steps, int speed)
// {
//     int sum = 0;
//     bool lastState = digitalRead(leftIRpin);
//     while (sum < steps * 2)
//     {
//         Serial.println("Forward");
//         bool leftIR = digitalRead(leftIRpin);
//         motor.setSpeed(speed);
//         motor.forward();
//         // esp_now_send(broadcastAddress, (uint8_t *)&state, sizeof(State));
//         // while (!isDataRecieved)
//         //     ;
//         if (leftIR == HIGH && lastState == LOW)
//         {
//             sum++;
//         }
//         Serial.println(sum);
//         lastState = leftIR;
//         // isDataRecieved = false;
//     }
// }
// void backward(int steps, int speed)
// {
//     int sum = 0;
//     bool lastState = digitalRead(leftIRpin);
//     while (sum < steps * 2)
//     {
//         Serial.println("Backward");
//         bool leftIR = digitalRead(leftIRpin);
//         motor.setSpeed(speed);
//         motor.backward();
//         // esp_now_send(broadcastAddress, (uint8_t *)&state, sizeof(State));
//         // while (!isDataRecieved)
//         //     ;
//         if (leftIR == HIGH && lastState == LOW)
//         {
//             sum++;
//         }
//         Serial.println(sum);
//         lastState = leftIR;
//         // isDataRecieved = false;
//     }
// }
// void turnRight(int speed)
// {
//     int sum = 0;
//     bool lastState = digitalRead(leftIRpin);
//     while (sum < 2)
//     {
//         Serial.println("Right");
//         bool leftIR = digitalRead(leftIRpin);
//         motor.setSpeed(speed);
//         motor.forwardA();
//         motor.backwardB();
//         // esp_now_send(broadcastAddress, (uint8_t *)&state, sizeof(State));
//         // while (!isDataRecieved)
//         //     ;
//         if (leftIR == HIGH && lastState == LOW)
//         {
//             sum++;
//         }
//         Serial.println(sum);
//         lastState = leftIR;
//         // isDataRecieved = false;
//     }
// }
// void turn(int speed)
// {
//     int sum = 0;
//     bool lastState = digitalRead(leftIRpin);
//     while (sum < 4)
//     {
//         Serial.println("Turning");
//         bool leftIR = digitalRead(leftIRpin);
//         motor.setSpeed(200);
//         motor.forwardA();
//         motor.backwardB();
//         // esp_now_send(broadcastAddress, (uint8_t *)&state, sizeof(State));
//         // while (!isDataRecieved)
//         //     ;
//         if (leftIR == HIGH && lastState == LOW)
//         {
//             sum++;
//         }
//         Serial.println(sum);
//         lastState = leftIR;
//         // isDataRecieved = false;
//     }
// }

// // void reset()
// // {
// //     motor.stop();
// //     sum = 0;
// // }
