#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <L298NX2.h>
#include <espnow.h>
#include <sstream>
#include <string>

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
    String tableData;
};

DataPacket sendData, receiveData;
State recieveState;

uint8_t carAddress[] = {0xD8, 0xF1, 0x5B, 0x08, 0x36, 0x55};
uint8_t sensorAddress[] = {0x50, 0x02, 0x91, 0xEB, 0x40, 0xDF};
uint8_t sdAddress[] = {0xB8, 0xD6, 0x1A, 0xA7, 0x98, 0xDC};

int speed = 100;
int turnSpeed = 100;
const int autoSpeed = 100;

const char *ssid = "RoboServeControl";
const char *password = "admin1234";
String array[50];
int size;

L298NX2 motor(D2, D3, D4, D5, D7, D6);

WiFiServer server(80);

void autoForward(int length);
void autoBackward();
void autoLeft();
void autoRight();
void gotoTable(int tableNo, String tableData);
void stringToArray(String string, String array[]);

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
    if (receiveData.found == FOUND)
    {
        if (receiveData.request == TABLENO)
        {
            gotoTable(receiveData.tableNo.toInt(), receiveData.tableData);
        }
    }
}

void gotoTable(int tableNo, String tableData)
{
    stringToArray(tableData, array);
    for (int i = 0; i < size; i++)
    {
        if (!array[i].toInt())
        {
            if (array[i] == "B")
            {
                autoBackward();
            }
            else if (array[i] == "L")
            {
                autoLeft();
            }
            else if (array[i] == "R")
            {
                autoRight();
            }
        }
        else
        {
            autoForward(array[i].toInt());
        }
    }
    motor.stop();
}

void backFromTable()
{
    autoBackward();
    for (int i = size - 1; i >= 0; i--)
    {
        if (!array[i].toInt())
        {
            if (array[i] == "B")
            {
                autoBackward();
            }
            else if (array[i] == "L")
            {
                autoRight();
            }
            else if (array[i] == "R")
            {
                autoLeft();
            }
        }
        else
        {
            autoForward(array[i].toInt());
        }
    }
    motor.stop();
}

void autoForward(int length)
{
    int sum = 0;
    sendData.request = IRDATA;
    esp_now_send(sensorAddress, (uint8_t *)&sendData, sizeof(sendData));
    bool lastState = receiveData.leftIRData;
    while (sum < length / 4)
    {
        motor.setSpeed(autoSpeed);
        motor.forward();
        sendData.request = IRULTRA;
        esp_now_send(sensorAddress, (uint8_t *)&sendData, sizeof(sendData));
        while (receiveData.ultrasonicData <= 40)
        {
            motor.stop();
            sendData.request = ULTRASONICDATA;
            esp_now_send(sensorAddress, (uint8_t *)&sendData, sizeof(sendData));
        }
        motor.forward();
        if (receiveData.leftIRData == true && lastState == false)
        {
            sum++;
        }
        lastState = receiveData.leftIRData;
    }
    motor.stop();
}

void autoBackward()
{
    int sum = 0;
    sendData.request = IRDATA;
    esp_now_send(sensorAddress, (uint8_t *)&sendData, sizeof(sendData));
    bool lastState = receiveData.leftIRData;
    while (sum < 4)
    {
        motor.setSpeed(autoSpeed);
        motor.forwardA();
        motor.backwardB();
        sendData.request = IRDATA;
        esp_now_send(sensorAddress, (uint8_t *)&sendData, sizeof(sendData));
        if (receiveData.leftIRData == true && lastState == false)
        {
            sum++;
        }
        lastState = receiveData.leftIRData;
    }
    motor.stop();
}

void autoLeft()
{
    int sum = 0;
    sendData.request = IRDATA;
    esp_now_send(sensorAddress, (uint8_t *)&sendData, sizeof(sendData));
    bool lastState = receiveData.rightIRData;
    while (sum < 4)
    {
        motor.setSpeed(autoSpeed);
        motor.forwardB();
        motor.backwardA();
        sendData.request = IRDATA;
        esp_now_send(sensorAddress, (uint8_t *)&sendData, sizeof(sendData));
        if (receiveData.rightIRData == true && lastState == false)
        {
            sum++;
        }
        lastState = receiveData.rightIRData;
    }
    motor.stop();
}

void autoRight()
{
    int sum = 0;
    sendData.request = IRDATA;
    esp_now_send(sensorAddress, (uint8_t *)&sendData, sizeof(sendData));
    bool lastState = receiveData.leftIRData;
    while (sum < 4)
    {
        motor.setSpeed(autoSpeed);
        motor.forwardA();
        motor.backwardB();
        sendData.request = IRDATA;
        esp_now_send(sensorAddress, (uint8_t *)&sendData, sizeof(sendData));
        if (receiveData.leftIRData == true && lastState == false)
        {
            sum++;
        }
        lastState = receiveData.leftIRData;
    }
    motor.stop();
}

void stringToArray(String string, String array[])
{
    int r = 0, t = 0;

    for (uint8_t i = 0; i < string.length(); i++)
    {
        if (string[i] == ' ')
        {
            if (i - r > 1)
            {
                array[t] = string.substring(r, i);
                t++;
            }
            r = (i + 1);
        }
    }
    size = t;
}

void setup()
{
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);
    esp_now_add_peer(sdAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    esp_now_add_peer(sensorAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

    motor.stop();
    Serial.begin(9600);
    WiFi.softAP(ssid, password);
    server.begin();
    Serial.println("Server started");
}

void loop()
{
    WiFiClient client = server.accept();
    if (client)
    {
        String request = client.readStringUntil('\r');
        client.flush();

        request.substring(6);

        Serial.println(request);

        // Forward
        if (request.indexOf("/F") != -1)
        {
            motor.setSpeed(speed);
            motor.forward();
        }

        // Backward
        if (request.indexOf("/B") != -1)
        {
            motor.setSpeed(speed);
            motor.backward();
        }

        // Left
        if (request.indexOf("/L") != -1)
        {
            motor.setSpeedA(turnSpeed);
            motor.setSpeedB(turnSpeed);
            motor.forwardB();
            motor.backwardA();
        }

        // Right
        if (request.indexOf("/R") != -1)
        {
            motor.setSpeedA(turnSpeed);
            motor.setSpeedB(turnSpeed);
            motor.forwardA();
            motor.backwardB();
        }

        if (request.indexOf("/SP") != -1)
        {
            speed = request.substring(7).toInt();
        }

        if (request.indexOf("/T") != -1)
        {
            turnSpeed = request.substring(6).toInt();
        }

        if (request.indexOf("/TB") != -1)
        {
            int tableNo = request.substring(7).toInt();
            sendData.request = TABLENO;
            sendData.tableNo = tableNo;
            sendData.found = FOUND;
            esp_now_send(sdAddress, (uint8_t *)&sendData, sizeof(sendData));
            delay(1000);
        }
        if (request.indexOf("/TO") != -1)
        {
            sendData.found = FOUND;
            sendData.request = ORDERPLACED;
            esp_now_send(sensorAddress, (uint8_t *)&sendData, sizeof(sendData));
        }
        if (request.indexOf("/CB") != -1)
        {
            backFromTable();
            sendData.request = ORDERCOMPLETE;
            sendData.found = FOUND;
            esp_now_send(sdAddress, (uint8_t *)&sendData, sizeof(sendData));
            esp_now_send(sensorAddress, (uint8_t *)&sendData, sizeof(sendData));
        }

        // Stop
        if (request.indexOf("/S") != -1)
        {
            motor.stop();
        }

        // Create the HTML webpage
        client.println("<!DOCTYPE html>");
        client.println("<html>");
        client.println("<head>");
        client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />");
        client.println("<title>Admin Control</title>");
        client.println("<style>");
        client.println("body {");
        client.println("font-family: Arial, sans-serif;");
        client.println("text-align: center;");
        client.println("}");
        client.println("button {");
        client.println("padding: 20px;");
        client.println("font-size: 18px;");
        client.println("margin: 10px;");
        client.println("width: 100px;");
        client.println("user-select: none; /* Prevent text selection */");
        client.println("}");
        client.println(".control-buttons {");
        client.println("display: flex;");
        client.println("flex-direction: column;");
        client.println("justify-content: center;");
        client.println("flex-wrap: wrap;");
        client.println("}");
        client.println(".speedControl {");
        client.println("margin: 50px;");
        client.println("display: flex;");
        client.println("flex-direction: column;");
        client.println("justify-content: center;");
        client.println("align-items: center;");
        client.println("flex-wrap: wrap;");
        client.println("}");
        client.println("input[type=\"range\"] {");
        client.println("width: 300px;");
        client.println("margin: 10px;");
        client.println("}");
        client.println(".goToTable {");
        client.println("margin: 50px;");
        client.println("}");
        client.println("input[type=\"number\"] {");
        client.println("padding: 10px;");
        client.println("font-size: 16px;");
        client.println("width: 100px;");
        client.println("}");
        client.println("@media (max-width: 600px) {");
        client.println("button {");
        client.println("width: 80px;");
        client.println("padding: 15px;");
        client.println("font-size: 16px;");
        client.println("}");
        client.println("}");
        client.println("</style>");
        client.println("</head>");
        client.println("<body>");
        client.println("<h1>Remote Control Car</h1>");
        client.println("<div class=\"control-buttons\">");
        client.println("<div class=\"up\">");
        client.println("<div></div>");
        client.println("<button id=\"forward\">F</button>");
        client.println("<div></div>");
        client.println("</div>");
        client.println("<div class=\"down\">");
        client.println("<button id=\"left\">L</button>");
        client.println("<button id=\"backward\">B</button>");
        client.println("<button id=\"right\">R</button>");
        client.println("</div>");
        client.println("</div>");
        client.println("<div class=\"speedControl\">");
        client.println("<label for=\"speed\">Speed</label>");
        client.println("<label id=\"spd\">100</label>");
        client.println("<input type=\"range\" min=\"0\" max=\"255\" value=\"100\" name=\"speed\" id=\"speed\" />");
        client.println("<label for=\"turnSpeed\">Turn Speed</label>");
        client.println("<label id=\"turn\">100</label>");
        client.println("<input type=\"range\" min=\"0\" max=\"255\" value=\"100\" name=\"turnSpeed\" id=\"turnSpeed\" />");
        client.println("</div>");
        client.println("<div class=\"takeOrder\">");
        client.println("<button id=\"takeOrder\">Take Order</button>");
        client.println("<button id=\"comeBack\">Come Back</button>");
        client.println("</div>");
        client.println("<div class=\"goToTable\">");
        client.println("<label for=\"table\">Go to Table</label>");
        client.println("<input type=\"number\" id=\"table\" name=\"table\" min=\"1\" placeholder=\"Enter table number\" />");
        client.println("<button id=\"go\">Go</button>");
        client.println("</div>");
        client.println("<script>");
        client.println("function sendRequest(action) {");
        client.println("var xhr = new XMLHttpRequest();");
        client.println("xhr.open(\"GET\", action, true);");
        client.println("xhr.send();");
        client.println("}");
        client.println("document.getElementById(\"forward\").onmousedown = function () {");
        client.println("sendRequest(\"/F\");");
        client.println("};");
        client.println("document.getElementById(\"forward\").onmouseup = function () {");
        client.println("sendRequest(\"/S\");");
        client.println("};");
        client.println("document.getElementById(\"backward\").onmousedown = function () {");
        client.println("sendRequest(\"/B\");");
        client.println("};");
        client.println("document.getElementById(\"backward\").onmouseup = function () {");
        client.println("sendRequest(\"/S\");");
        client.println("};");
        client.println("document.getElementById(\"left\").onmousedown = function () {");
        client.println("sendRequest(\"/L\");");
        client.println("};");
        client.println("document.getElementById(\"left\").onmouseup = function () {");
        client.println("sendRequest(\"/S\");");
        client.println("};");
        client.println("document.getElementById(\"right\").onmousedown = function () {");
        client.println("sendRequest(\"/R\");");
        client.println("};");
        client.println("document.getElementById(\"right\").onmouseup = function () {");
        client.println("sendRequest(\"/S\");");
        client.println("};");
        client.println("document.getElementById(\"speed\").oninput = function () {");
        client.println("document.getElementById(\"spd\").innerHTML = this.value;");
        client.println("sendRequest(\"/SP\" + this.value);");
        client.println("};");
        client.println("document.getElementById(\"turnSpeed\").oninput = function () {");
        client.println("document.getElementById(\"turn\").innerHTML = this.value;");
        client.println("sendRequest(\"/T\" + this.value);");
        client.println("};");
        client.println("document.getElementById(\"go\").onclick = function () {");
        client.println("var tableNumber = document.getElementById(\"table\").value;");
        client.println("if (tableNumber) {");
        client.println("sendRequest(\"/TB\" + tableNumber);");
        client.println("} else {");
        client.println("alert(\"Please enter a table number.\");");
        client.println("}");
        client.println("};");
        client.println("document.getElementById(\"takeOrder\").onclick = function () {");
        client.println("sendRequest(\"/TO\");");
        client.println("};");
        client.println("document.getElementById(\"comeBack\").onclick = function () {");
        client.println("sendRequest(\"/CB\");");
        client.println("};");
        client.println("</script>");
        client.println("</body>");
        client.println("</html>");
    }
}