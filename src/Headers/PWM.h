#ifndef PWM_H
#define PWM_H
#include <Arduino.h>
class PWM
{
private:
    int _motorPin1;
    int _motorPin2;
    int _enablePin;
    int _dutyCycle;

    const int _freq = 30000;
    const int _pwmChannel = 0;
    const int _resolution = 8;
    const int _bits = pow(2, _resolution);

public:
    PWM(int motorPin1, int motorPin2, int enablePin);
    ~PWM();
    void SetDutyCycle(int dutyCycle);
};
#endif