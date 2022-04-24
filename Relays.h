#ifndef _Relays_h

    #define _Relays_h
    #include <Arduino.h>

    class Relay
    {
        private:
            byte _relay_pin_fan;
            byte _relay_pin_smoke;
            byte _relay_pin_hot1;
            byte _relay_pin_hot2;
            bool _smokeState;
            bool _fanState;
            bool _hot1State;
            bool _hot2State;
        
        public:
            Relay(byte relay_pin_fan, byte relay_pin_smoke, byte relay_pin_hot1, byte relay_pin_hot2);

            void Initialize();
            void TurnOffAll();
            void TurnOnAll();

            bool GetSmokeState();
            void SmokeOn();
            void SmokeOff();
            void SmokeChange();

            bool GetFanState();
            void FanOn();
            void FanOff();
            void FanChange();

            void HotAllOn();
            void HotAllOff();
            void HotAllChange();

            bool GetHot1State();
            void Hot1On();
            void Hot1Off();
            void Hot1Change();
            
            bool GetHot2State();
            void Hot2On();
            void Hot2Off();
            void Hot2Change();
    };

#endif