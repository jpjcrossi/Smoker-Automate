#include <WiFi.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <esp_wifi.h>
#include "ArduinoJson.h"
#include "./Models/OffSetModel.h"

// mqtt constants
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

class MessageService
{
protected:
    static MessageService *MessageService_;
    WiFiClass _WiFi;

private:
    char *_broker;
    int _port = 0;
    char *_mqttuser;
    char *_mqttpass;
    char *_subscribeTopic;
    char *_alertTopic;
    char *_operationTopic;
    void SetInstance(MessageService *messageService)
    {
        MessageService_ = messageService;
    }

public:
    MessageService(char *broker, int port, char *mqttuse, char *mqttpass);
    ~MessageService();
    void Connect(char *subscribeTopic, char *alertTopic, char *operationTopic);
    void (*callbackMessage)(OffSetModel offset);
    MessageService(MessageService &other) = delete;
    void operator=(const MessageService &) = delete;
    static MessageService *GetInstance();
    void Loop()
    {
        mqttClient.loop();
    }
};

MessageService *MessageService::MessageService_ = nullptr;
MessageService *MessageService::GetInstance()
{
    return MessageService_;
}

MessageService::~MessageService()
{
}
void callback(char *topic, byte *payload, unsigned int length)
{
    OffSetModel offset;
    String messageTemp;
    for (int i = 0; i < length; i++)
    {
        messageTemp += (char)payload[i];
    }
    //Serial.println(messageTemp);
    
    DynamicJsonDocument doc(1024);                         //Memory pool
    auto error = deserializeJson(doc, payload);
    if (error) {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(error.c_str());
        return;
    }
    offset.Temperature =  doc["Temperature"];
    offset.OffSet = doc["OffSet"];
    offset.Tolerance = doc["Tolerance"];
    offset.NaturalFlow = doc["NaturalFlow"];
    MessageService::GetInstance()->callbackMessage(offset);
}
MessageService::MessageService(char *broker, int port, char *mqttuser, char *mqttpass)
{
    _broker = broker;
    _port = port;
    _mqttuser = mqttuser;
    _mqttpass = mqttpass;
    MessageService::SetInstance(this);
}

void MessageService::Connect(char *subscribeTopic, char *alertTopic, char *operationTopic)
{
    _subscribeTopic = subscribeTopic;
    _alertTopic = alertTopic;
    _operationTopic = operationTopic;

    mqttClient.setServer(_broker, _port);
    mqttClient.setCallback(callback);

    while (!mqttClient.connected())
    {
        Serial.println("Connecting to MQTT…");
        String clientId = "ESP32Client -";
        if (mqttClient.connect(clientId.c_str(), _mqttuser, _mqttpass))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(mqttClient.state());
            delay(2000);
        }
    }
    mqttClient.subscribe(subscribeTopic);
}
