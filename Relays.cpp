#include "Relays.h"

Relay::Relay(byte relay_pin_fan, byte relay_pin_smoke, byte relay_pin_hot1, byte relay_pin_hot2)
{
    _relay_pin_fan = relay_pin_fan;
    _relay_pin_smoke = relay_pin_smoke;
    _relay_pin_hot1 = relay_pin_hot1;
    _relay_pin_hot2 = relay_pin_hot2;

    // Przypisanie Pinów
    pinMode(_relay_pin_fan, OUTPUT);
    pinMode(_relay_pin_smoke, OUTPUT);
    pinMode(_relay_pin_hot1, OUTPUT);
    pinMode(_relay_pin_hot2, OUTPUT);

    // Wyłączenie Przełączników
    _smokeState = false;
    _fanState = false;
    _hot1State = false;
    _hot2State = false;
    TurnOffAll();
}

void Relay::TurnOffAll()
{
    SmokeOff();
    FanOff();
    HotAllOff();
}
void Relay::TurnOnAll()
{
    SmokeOn();
    FanOn();
    HotAllOn();
}

bool Relay::GetSmokeState()
{
    return _smokeState;
}
void Relay::SmokeOn()
{
    _smokeState = true;
    digitalWrite(_relay_pin_smoke, !_smokeState);
}
void Relay::SmokeOff()
{
    _smokeState = false;
    digitalWrite(_relay_pin_smoke, !_smokeState);
}
void Relay::SmokeChange()
{
    _smokeState = !_smokeState;
    digitalWrite(_relay_pin_smoke, !_smokeState);
}

bool Relay::GetFanState()
{
    return _fanState;
}
void Relay::FanOn()
{
    _fanState = true;
    digitalWrite(_relay_pin_fan, !_fanState);
}
void Relay::FanOff()
{
    _fanState = false;
    digitalWrite(_relay_pin_fan, !_fanState);
}
void Relay::FanChange()
{
    _fanState = !_fanState;
    digitalWrite(_relay_pin_fan, !_fanState);
}

bool Relay::GetHot1State()
{
    return _hot1State;
}
void Relay::Hot1On()
{
    _hot1State = true;
    digitalWrite(_relay_pin_hot1, !_hot1State);
}
void Relay::Hot1Off()
{
    _hot1State = false;
    digitalWrite(_relay_pin_hot1, !_hot1State);
}
void Relay::Hot1Change()
{
    _hot1State = !_hot1State;
    digitalWrite(_relay_pin_hot1, !_hot1State);
}

bool Relay::GetHot2State()
{
    return _hot2State;
}
void Relay::Hot2On()
{
    _hot2State = true;
    digitalWrite(_relay_pin_hot2, !_hot2State);
}
void Relay::Hot2Off()
{
    _hot2State = false;
    digitalWrite(_relay_pin_hot2, !_hot2State);
}
void Relay::Hot2Change()
{
    _hot2State = !_hot2State;
    digitalWrite(_relay_pin_hot2, !_hot2State);
}

void Relay::HotAllOn()
{
    Hot1On();
    Hot2On();
}
void Relay::HotAllOff()
{
    Hot1Off();
    Hot2Off();
}
void Relay::HotAllChange()
{
    Hot1Change();
    Hot2Change();
}