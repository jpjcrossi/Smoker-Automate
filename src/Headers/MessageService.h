#ifndef MessageService_H
#define MessageService_H

#include <WiFi.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>
#include <Models/OffSetModel.h>

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

    void Loop();
};
#endif