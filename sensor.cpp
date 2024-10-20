#include "sensor.h"

#define DHTPIN 4         // Pin connected to DHT11 sensor
#define DHTTYPE DHT11    // DHT 11
#define MICPIN 36        // KY-037 connected to GPIO 36 (ADC pin)

Sensor::Sensor() : dht(DHTPIN, DHTTYPE), micPin(MICPIN) {}

void Sensor::init() {
    // Start I2C for BMP280 and BH1750
    Wire.begin(21, 22); // Adjust if SDA and SCL are on different pins

    // Initialize BH1750
    if (!luxSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        Serial.println("[BH1750] ERROR: Initialization failed");
    }

    // Initialize DHT11
    dht.begin();

    // Initialize BMP280
    if (!bmp280.begin(0x76)) {  // 0x76 is the default I2C address for BMP280
        Serial.println("[BMP280] ERROR: Initialization failed");
    }
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

float Sensor::readPressure() {
    return bmp280.readPressure();
}

float Sensor::readAltitude() {
    return bmp280.readAltitude(1013.25); // Adjust to your location's sea-level pressure
}

int Sensor::readMicLevel() {
    return analogRead(micPin); // Read the analog value from KY-037
}
