#include <WiFi.h>
#include <ThingSpeak.h>
#include "esp_sleep.h"
#include "sensor.h"

// Wi-Fi configurations
const char* ssid = "Wi-Fi SSID.4G";          // Wi-Fi SSID
const char* password = "Wi-Fi Password";           // Wi-Fi Password

// ThingSpeak configurations
unsigned long myChannelNumber =  Your ThingSpeak channel ID;        // Your ThingSpeak channel ID
const char* myWriteAPIKey = " Your ThingSpeak Write API Key"; // Your ThingSpeak Write API Key

WiFiClient espClient;

// Create Sensor object
Sensor sensor;

const uint64_t TIME_TO_SLEEP = 55; // 60 seconds target interval

void setup() {
    Serial.begin(115200);
    
    // Initialize the sensors
    sensor.init();

    // Connect to Wi-Fi
    connectToWiFi();

    // Initialize ThingSpeak
    ThingSpeak.begin(espClient);
}

void loop() {
    // Record the start time
    unsigned long startTime = millis();

    // Ensure Wi-Fi is connected before publishing
    if (WiFi.status() != WL_CONNECTED) {
        connectToWiFi();  // Reconnect if disconnected
    }

    // Read sensor data
    float lux = sensor.readLux();
    float temperature = sensor.readTemperature();
    float humidity = sensor.readHumidity();
    float pressure = sensor.readPressure();      // Read BMP280 pressure
    float altitude = sensor.readAltitude();      // Read BMP280 altitude
    int micLevel = sensor.readMicLevel();        // Read KY-037 mic level

    // Publish data to ThingSpeak
    publishSensorData(lux, temperature, humidity, pressure, altitude, micLevel);

    // Measure elapsed time for reading and publishing
    unsigned long elapsedTime = millis() - startTime;
    Serial.println("Elapsed time for reading and publishing: " + String(elapsedTime / 1000) + " seconds");

    // Add delay before sleeping (to ensure everything is stable)
    delay(2000); // 2-second delay to ensure Wi-Fi and publishing is complete

    // If the process took less than 60 seconds, sleep for the remaining time
    if (elapsedTime < TIME_TO_SLEEP * 1000) {
        unsigned long sleepTime = (TIME_TO_SLEEP * 1000) - elapsedTime;
        enterLightSleep(sleepTime / 1000); // Convert to seconds
    } else {
        Serial.println("The process took longer than 60 seconds, adjusting the next cycle.");
    }
}

// Connect to Wi-Fi
void connectToWiFi() {
    Serial.print("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWi-Fi connected");
}

// Publish sensor data to ThingSpeak
void publishSensorData(float lux, float temperature, float humidity, float pressure, float altitude, int micLevel) {
    // Set field values
    ThingSpeak.setField(1, lux);
    ThingSpeak.setField(2, temperature);
    ThingSpeak.setField(3, humidity);
    ThingSpeak.setField(4, pressure);
    ThingSpeak.setField(5, altitude);
    ThingSpeak.setField(6, micLevel);

    // Write to ThingSpeak
    int responseCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if (responseCode == 200) {
        Serial.println("Data published successfully");
    } else {
        Serial.print("Error publishing data, HTTP error code: ");
        Serial.println(responseCode);
    }
}

// Enter light sleep for specified seconds
void enterLightSleep(uint64_t sleepSeconds) {
    Serial.println("Entering light sleep for " + String(sleepSeconds) + " seconds");

    // Calculate sleep time in microseconds
    uint64_t sleepTimeMicroSeconds = sleepSeconds * 1000000;

    // Configure ESP32 for timer wake-up
    esp_sleep_enable_timer_wakeup(sleepTimeMicroSeconds);

    // Enter light sleep
    esp_light_sleep_start(); 

    // Upon wake-up, ESP32 will continue from here
    Serial.println("Woke up from light sleep");
}
