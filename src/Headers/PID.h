#ifndef PID_H
#define PID_H
#include <Arduino.h>
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

    void addNewSample(double _sample);

    void setSetPoint(double _setPoint);
};
#endif