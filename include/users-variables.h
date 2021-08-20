#include <Arduino.h>
// NetWork Connection
char* ssid = (char *)"ROSSIWRL"; // no need to fill in
char* password = (char *)"Master@db9$";
const char* ntpServer = "pool.ntp.org";

// MQTT
const char broker[] = "192.168.1.11";
int        port     = 1883;
const char mqttuser[] = "ajch"; //add eventual mqtt username
const char mqttpass[] = "Master@db9$"; //add eventual mqtt password

