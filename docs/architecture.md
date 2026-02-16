# System Architecture

Baikonur Eco AI Monitoring System MVP architecture.

## Components

Sensors:
- DHT11 (temperature and humidity)
- HC-SR04 (dust container level)
- SR505 PIR (motion detection)
- HSD (sound level)
- HYDZ (rain/water detection)

Controller:
ESP32 DevKit V1

Communication:
WiFi → Telegram Bot API

AI Layer:
Rule-based environmental risk evaluation.

User Interface:
Telegram Bot.

## Data Flow

Sensors → ESP32 → Telegram Bot → AI Risk Analysis → User

## Purpose

Demonstrate an IoT-based ecological monitoring system prototype
for the Baikonur Cosmodrome environment.
