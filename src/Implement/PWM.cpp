#include "Headers/PWM.h"

void PWM::SetDutyCycle(int dutyCycle)
{
    // int toPWM = map(constrain(dutyCycle, 0, 100), 0, 100, 0, 255);

    int toPWM = map(dutyCycle, 0, 100, 0, 255);

    uint32_t duty = (this->_bits / 255) * min(toPWM, 255);
    digitalWrite(_fanEnablePin, LOW); // see page 7 of datasheet. You must keep ENABLE pin low before applying changes to IN
    digitalWrite(_fanPin1, LOW);
    digitalWrite(_fanPin2, HIGH);

    _dutyCycle = dutyCycle;
    ledcWrite(_pwmChannel, duty);
};

PWM::PWM(int fanPin1, int fanPin2, int fanEnablePin)
{
    _fanPin1 = fanPin1;
    _fanPin2 = fanPin2;
    _fanEnablePin = fanEnablePin;

    // sets the pins as outputs:
    pinMode(_fanPin1, OUTPUT);
    pinMode(_fanPin2, OUTPUT);
    pinMode(_fanEnablePin, OUTPUT);

    // configure LED PWM functionalitites
    ledcSetup(_pwmChannel, _freq, _resolution);

    // attach the channel to the GPIO to be controlled
    ledcAttachPin(_fanEnablePin, _pwmChannel);

    digitalWrite(_fanPin1, HIGH);
    digitalWrite(_fanPin2, LOW);
}

PWM::~PWM()
{
}