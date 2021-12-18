#pragma once
#include <Arduino.h>

class OffSetModel
{

public:
    double OffSet = 0;
    double Tolerance = 0;
    double MinValue = 0;
    int TimeToCorrect = 0;
    int TimeToStabilize = 0;
};
