#include "Headers/PID.h"

PID::PID(double _kP, double _kI, double _kD)
{
    kP = _kP;
    kI = _kI;
    kD = _kD;
}

PID::~PID()
{
}

void PID::addNewSample(double _sample)
{
    sample = _sample;
}

void PID::setSetPoint(double _setPoint)
{
    setPoint = _setPoint;
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