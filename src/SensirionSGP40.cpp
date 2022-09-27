#ifdef SENSORS

#include "defaults.h"
#include <AsyncWiFiSettings.h>
#include "Adafruit_SGP40.h"
#include "strings.h"
#include "SensirionSGP40.h"

#include "globals.h"
#include "mqtt.h"
#include "string_utils.h"


namespace SensirionSGP40
{
    int sckPin = 0;
    int doutPin = 0;
    int sensorInterval = 30000;
    String SGP40_I2c;
    unsigned long lastMillis = 0;
    int SGP40_I2c_Bus;
    Adafruit_SGP40 sgp;
    bool initialized = false;

    void Setup()
    {
         bool success = false;
    
        if (!I2C_Bus_1_Started && !I2C_Bus_2_Started && (SGP40_I2c != "0x59")) return;
            
       
        if (SGP40_I2c_Bus == 1)
        {
            success = sgp.begin(&Wire);
        }
        else if (SGP40_I2c_Bus == 2)
        {
            success = sgp.begin(&Wire1);
        }
        if (!success)
        {
            Serial.println("SGP40 not found");
        }
        else
        {
            initialized = true;
            Serial.print("SGP40 serial #");
            Serial.print(sgp.serialnumber[0], HEX);
            Serial.print(sgp.serialnumber[1], HEX);
            Serial.println(sgp.serialnumber[2], HEX);
        }
    }

    void ConnectToWifi()
    {
        AsyncWiFiSettings.html("h4", "SGP40 Air Quality Sensor");
        SGP40_I2c_Bus = AsyncWiFiSettings.integer("SGP40_I2c_Bus", 1, 2, DEFAULT_I2C_BUS, "I2C Bus");
        SGP40_I2c = AsyncWiFiSettings.string("SGP40_I2c", "", "I2C address (0x59)");
        sensorInterval = AsyncWiFiSettings.integer("Sensor Update Interval", 0, 1>>15, 30000, "Update Inteval");
    }

    void SerialReport()
    {
        if (!I2C_Bus_1_Started && !I2C_Bus_2_Started) return;
        if (SGP40_I2c.isEmpty()) return;
        Serial.print("SGP40: ");
        Serial.println("on bus" + String(SGP40_I2c_Bus));
    }

    void Loop(float temperature=20, float humidity=50)
    {
        if (!I2C_Bus_1_Started && !I2C_Bus_2_Started) return;
        if (!initialized) return;
        if (millis() - lastMillis < sensorInterval) return;

        uint16_t vocraw = sgp.measureRaw(temperature, humidity);
        int32_t vocIndex = sgp.measureVocIndex(temperature, humidity);
        mqttClient.publish((roomsTopic + "/voc_raw").c_str(), 0, true, String(vocraw).c_str());
        mqttClient.publish((roomsTopic + "/voc").c_str(), 0, true, String(vocIndex).c_str());
        lastMillis = millis();
    }

    

    bool SendDiscovery()
    {
        if (SGP40_I2c.isEmpty()) return true;
        return sendSensorDiscovery("VOC Raw", EC_NONE, "volatile_organic_compounds_index_raw") && sendSensorDiscovery("VOC", EC_NONE, "volatile_organic_compounds_index");
    }
}

#endif // SENSORS