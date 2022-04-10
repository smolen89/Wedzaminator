#include "TemperatureLib.h"

TemperatureLib::TemperatureLib(uint8_t sensors_pin)
{
    _ow = OneWire(sensors_pin);
    _sensors.setOneWire(&_ow);
    _sensors.setResolution(TEMPERATURE_SENSOR_PRECISION);
    _sensors.begin();
    refresh();
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