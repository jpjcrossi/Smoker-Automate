#include <Arduino.h>
// NetWork Connection
char *ssid = (char *)"ROSSIWRL"; // no need to fill in
char *password = (char *)"Master@db9$";
const char *ntpServer = "pool.ntp.org";

// MQTT
char broker[] = "192.168.1.11";
int port = 1883;
char mqttuser[] = "ajch"; //add eventual mqtt username
char mqttpass[] = "Master@db9$"; //add eventual mqtt password
char subscribeTopic[] = "Smoker_Automate_subscribeTopic";
char alertTopic[] = "Smoker_Automate_alertTopic";
char operationTopic[] = "Smoker_Automate_operationTopic";

// SPI for thermocouple
int thermoSO = 19;
int thermoCS = 5;
int thermoCLK = 18;
double TemperatureOffSet = 0.00;
/*

MAX6675       ESP32 D1 Mini
--------      -------------
VCC       ->         3.3v
GND       ->         GND
CLK       ->         IO18
CS        ->         IO5
SO        ->         IO19

*/

// PWM FAN
//int FanPin1 = 21;               
//int FanPin2 = 22;
int FanPin1 = 00;               
int FanPin2 = 04;
int FanenablePin = 17;
double FanOffSet = 48;
/*
L298N          ESP32 D1 Mini
--------      -------------
Enable1   ->         IO17
Input1    ->         IO00
Input2    ->         IO04
*/


// PID
double kP = 1.0;
double kI = 0.0;
double kD = 0.0;


enum SmokerState {
    RELEASED_TO_FIX,
    APPLYING_CORRECTION,
    STABILIZING
};