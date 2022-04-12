#include "TemperatureLib.h"

TemperatureLib::TemperatureLib(uint8_t sensors_pin)
{
    _ow = OneWire(sensors_pin);
    _sensors.setOneWire(&_ow);
    _sensors.setResolution(TEMPERATURE_SENSOR_PRECISION);
    _sensors.begin();

    Serial.println("========================");
    Serial.println("Dallas Temperature Sensors Initialize");
    Serial.println("========================");

    Serial.print("Rozdzielczość: ");
    Serial.println(_sensors.getResolution());

    Serial.print("Wykrytych urządzeń: ");
    Serial.println(_sensors.getDeviceCount());

    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
    Serial.print("Requesting temperatures...");
    _sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.println("DONE");

    // After we got the temperatures, we can print them here.
    // We use the function ByIndex, and as an example get the temperature from the first sensor only.
    for ( int DeviceIndex = 0; DeviceIndex < _sensors.getDeviceCount(); DeviceIndex++ )
    {
        float tempC = _sensors.getTempCByIndex(DeviceIndex);

        // Check if reading was successful
        if (tempC != DEVICE_DISCONNECTED_C)
        {
            Serial.print("Temperature for the device ");
            Serial.print(DeviceIndex);
            Serial.print(" is: ");
            Serial.println(tempC, TEMPERATURE_SENSOR_DISMENSION);
        }
        else
        {
            Serial.println("Error: Could not read temperature data");
        }
    }
}

void TemperatureLib::refresh()
{
    _sensors.requestTemperatures();
}

float TemperatureLib::getTemperature(const uint8_t *deviceAddress)
{
    float tempC = _sensors.getTempC(deviceAddress);
    return tempC;
}

String TemperatureLib::getFormatedTemperature(const uint8_t *deviceAddress)
{
    float tempC = _sensors.getTempC(deviceAddress);
    return String(tempC, TEMPERATURE_SENSOR_DISMENSION) + "\337";
}

bool TemperatureLib::isLowTemperature(float temp)
{
    if (temp <= targetTemperature - hysteresis)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool TemperatureLib::isHighTemperature(float temp)
{
    if (temp >= targetTemperature + hysteresis)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool TemperatureLib::isNormalTemperature(float temp)
{
    if (temp < targetTemperature + hysteresis && temp > targetTemperature - hysteresis)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void TemperatureLib::Search()
{
    Serial.println("========================");
    Serial.println("Dallas Temperature Sensor Log");
    Serial.println("========================");

}

DallasTemperature TemperatureLib::GetDT()
{
    return _sensors;
}