class PWM
{
private:
    int _motor1Pin1;
    int _motor1Pin2;
    int _enable1Pin;
    int _enable2Pin;
    int _dutyCycle;

    const int freq = 30000;
    const int pwmChannel = 0;
    const int resolution = 8;

public:
    PWM(int motor1Pin1, int motor1Pin2, int enable1Pin, int enable2Pin);
    ~PWM();
    void SetDutyCycle(int dutyCycle){
            _dutyCycle = dutyCycle;
     digitalWrite(_enable2Pin, 1);
     digitalWrite(_motor1Pin1, HIGH);
     digitalWrite(_motor1Pin2, LOW);
     ledcWrite(pwmChannel, dutyCycle);
    };
};

PWM::PWM(int motor1Pin1, int motor1Pin2, int enable1Pin, int enable2Pin)
{
    _motor1Pin1 = motor1Pin1;
    _motor1Pin2 = motor1Pin2;
    _enable1Pin = enable1Pin;
    _enable2Pin = enable2Pin;

    // sets the pins as outputs:
    pinMode(_motor1Pin1, OUTPUT);
    pinMode(_motor1Pin2, OUTPUT);
    pinMode(_enable1Pin, OUTPUT);
    pinMode(_enable2Pin, OUTPUT);

    // configure LED PWM functionalitites
    ledcSetup(pwmChannel, freq, resolution);

    // attach the channel to the GPIO to be controlled
    ledcAttachPin(enable1Pin, pwmChannel);
}


PWM::~PWM()
{
}
