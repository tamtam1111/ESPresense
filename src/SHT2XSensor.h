#ifdef SENSORS

#pragma  once

#include <ArduinoJson.h>

// Forward declares
class AsyncMqttClient;

namespace SHT2XSensor
{
    void Setup();
    void ConnectToWifi();
    void SerialReport();
    void Loop();
    bool SendDiscovery();
    float getTemperature();
    float getHumidity();
}

#endif // SENSOR