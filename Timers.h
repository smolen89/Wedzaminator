#ifndef _Timers_h
    #define _Timers_h

    #include <Arduino.h>
    #include <inttypes.h>
    #define MILIS(t) (unsigned long) (t)
    #define SECS(t) (unsigned long) (t * 1000)
    #define MINS(t) SECS(t) * 60
    #define HOURS(t) MINS(t) * 60
    #define STOP 0

    class Timer
    {
    private:
        uint32_t _time;
        uint32_t _lastTime;

    public:
        void begin(const uint32_t);
        void restart();
        bool available();
        void time(const uint32_t);
        void add(const uint32_t);
        uint32_t time();
    };

#endif