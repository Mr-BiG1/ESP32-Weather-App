# ESP32 Weather App ☀️🌧️

An ESP32-based weather station that displays real-time weather data on a 3.2" ILI9341 TFT screen. It fetches data from the **OpenWeatherMap API**, shows animated weather icons, and includes a web interface for updating your **city name** and **API key** on the fly.

## 🚀 Features

- 🌡️ **Live Weather Data**  
  Displays temperature, feels-like temperature, humidity, pressure, wind speed, and visibility.

- 🖼️ **TFT Display (ILI9341)**  
  Clear and vibrant weather info with condition-based icons.

- 🌐 **Built-in Web Server**  
  Easily update your **city** and **API key** via browser — no code upload required.

- 🕒 **NTP Time Sync**  
  Displays accurate local time using Network Time Protocol.

- 💾 **EEPROM Storage**  
  Saves your city and API key even after power loss.

---

## 🧰 Hardware Requirements

- ✅ ESP32 (tested on ESP32-S3)
- ✅ 3.2" ILI9341 TFT SPI Display
- ✅ Wi-Fi connection
- (Optional) Touch support, buzzer, or RTC module for future expansion

---

## 💻 Software Requirements

- **Arduino IDE** with ESP32 board support  
  [Install instructions](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)

- **Libraries to install:**
  - `WiFi`
  - `HTTPClient`
  - `ArduinoJson`
  - `Adafruit_GFX`
  - `Adafruit_ILI9341`
  - `WebServer` *(ESP32 version)*
  - `EEPROM`
  - `NTPClient`
  - `WiFiUdp`

---

## 🔧 Setup Instructions

1. **Clone this repo:**
   ```bash
   git clone https://github.com/your-username/esp32-weather-app.git
   cd esp32-weather-app
   ```

2. **Open the `.ino` file** in Arduino IDE.

3. **Install all required libraries** via Library Manager or manually.

4. **Connect your ESP32**, select the correct board and COM port.

5. **Flash the code**, then connect to the ESP32 web server and enter:
   - Your **City Name**
   - Your **OpenWeatherMap API Key**

---

## 🌍 Web Interface

After flashing, connect the ESP32 to Wi-Fi and visit the IP shown on the TFT screen (e.g., `http://192.168.0.105`) in your browser to access the settings page.

---

## 📦 File Structure

```
esp32-weather-app/
├── esp32-weather-app.ino
├── icons/                  # Weather icons (stored in SPIFFS)
├── data/                   # For SPIFFS upload if used
├── README.md
```

---

## 📡 API & Notes

- Sign up at [OpenWeatherMap](https://openweathermap.org/api) to get your **free API key**.
- API usage is subject to limits on free plans — use wisely!

---

## 🤝 Credits

- Weather icons inspired by OpenWeather & Adafruit examples  
- Special thanks to the ESP32 and Arduino communities!

---

## 📜 License

MIT License [LICENSE]. Feel free to use and modify for your own projects!
