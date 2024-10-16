// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
// #include <L298NX2.h>
// #include <Arduino.h>

// void handleRoot();
// void handleTurn();
// void performRightTurn(int speed, int delayMs);

// // Motor pins configuration for L298N
// L298NX2 motor(D2, D3, D4, D5, D7, D6);

// // Create Wi-Fi server
// ESP8266WebServer server(80);

// // Variables to store turn delay and motor speed
// int turnDelay = 500;  // Default turn delay for a 90-degree turn
// int motorSpeed = 150; // Default motor speed

// // Wi-Fi credentials for AP mode
// const char *ssid = "RoboServe";
// const char *password = "admin1234";

// // HTML content for web page (mobile-friendly, with sliders)
// const char *htmlPage =
//     "<html>\
//     <head>\
//       <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
//       <style>\
//         body { font-family: Arial, sans-serif; text-align: center; padding: 20px; }\
//         .slider-container { margin: 20px 0; }\
//         label { font-size: 1.2em; }\
//         input[type=range] { width: 100%; max-width: 300px; }\
//         .button { padding: 10px 20px; background-color: #4CAF50; color: white; border: none; font-size: 1.2em; cursor: pointer; margin: 10px 0; }\
//         .button:hover { background-color: #45a049; }\
//       </style>\
//     </head>\
//     <body>\
//       <h1>90 Degree Turn Control</h1>\
//       <div class='slider-container'>\
//         <label for='speed'>Motor Speed: <span id='speedValue'>150</span></label><br>\
//         <input type='range' id='speed' name='speed' min='50' max='255' value='150' oninput='updateSpeedLabel(this.value)'>\
//       </div>\
//       <div class='slider-container'>\
//         <label for='delay'>Turn Delay (ms): <span id='delayValue'>500</span></label><br>\
//         <input type='range' id='delay' name='delay' min='100' max='2000' value='500' oninput='updateDelayLabel(this.value)'>\
//       </div>\
//       <button class='button' onclick='sendTurnRequest()'>Test Right Turn</button>\
//       <script>\
//         function updateSpeedLabel(value) {\
//           document.getElementById('speedValue').innerText = value;\
//         }\
//         function updateDelayLabel(value) {\
//           document.getElementById('delayValue').innerText = value;\
//         }\
//         function sendTurnRequest() {\
//           var speed = document.getElementById('speed').value;\
//           var delay = document.getElementById('delay').value;\
//           var xhr = new XMLHttpRequest();\
//           xhr.open('POST', '/turn', true);\
//           xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');\
//           xhr.send('speed=' + speed + '&delay=' + delay);\
//           alert('Turn request sent!\\nSpeed: ' + speed + '\\nDelay: ' + delay + ' ms');\
//         }\
//       </script>\
//     </body>\
//   </html>";

// void setup()
// {
//     Serial.begin(9600);
//     motor.stop();

//     // Set up Wi-Fi in AP mode
//     WiFi.softAP(ssid, password);
//     Serial.println("Wi-Fi Access Point started");

//     // Start server and serve the web page
//     server.on("/", handleRoot);
//     server.on("/turn", handleTurn); // Handle form submission for turn
//     server.begin();
//     Serial.println("HTTP server started");

//     // Initialize motors
//     motor.setSpeed(motorSpeed);
// }

// void loop()
// {
//     server.handleClient(); // Handle client requests
// }

// // Handle root URL (serve the HTML page)
// void handleRoot()
// {
//     server.send(200, "text/html", htmlPage);
// }

// // Handle form submission for turn control (POST request)
// void handleTurn()
// {
//     if (server.hasArg("speed") && server.hasArg("delay"))
//     {
//         motorSpeed = server.arg("speed").toInt(); // Get speed value from web form
//         turnDelay = server.arg("delay").toInt();  // Get delay value from web form
//         Serial.print("Turn delay set to: ");
//         Serial.println(turnDelay);
//         Serial.print("Motor speed set to: ");
//         Serial.println(motorSpeed);

//         // Perform right turn (90-degree) with the specified delay and speed
//         performRightTurn(motorSpeed, turnDelay);

//         // Respond back to the client
//         server.send(200, "text/html", "<html><body><h1>Right turn test completed!</h1><p>Speed: " + String(motorSpeed) + "<br>Delay: " + String(turnDelay) + " ms</p><a href=\"/\">Go back</a></body></html>");
//     }
//     else
//     {
//         server.send(400, "text/html", "Invalid input");
//     }
// }

// // Perform a right turn (90 degrees) using the specified delay and speed
// void performRightTurn(int speed, int delayMs)
// {
//     motor.setSpeed(speed); // Set motor speed for the turn

//     // Run left motor forward and right motor backward for a sharp right turn
//     motor.forwardA();  // Left motor forward
//     motor.backwardB(); // Right motor backward

//     delay(delayMs);        // Turn for the specified duration
//     motor.setSpeed(speed); // Reduce speed to half to apply gentle stop pressure
//     motor.forwardB();      // Momentarily run the right motor forward to counter momentum
//     delay(50);             // Wait for a short duration

//     motor.stop(); // Stop the motors after the turn
// }
