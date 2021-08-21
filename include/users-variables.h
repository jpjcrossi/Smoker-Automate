#include <Arduino.h>
// NetWork Connection
char *ssid = (char *)"ROSSIWRL"; // no need to fill in
char *password = (char *)"Master@db9$";
const char *ntpServer = "pool.ntp.org";

// MQTT
char broker[] = "192.168.1.11";
int port = 1883;
char mqttuser[] = "ajch";        //add eventual mqtt username
char mqttpass[] = "Master@db9$"; //add eventual mqtt password
char subscribeTopic[] = "esp/test";
char alertTopic[] = "Smoker_Automate_alertTopic";
char operationTopic[] = "Smoker_Automate_operationTopic";