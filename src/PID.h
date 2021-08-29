class PID
{
private:
    double kP, kI, kD;
    double sample;
    double setPoint;
    double error;
    double lastSample;
    double P, I, D;
    double pid;
    long lastProcess;

public:
    PID(double _kP, double _kI, double _kD);
    ~PID();
    double process();

    void addNewSample(double _sample)
    {
        sample = _sample;
    }

    void setSetPoint(double _setPoint)
    {
        setPoint = _setPoint;
    }
};

PID::PID(double _kP, double _kI, double _kD)
{
    kP = _kP;
    kI = _kI;
    kD = _kD;
}

PID::~PID()
{
}

double PID::process()
{
    // implementing P ID
    error = setPoint - sample;
    float deltaTime = (millis() - lastProcess) / 1000.0;
    lastProcess = millis();

    //P
    P = error * kP;

    //I
    I = I + (error * kI) * deltaTime;

    //D
    D = (lastSample - sample) * kD / deltaTime;
    lastSample = sample;

    // sum all
    pid = P + I + D;

    return pid;
}