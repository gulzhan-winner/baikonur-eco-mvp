# Launch Guide (How to Run)

This guide explains how to run the MVP.

## 1. Requirements
- ESP32 DevKit V1
- Arduino IDE installed
- WiFi network credentials
- Telegram Bot token (BotFather)
- Chat ID for notifications
- Connected sensors (optional for simulation)

## 2. Firmware Setup
1. Open the firmware file in Arduino IDE:
   firmware/esp32/esp32_bot_dht11.ino
2. Set WiFi credentials:
   - SSID
   - Password
3. Set Telegram credentials:
   - BOTtoken
   - CHAT_ID

## 3. Upload to ESP32
1. Connect ESP32 via USB
2. Select board: ESP32 Dev Module
3. Select the correct COM port
4. Click Upload

## 4. Telegram Test
1. Open the Telegram bot chat
2. Send: /start
3. Send: /status
4. The bot should reply with sensor readings and AI risk level.

## 5. Simulation Mode (when hardware is unavailable)
If sensors are not available, the project can be demonstrated using a local simulation script:
firmware/simulation/mvp_simulation.py

This produces sample sensor values and a calculated AI risk level.
