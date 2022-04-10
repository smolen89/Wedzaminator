#ifndef _TemperatureLib_h
    #define _TemperatureLib_h

    #include <Arduino.h>
    #include <OneWire.h>
    #include <DallasTemperature.h>

    // Device resolution
    // #define TEMP_9_BIT  0x1F //  9 bit
    // #define TEMP_10_BIT 0x3F // 10 bit
    // #define TEMP_11_BIT 0x5F // 11 bit
    // #define TEMP_12_BIT 0x7F // 12 bit
    #define TEMPERATURE_PRECISION 0x3F

    // Resolution:
    //    9 - Temperature range: xxx.0C    to xxx.5C    in 0.5C    discrete step.
    //   10 - Temperature range: xxx.00C   to xxx.75C   in 0.25C   discrete step.
    //   11 - Temperature range: xxx.000C  to xxx.875C  in 0.125C  discrete step.
    //   12 - Temperature range: xxx.0000C to xxx.9375C in 0.0625C discrete step.
    #define TEMPERATURE_SENSOR_PRECISION 9
    #define TEMPERATURE_SENSOR_DISMENSION 1


    class TemperatureLib
    {
        public:
            // Constructor
            TemperatureLib(uint8_t sensors_pin);

            uint8_t targetTemperature;
            uint8_t hysteresis;
            float getTemperature(const uint8_t*);
            void refresh(void);
            String getFormatedTemperature(const uint8_t*);
            bool isLowTemperature(float);
            bool isNormalTemperature(float);
            bool isHighTemperature(float);
            void Search();

            

        private:
            OneWire _ow;
            DallasTemperature _sensors;
    };
#endif