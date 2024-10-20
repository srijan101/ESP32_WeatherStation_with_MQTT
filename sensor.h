#include "sensor.h"

#define DHTPIN 4         // Pin connected to DHT11 sensor
#define DHTTYPE DHT11    // DHT 11

Sensor::Sensor() : dht(DHTPIN, DHTTYPE) {}

void Sensor::init() {
    // Start I2C
    Wire.begin(21, 22); // Adjust if SDA and SCL are on different pins

    // Initialize BH1750
    if (!luxSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        Serial.println("[BH1750] ERROR: Initialization failed");
    }

    // Initialize DHT11
    dht.begin();
}

float Sensor::readLux() {
    return luxSensor.readLightLevel();
}

float Sensor::readTemperature() {
    return dht.readTemperature();
}

float Sensor::readHumidity() {
    return dht.readHumidity();
}
