# ESP32 Weather App

This is an ESP32-based weather app that displays real-time weather data on a TFT display. It fetches data from the OpenWeatherMap API and includes a web server for updating settings.

## Features
- **Real-time weather data**: Temperature, feels-like temperature, humidity, pressure, wind speed, and visibility.
- **TFT display**: Shows weather information with icons for different conditions.
- **Web server**: Allows updating the city and API key via a web interface.
- **NTP integration**: Displays the current time.

## Hardware Requirements
- ESP32 microcontroller
- ILI9341 TFT display
- Wi-Fi connection

## Software Requirements
- Arduino IDE
- Libraries:
  - `WiFi`
  - `HTTPClient`
  - `ArduinoJson`
  - `Adafruit_GFX`
  - `Adafruit_ILI9341`
  - `WebServer`
  - `EEPROM`
  - `NTPClient`

## Setup
1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/your-repo-name.git
