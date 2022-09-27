#ifdef SENSORS


#include <AsyncWiFiSettings.h>
#include "SHT2x.h"
#include "strings.h"
#include "SHT2XSensor.h"
#include "defaults.h"
#include "globals.h"
#include "mqtt.h"
#include "string_utils.h"



namespace SHT2XSensor
{

    int sckPin = 0;
    int doutPin = 0;
    int sensorInterval = 1000;
    unsigned long lastMillis = 0;
    int SHT2x_I2c_Bus;
    String SHT2x_I2c;
    SHT2x sht;
    float h=40.;
    float t=20.;

    void Setup()
    {
        bool success = false;
         if (!I2C_Bus_1_Started && !I2C_Bus_2_Started && SHT2x_I2c.isEmpty()) return;
         

        if (SHT2x_I2c_Bus == 1)
        {
            success = sht.begin(&Wire);
        }
        else if (SHT2x_I2c_Bus == 2)
        {
            success = sht.begin(&Wire1);
        }
        if (!success)
        {
            Serial.println("SHT2x not found");
        }
        else
        {
            Serial.println("Found SHT2x ");
        }
    }

    void ConnectToWifi()
    {
        AsyncWiFiSettings.html("h4", "SHT2x - Temperature and Humidity Sensor:");
        SHT2x_I2c_Bus = AsyncWiFiSettings.integer("SHT2x_I2c_Bus", 1, 2, DEFAULT_I2C_BUS, "I2C Bus");
        SHT2x_I2c = AsyncWiFiSettings.string("SHT2x_I2c", "", "I2C address (0x40)");
    }

    void SerialReport()
    {
        if (!I2C_Bus_1_Started && !I2C_Bus_2_Started) return;
           
        Serial.print("SHT2x Sensor: ");
        Serial.println(SHT2x_I2c + " on bus " + SHT2x_I2c_Bus);
    }

    void Loop()
    {
        if (!I2C_Bus_1_Started && !I2C_Bus_2_Started) return;

        if (millis() - lastMillis < sensorInterval)
            return;
        if (!sht.isConnected())
            return;
        bool b = sht.read();
        if (!b) return;
        t = sht.getTemperature();
        h = sht.getHumidity();
        mqttClient.publish((roomsTopic + "/temperature").c_str(), 0, true, String(t).c_str());
        mqttClient.publish((roomsTopic + "/humidity").c_str(), 0, true, String(h).c_str());
        lastMillis = millis();
    }

 

    bool SendDiscovery()
    {
        if (SHT2x_I2c.isEmpty()) return true;

        return sendSensorDiscovery("temperature", EC_NONE, "temperature", "°C")
            && sendSensorDiscovery("humidity", EC_NONE, "humidity", "%");
    }

    float getTemperature()
    {
        return t;
    }
    float getHumidity()
    {
        return h;
    }
}

#endif // SENSORS
