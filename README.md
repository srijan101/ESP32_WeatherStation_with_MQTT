# ESP32 Weather Station with MQTT

This project utilizes an ESP32 to collect weather data from sensors and log it to a ThingSpeak channel. It integrates Wi-Fi and employs light sleep mode for power efficiency between data cycles.

## Features
- **Wi-Fi Connectivity**: Connects to a specified Wi-Fi network.
- **Weather Monitoring**: Reads data from multiple sensors:
  - Light (lux)
  - Temperature and Humidity
  - Pressure and Altitude
  - Microphone Level
- **MQTT Integration**: Publishes sensor data to a ThingSpeak channel.
- **Power Efficiency**: Uses light sleep mode to conserve power between updates.

## Hardware Requirements
- ESP32 board
- Sensors:
  - Lux sensor
  - Temperature and humidity sensor
  - BMP280 (pressure and altitude sensor)
  - KY-037 (microphone sensor)
- Wi-Fi network for connectivity

## Setup
1. Clone the repository:
   ```bash
   git clone https://github.com/srijan101/ESP32_WeatherStation_with_MQTT.git
   cd ESP32_WeatherStation_with_MQTT
