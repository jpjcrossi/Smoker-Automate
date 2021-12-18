#pragma once
#include <Arduino.h>
class FeedBackModel
{
public:
    String State_Value;
    double PWM_Value = 0;
    double Temperature_Value = 0;
};