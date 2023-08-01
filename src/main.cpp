#include <Arduino.h>
#include <main.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

const char *ssid = "MyESP8266AP坤🐥";
const char *password = "88888888";
ESP8266WebServer server(80);

const int ledPin = 2;  // GPIO2

int ledBrightness = 255;

void handleRoot() {
  // // Allocate a temporary JsonDocument
  // // Use https://arduinojson.org/v6/assistant to compute the capacity.
  // StaticJsonDocument<200> doc;

  // // Create the "message" key
  // doc["message"] = "Hello, World!";

  // // Serialize the JsonDocument to a string
  // String jsonString;
  // serializeJson(doc, jsonString);

  // // Send the JSON response
  // server.send(200, "application/json", jsonString);

  String html = "<html><head>";
  html += "<style>";
  html += "body {";
  html += "  font-family: Arial, sans-serif;";
  html += "  text-align: center;";
  html += "}";
  html += "h1 {";
  html += "  color: #333;";
  html += "}";
  html += "input[type=range] {";
  html += "  width: 80%;";
  html += "  margin: 20px auto;";
  html += "}";
  html += "</style>";
  html += "</head><body>";

  html += "<h1>LED Control</h1>";

  html += "<p>Click <a href=\"/on\"><button>ON</button></a> to turn on the LED.</p>";
  html += "<p>Click <a href=\"/off\"><button>OFF</button></a> to turn off the LED.</p>";
  html += "<img src=\"/cute.gif\" alt=\"Cute GIF\">";
  html += "<img src=\"/cute.JPG\" alt=\"Cute JPG\">";
  html += "<p>LED Brightness: " + String(ledBrightness) + "</p>";
  html += "<input type=\"range\" min=\"0\" max=\"255\" value=\"" + String(ledBrightness) + "\" onchange=\"updateBrightness(this.value)\">";
  html += "<script> function updateBrightness(value) {";
  html += "var xhr = new XMLHttpRequest();";
  html += "xhr.open('GET', '/set?brightness=' + value, true);";
  html += "xhr.send();";
  html += "}</script>";

  html += "</body></html>";


  server.send(200, "text/html", html);
}

void handleOn() {
  // 在这里编写控制LED打开的代码
  digitalWrite(ledPin, LOW);  // 打开LED
  server.send(200, "text/plain", "LED turned on");
}

void handleOff() {
  // 在这里编写控制LED关闭的代码
  digitalWrite(ledPin, HIGH);   // 关闭LED
  server.send(200, "text/plain", "LED turned off");
}

void handleSet() {
  if (server.hasArg("brightness")) {
    ledBrightness = server.arg("brightness").toInt();
    analogWrite(ledPin, ledBrightness);
    server.send(200, "text/plain", "LED Brightness set to " + String(ledBrightness));
  }
}

void handleImage() {
  File imageFile = SPIFFS.open("/cute.gif", "r");
  if (imageFile) {
    server.streamFile(imageFile, "image/gif");
    imageFile.close();
  }
}

void setup(){
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  analogWriteRange(255);  // 设置PWM范围为0-255
  analogWrite(ledPin, ledBrightness);
  
  WiFi.softAP(ssid, password);
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Start the server
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/set", handleSet);
  server.on("/cute.gif", handleImage);
  server.begin();
}

void loop()
{
  server.handleClient();
}