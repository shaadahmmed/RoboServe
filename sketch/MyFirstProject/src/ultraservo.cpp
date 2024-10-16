// #include <Arduino.h>
// #include <Servo.h>

// const int echoPin = D5;
// const int trigPin = D6;

// Servo myservo;
// long TravelTime, distance;

// void hungry_tommy();
// long calculate_distance();

// void setup()
// {
//     Serial.begin(9600);
//     pinMode(echoPin, INPUT);
//     pinMode(trigPin, OUTPUT);
//     myservo.attach(D4);
//     myservo.write(90);
//     delay(1000);
// }

// void loop()
// {
//     hungry_tommy();
//     // Serial.println(calculate_distance());
//     // delay(10);
// }

// void hungry_tommy() // continuously moves 0 to 180, stops when obstacle found
// {
//     for (int i = 100; i <= 180; i += 10)
//     {
//         myservo.write(i);
//         Serial.println(distance);
//         while (calculate_distance() <= 20)
//         {
//             Serial.print("Object Detected Left at: ");
//             Serial.print(distance);
//             Serial.println("cm");
//         }
//         delay(50);
//     }
//     for (int i = 170; i >= 90; i -= 10)
//     {
//         myservo.write(i);
//         Serial.println(distance);
//         while (calculate_distance() <= 20)
//         {
//             Serial.print("Object Detected Left at: ");
//             Serial.print(distance);
//             Serial.println("cm");
//         }
//         delay(50);
//     }
//     for (int i = 80; i >= 0; i -= 10)
//     {
//         myservo.write(i);
//         Serial.println(distance);
//         while (calculate_distance() <= 20)
//         {
//             Serial.print("Object Detected Right at: ");
//             Serial.print(distance);
//             Serial.println("cm");
//         }
//         delay(50);
//     }
//     for (int i = 10; i <= 90; i += 10)
//     {
//         myservo.write(i);
//         Serial.println(distance);
//         while (calculate_distance() <= 20)
//         {
//             Serial.print("Object Detected Right at: ");
//             Serial.print(distance);
//             Serial.println("cm");
//         }
//         delay(50);
//     }
// }

// long calculate_distance() 
// {

//     digitalWrite(trigPin, LOW);
//     delayMicroseconds(2);
//     digitalWrite(trigPin, HIGH);
//     delayMicroseconds(10);
//     digitalWrite(trigPin, LOW);

//     TravelTime = pulseIn(echoPin, HIGH);
//     distance = TravelTime / 58.2;

//     return distance;
// }