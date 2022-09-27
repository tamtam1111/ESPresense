#ifdef SENSORS

#pragma  once

#include <ArduinoJson.h>

// Forward declares
class AsyncMqttClient;

namespace SensirionSGP40
{
    void Setup();
    void ConnectToWifi();
    void SerialReport();
    void Loop(float temperature, float humidity);
    bool SendDiscovery();
}

#endif // SENSOR