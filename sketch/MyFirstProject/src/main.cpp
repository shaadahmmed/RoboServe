// #include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <L298NX2.h>

// const char *ssid = "RoboServe";
// const char *password = "admin1234";

// L298NX2 motor(D5, D7, D6, D2, D3, D4);

// WiFiServer server(80);

// void setup()
// {
//     motor.stop();
//     Serial.begin(9600);
//     WiFi.softAP(ssid, password);
//     server.begin();
//     Serial.println("Server started");
// }

// void loop()
// {
//     WiFiClient client = server.accept();
//     if (client)
//     {
//         String request = client.readStringUntil('\r');
//         client.flush();

//         // Forward
//         if (request.indexOf("/F") != -1)
//         {
//             motor.setSpeed(150);
//             motor.forward();
//         }

//         // Backward
//         if (request.indexOf("/B") != -1)
//         {
//             motor.setSpeed(150);
//             motor.backward();
//         }

//         // Left
//         if (request.indexOf("/L") != -1)
//         {
//             motor.setSpeedA(50);
//             motor.setSpeedB(150);
//             motor.forward();
//         }

//         // Right
//         if (request.indexOf("/R") != -1)
//         {
//             motor.setSpeedA(150);
//             motor.setSpeedB(50);
//             motor.forward();
//         }

//         // Stop
//         if (request.indexOf("/S") != -1)
//         {
//             motor.stop();
//         }

//         // Create the HTML webpage
//         client.println("HTTP/1.1 200 OK");
//         client.println("Content-Type: text/html");
//         client.println("");
//         client.println("<!DOCTYPE HTML>");
//         client.println("<html>");
//         client.println("<head>");
//         client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
//         client.println("<style>");
//         client.println("body { font-family: Arial, sans-serif; text-align: center; }");
//         client.println("button { padding: 20px; font-size: 18px; margin: 10px; width: 100px; user-select: none; /* Prevent text selection */ }");
//         client.println(".control-buttons { display: flex; justify-content: center; flex-wrap: wrap; }");
//         client.println("@media (max-width: 600px) { button { width: 80px; padding: 15px; font-size: 16px; } }");
//         client.println("</style>");
//         client.println("</head>");
//         client.println("<body>");
//         client.println("<h1>Remote Control Car</h1>");
//         client.println("<div class=\"control-buttons\">");
//         client.println("<button id=\"forward\">Forward</button><br>");
//         client.println("<button id=\"left\">Left</button>");
//         client.println("<button id=\"right\">Right</button><br>");
//         client.println("<button id=\"backward\">Backward</button><br>");
//         client.println("</div>");
//         client.println("<script>");
//         client.println("function sendRequest(action) {");
//         client.println("  var xhr = new XMLHttpRequest();");
//         client.println("  xhr.open('GET', action, true);");
//         client.println("  xhr.send();");
//         client.println("}");
//         client.println("document.getElementById('forward').onmousedown = function() { sendRequest('/F'); };");
//         client.println("document.getElementById('forward').onmouseup = function() { sendRequest('/S'); };");
//         client.println("document.getElementById('backward').onmousedown = function() { sendRequest('/B'); };");
//         client.println("document.getElementById('backward').onmouseup = function() { sendRequest('/S'); };");
//         client.println("document.getElementById('left').onmousedown = function() { sendRequest('/L'); };");
//         client.println("document.getElementById('left').onmouseup = function() { sendRequest('/S'); };");
//         client.println("document.getElementById('right').onmousedown = function() { sendRequest('/R'); };");
//         client.println("document.getElementById('right').onmouseup = function() { sendRequest('/S'); };");

//         // For touch devices (mobile)
//         client.println("document.getElementById('forward').ontouchstart = function() { sendRequest('/F'); };");
//         client.println("document.getElementById('forward').ontouchend = function() { sendRequest('/S'); };");
//         client.println("document.getElementById('backward').ontouchstart = function() { sendRequest('/B'); };");
//         client.println("document.getElementById('backward').ontouchend = function() { sendRequest('/S'); };");
//         client.println("document.getElementById('left').ontouchstart = function() { sendRequest('/L'); };");
//         client.println("document.getElementById('left').ontouchend = function() { sendRequest('/S'); };");
//         client.println("document.getElementById('right').ontouchstart = function() { sendRequest('/R'); };");
//         client.println("document.getElementById('right').ontouchend = function() { sendRequest('/S'); };");
//         client.println("</script>");
//         client.println("</body>");
//         client.println("</html>");
//     }
// }