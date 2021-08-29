#include "Arduino.h"
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
    const int _bits = pow(2,_resolution); 


public:
    PWM(int motorPin1, int motorPin2, int enablePin);
    ~PWM();
    void SetDutyCycle(int dutyCycle)
    {

        int toPWM =   map(dutyCycle, 0,100,0,255);

        uint32_t duty = (this->_bits / 255) * min(toPWM, 255);
        digitalWrite(_enablePin, LOW);// see page 7 of datasheet. You must keep ENABLE pin low before applying changes to IN			
        digitalWrite(_motorPin1, LOW);
        digitalWrite(_motorPin2, HIGH);

        _dutyCycle = dutyCycle;
        ledcWrite(_pwmChannel, duty);
    };
};

PWM::PWM(int motorPin1, int motorPin2, int enablePin)
{
    _motorPin1 = motorPin1;
    _motorPin2 = motorPin2;
    _enablePin = enablePin;
    

    // sets the pins as outputs:
    pinMode(_motorPin1, OUTPUT);
    pinMode(_motorPin2, OUTPUT);
    pinMode(_enablePin, OUTPUT);

    // configure LED PWM functionalitites
    ledcSetup(_pwmChannel, _freq, _resolution);

    // attach the channel to the GPIO to be controlled
    ledcAttachPin(_enablePin, _pwmChannel);
 
    digitalWrite(_motorPin1, HIGH);
    digitalWrite(_motorPin2, LOW); 
}

PWM::~PWM()
{
}
