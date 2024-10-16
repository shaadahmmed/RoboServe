#ifndef SETUP_H
#define SETUP_H

#include <WiFi.h>
#include <WebServer.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include "display.h"
#include <queue>

using namespace std;

WebServer server(80);

bool isCredentialExist();
void startAP();
void setupRequest();
void handleSetupPost();
void handleFileRequest(String path);
String getContentType(String filename);
bool saveCredentials(String json);
bool connectToWiFi(String credentialJSON);
bool connectFromCredentials();
void setMenu();
void setCart();
void startClient();
void handlePlacedOrder();
void printSetupCredentials();
bool initialSDCard();
bool removeCredentials();
void handleAddTable();

const char *SSID = "Robo_Serve";
const char *PASSWORD = "admin1234";

queue<String> orderList;

void beginSetup()
{
  Wire.begin();
  Serial.begin(115200);
  if (isCredentialExist())
  {
    for (byte i = 1; !connectFromCredentials(); i++)
    {
      displayClear();
      displayPrint("Failed to connect to WiFi", 0);
      displayPrint("Retrying", 1);
      displayPrint("Attempt " + String(i + 1), 2);
      delay(1000);
      if (i == 5)
      {
        displayClear();
        displayPrint("Failed to connect", 0);
        delay(1000);
        displayPrintCenter("Rebooting...", 1);
        delay(5000);
        ESP.restart();
      }
    }
    displayClear();
    startClient();
    displayPrintCenter("Server Started", 1);
    displayPrintCenter(WiFi.localIP().toString(), 2);
    return;
  }
  startAP();
  setupRequest();
  server.begin();
}

void startAP()
{
  displayClear();
  displayPrintCenter("Starting Setup", 1);
  delay(2000);
  displayClear();
  WiFi.softAP(SSID, PASSWORD);
  printSetupCredentials();
}

void setupRequest()
{
  server.on("/", HTTP_GET, []()
            { handleFileRequest("/setup/setup.html"); });
  server.on("/setup.css", HTTP_GET, []()
            { handleFileRequest("/setup/setup.css"); });
  server.on("/setup.js", HTTP_GET, []()
            { handleFileRequest("/setup/setup.js"); });
  server.on("/submit", HTTP_POST, []()
            { handleSetupPost(); });
}

void handleSetupPost()
{
  displayClear();
  if (!server.hasArg("plain"))
  {
    displayPrint("Invalid request...", 0);
    server.send(404, "text/plain", "Invaild Request");
    return;
  }
  String credentials = server.arg("plain");

  if (!connectToWiFi(credentials))
  {
    server.send(404, "text/plain", "wrong credential");
    displayClear();
    displayPrint("Failed to connect", 0);
    displayPrint("Check your credentials", 1);
    displayPrint("Try Again...", 2);
    delay(2000);
    printSetupCredentials();
    return;
  }
  if (!saveCredentials(credentials))
  {
    server.send(404, "text/plain", "credentials not saved");
    displayClear();
    displayPrint("Failed to save credentials", 0);
    displayPrint("Check your SD Card", 1);
    displayPrint("Rebooting...", 2);
    delay(2000);
    ESP.restart();
  }
  delay(2000);
  server.send(200, "text/plain", WiFi.localIP().toString());
  delay(5000);
  WiFi.softAPdisconnect(true);
  ESP.restart();
}

void handleFileRequest(String path)
{
  if (SD.exists(path))
  {
    File file = SD.open(path, "r");
    server.streamFile(file, getContentType(path));
    file.close();
  }
  else
  {
    server.send(404, "text/plain", "File not found");
  }
}

String getContentType(String filename)
{
  if (filename.endsWith(".htm") || filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else
    return "text/plain";
}

bool saveCredentials(String json)
{
  File file = SD.open("/setup/credentials.json", FILE_WRITE);
  if (!file)
  {
    displayPrint("Failed to save credentials", 0);
    return false;
  }
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, json);
  if (!error)
  {
    doc["server"] = WiFi.localIP().toString();
    doc["subnet"] = WiFi.subnetMask().toString();
    doc["gateway"] = WiFi.gatewayIP().toString();
    file.println(doc.as<String>());
    file.close();
    displayPrint("Credentials saved", 3);
    return true;
  }
  displayClear();
  displayPrintCenter("Credential", 0);
  displayPrintCenter("Corrupted", 1);
  delay(2000);
  displayPrintCenter("Rebooting...", 2);
  delay(500);
  ESP.restart();
  return false;
}

bool isCredentialExist()
{
  return SD.exists("/setup/credentials.json");
}

bool connectToWiFi(String credentialJSON)
{
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, credentialJSON);
  if (error)
  {
    displayClear();
    displayPrint("Connection Failed", 0);
    displayPrint("Corrupted Credentials", 1);
    server.send(404, "text/plain", "Invalid JSON");
    return false;
  }
  String ssid = doc["ssid"];
  String password = doc["password"];
  IPAddress ipAddr, subnet, gateway;
  bool firstSetup = true;
  if (doc["ip"].is<String>() && doc["subnet"].is<String>() && doc["gateway"].is<String>())
  {
    ipAddr.fromString(doc["ip"].as<String>());
    subnet.fromString(doc["subnet"].as<String>());
    gateway.fromString(doc["gateway"].as<String>());
    WiFi.config(ipAddr, gateway, subnet);
    firstSetup = false;
  }

  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long startTime = millis();
  unsigned long timeout = 15000;
  displayClear();
  displayPrint("Connecting", 0);
  displayPrint(ssid, 1);
  displayPrint(password, 2);
  if (!firstSetup)
  {
    displayPrint(ipAddr.toString(), 3);
  }
  byte displayColumn = 9;
  displaySetCursor(10, 0);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    displayColumn++;
    if (displayColumn > 19)
    {
      displaySetCursor(10, 0);
      displayPrint("          ");
      displaySetCursor(10, 0);
      displayColumn = 9;
    }
    displayPrint(".");

    if (millis() - startTime >= timeout)
    {
      displayClear();
      WiFi.disconnect();
      server.send(404, "text/plain", "Connection Timeout");
      return false;
    }
  }
  displayOverrideRow("Connected", 0);
  displayOverrideRow(WiFi.localIP().toString(), 3);
  WiFi.mode(WIFI_STA);
  return true;
}

bool connectFromCredentials()
{
  File file = SD.open("/setup/credentials.json", FILE_READ);
  if (!file)
    return false;
  String credential = file.readStringUntil('\n');
  Serial.println(credential);
  file.close();
  return connectToWiFi(credential);
}

void printSetupCredentials()
{
  displayClear();
  displayPrint("RoboServe Setup", 0);
  displayPrint(WiFi.softAPIP().toString(), 1);
  displayPrint(String(SSID), 2);
  displayPrint(String(PASSWORD), 3);
}

bool initialSDCard()
{
  if (!SD.begin(5))
  {
    displayPrintCenter("SD Card failed", 0);
    displayPrintCenter("Check Your SD Card", 1);
    delay(2000);
    displayPrintCenter("Rebooting", 2);
    delay(500);
    for (int i = 0; i < 3; i++)
    {
      displayPrint(".");
      delay(500);
    }
    ESP.restart();
    return false;
  }
  displayPrint("SD Card initialized", 1);
  return true;
}

bool removeCredentials()
{
  if (SD.remove("/setup/credentials.json"))
  {
    displayClear();
    displayPrint("Credentials removed", 1);
    displayPrint("Rebooting...", 2);
    delay(2000);
    displayClear();
    ESP.restart();
    return true;
  }
  displayClear();
  ESP.restart();
  return false;
}

void startClient()
{
  setMenu();
  setCart();
  server.begin();
}

void setMenu()
{
  server.on("/", HTTP_GET, []()
            { Serial.println("Menu");
            Serial.println(SD.exists("/client/menu/menu.html"));
  handleFileRequest("/client/menu/menu.html"); });
  server.on("/desktop.css", HTTP_GET, []()
            { handleFileRequest("/client/menu/desktop.css"); });
  server.on("/phone.css", HTTP_GET, []()
            { handleFileRequest("/client/menu/phone.css"); });
  server.on("/menu.js", HTTP_GET, []()
            { handleFileRequest("/client/menu/menu.js"); });
  server.on("/table", HTTP_GET, []()
            { handleFileRequest("/client/menu/table.html"); });
  server.on("/table.css", HTTP_GET, []()
            { handleFileRequest("/client/menu/table.css"); });
  server.on("/addTable", HTTP_POST, []()
            { handleAddTable(); });
}
void setCart()
{
  server.on("/cart.html", HTTP_GET, []()
            { handleFileRequest("/client/cart/cart.html"); });
  server.on("/cartDesktop.css", HTTP_GET, []()
            { handleFileRequest("/client/cart/cartDesktop.css"); });
  server.on("/cartPhone.css", HTTP_GET, []()
            { handleFileRequest("/client/cart/cartPhone.css"); });
  server.on("/cart-script.js", HTTP_GET, []()
            { handleFileRequest("/client/cart/cart-script.js"); });
  server.on("/order_placed", HTTP_POST, []()
            { handlePlacedOrder(); });
}

void handleAddTable()
{
  String table = server.arg("plain");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, table);
  if (error)
  {
    server.send(404, "text/plain", "Invalid JSON");
    return;
  }
  String tableNo = doc["tableNo"].as<String>();
  String tablePath = doc["tableData"].as<String>();
  if (SD.exists("/table/" + tableNo + ".txt"))
  {
    server.send(200, "text/plain", "Table already exists");
    return;
  }
  File tableData = SD.open("/table/" + tableNo + ".txt", FILE_WRITE);
  if (!tableData)
  {
    server.send(404, "text/plain", "Failed to add table");
    return;
  }
  tableData.println(tablePath);
  displayClear();
  displayPrintCenter("Table Added", 1);
  displayPrintCenter(tableNo, 2);
  delay(2000);
  server.send(200, "text/plain", "success");
}

void handlePlacedOrder()
{

  String order = server.arg("plain");
  displayClear();
  displayPrintCenter("New Order", 1);
  displayPrintCenter(order, 2);
  orderList.push(order);
  server.send(200, "text/plain", "Order received");
}

#endif
