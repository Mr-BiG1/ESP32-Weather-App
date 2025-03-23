#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// WiFi
const char* ssid = "Ancysibi";
const char* password = "121Ryansibi";

// OpenWeatherMap
char city[32] = "Toronto"; 
char apiKey[64] = "3ec28e5011fbdd0782c240b9aceebd8d"; // Replace with your OpenWeatherMap API key

// TFT Pins
#define TFT_CS   10
#define TFT_DC   12
#define TFT_RST  11
#define TFT_MOSI 13
#define TFT_CLK  14

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Web Server
WebServer server(80);

// EEPROM
#define EEPROM_SIZE 128

// NTP Client for time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -14400, 60000); // GMT-4 (Toronto time)

// Weather Icons
const uint16_t sunColor = ILI9341_YELLOW;
const uint16_t cloudColor = ILI9341_LIGHTGREY;
const uint16_t rainColor = ILI9341_BLUE;
const uint16_t snowColor = ILI9341_WHITE;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Init display
  SPI.begin(TFT_CLK, -1, TFT_MOSI, TFT_CS);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  tft.println("Connecting WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  tft.println("WiFi Connected");
  delay(500);

  // Initialize NTP client for time
  timeClient.begin();
  timeClient.update();

  // Display IP Address
  String ipAddress = WiFi.localIP().toString();
  tft.setCursor(10, 10);
  tft.println("IP: " + ipAddress);

  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);
  loadSettings();

  // Start Web Server
  server.on("/", handleRoot);
  server.on("/update", handleUpdate);
  server.begin();
  tft.println("Web Server Started");
}

void loop() {
  server.handleClient();
  timeClient.update();

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    char url[128];
    snprintf(url, sizeof(url), "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric", city, apiKey);
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println(payload);

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      const char* weather = doc["weather"][0]["main"];
      float temp = doc["main"]["temp"];
      float feelsLike = doc["main"]["feels_like"];
      int humidity = doc["main"]["humidity"];
      const char* description = doc["weather"][0]["description"];
      int pressure = doc["main"]["pressure"];
      float windSpeed = doc["wind"]["speed"];
      int visibility = doc["visibility"].as<int>() / 1000;

      String currentTime = timeClient.getFormattedTime();

      tft.fillScreen(ILI9341_BLACK);
      tft.drawRect(0, 0, tft.width(), tft.height(), ILI9341_WHITE);

      tft.setCursor(10, 10);
      tft.setTextSize(2);
      tft.setTextColor(ILI9341_CYAN);
      tft.print("Weather in ");
      tft.println(city);

      tft.setCursor(10, 40);
      tft.setTextSize(2);
      tft.setTextColor(ILI9341_WHITE);
      tft.print("Time: ");
      tft.println(currentTime);

      tft.setCursor(10, 70);
      tft.setTextSize(3);
      if (temp < 15) tft.setTextColor(ILI9341_BLUE);
      else if (temp > 28) tft.setTextColor(ILI9341_RED);
      else tft.setTextColor(ILI9341_YELLOW);
      tft.print(temp, 1);
      tft.println(" C");

      tft.setCursor(10, 110);
      tft.setTextSize(2);
      tft.setTextColor(ILI9341_ORANGE);
      tft.print("Feels: ");
      tft.print(feelsLike, 1);
      tft.println(" C");

      tft.setCursor(10, 140);
      tft.setTextSize(2);
      tft.setTextColor(ILI9341_GREEN);
      tft.print("Humidity: ");
      tft.print(humidity);
      tft.println("%");

      tft.setCursor(10, 170);
      tft.setTextSize(2);
      tft.setTextColor(ILI9341_WHITE);
      tft.print("Condition: ");
      tft.println(description);

      if (strcmp(weather, "Clear") == 0) drawSun(200, 50);
      else if (strcmp(weather, "Clouds") == 0) drawCloud(200, 50);
      else if (strcmp(weather, "Rain") == 0) drawRain(200, 50);
      else if (strcmp(weather, "Snow") == 0) drawSnow(200, 50);

      tft.setCursor(10, 220);
      tft.setTextSize(1);
      tft.setTextColor(ILI9341_WHITE);
      tft.print("IP: ");
      tft.println(ipAddress);
    } else {
      Serial.println("Error fetching weather");
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  delay(60000);
}

void handleRoot() {
  String html = "<html><body><h1>Weather Station Settings</h1>";
  html += "<form action='/update' method='POST'>";
  html += "City: <input type='text' name='city' value='" + String(city) + "'><br>";
  html += "API Key: <input type='text' name='apiKey' value='" + String(apiKey) + "'><br>";
  html += "<input type='submit' value='Update'></form></body></html>";
  server.send(200, "text/html", html);
}

void handleUpdate() {
  if (server.hasArg("city") && server.hasArg("apiKey")) {
    strncpy(city, server.arg("city").c_str(), sizeof(city) - 1);
    strncpy(apiKey, server.arg("apiKey").c_str(), sizeof(apiKey) - 1);
    city[sizeof(city) - 1] = '\0';
    apiKey[sizeof(apiKey) - 1] = '\0';
    saveSettings();
    server.send(200, "text/plain", "Settings Updated");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void loadSettings() {
  EEPROM.get(0, city);
  EEPROM.get(64, apiKey);
  city[sizeof(city) - 1] = '\0';
  apiKey[sizeof(apiKey) - 1] = '\0';
}

void saveSettings() {
  EEPROM.put(0, city);
  EEPROM.put(64, apiKey);
  EEPROM.commit();
}

void drawSun(int x, int y) {
  tft.fillCircle(x, y, 20, sunColor);
  for (int i = 0; i < 360; i += 45) {
    float angle = i * 3.14159 / 180;
    int x1 = x + 25 * cos(angle);
    int y1 = y + 25 * sin(angle);
    int x2 = x + 40 * cos(angle);
    int y2 = y + 40 * sin(angle);
    tft.drawLine(x1, y1, x2, y2, sunColor);
  }
}

void drawCloud(int x, int y) {
  tft.fillCircle(x - 15, y + 10, 15, cloudColor);
  tft.fillCircle(x + 15, y + 10, 15, cloudColor);
  tft.fillCircle(x, y, 20, cloudColor);
  tft.fillRect(x - 25, y, 50, 20, cloudColor);
}

void drawRain(int x, int y) {
  drawCloud(x, y);
  for (int i = 0; i < 3; i++) {
    tft.drawLine(x - 15 + i * 15, y + 30, x - 10 + i * 15, y + 50, rainColor);
  }
}

void drawSnow(int x, int y) {
  drawCloud(x, y);
  for (int i = 0; i < 3; i++) {
    tft.drawLine(x - 15 + i * 15, y + 30, x - 10 + i * 15, y + 50, snowColor);
    tft.drawLine(x - 10 + i * 15, y + 30, x - 15 + i * 15, y + 50, snowColor);
  }
}
