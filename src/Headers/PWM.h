#ifndef PWM_H
#define PWM_H
#include <Arduino.h>
class PWM
{
private:
    int _fanPin1;
    int _fanPin2;
    int _fanEnablePin;
    int _dutyCycle;

    const int _freq = 5000;
    const int _pwmChannel = 0;
    const int _resolution =10;
    const int _bits = pow(2, _resolution);

public:
    PWM(int fanPin1, int fanPin2, int fanEnablePin);
    ~PWM();
    void SetDutyCycle(int dutyCycle);
};
#endif