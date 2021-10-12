#include <Headers/MessageService.h>

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

MessageService *MessageService::MessageService_ = nullptr;
MessageService *MessageService::GetInstance()
{
    return MessageService_;
}

void MessageService::Loop()
{
    mqttClient.loop();
}

void callback(char *topic, byte *payload, unsigned int length)
{
    OffSetModel offset;
    String messageTemp;
    for (int i = 0; i < length; i++)
    {
        messageTemp += (char)payload[i];
    }

    DynamicJsonDocument doc(1024); //Memory pool
    auto error = deserializeJson(doc, payload);
    if (error)
    {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(error.c_str());
        return;
    }

     offset.Temperature = doc["Temperature"];
     offset.OffSet = doc["OffSet"];
     offset.Tolerance = doc["Tolerance"];
     offset.NaturalFlow = doc["NaturalFlow"];
      MessageService::GetInstance()->callbackMessage(offset);
}

MessageService::MessageService(char *broker, int port, char *mqttuser, char *mqttpass)
{
    _port = port;
    _broker = broker;
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

MessageService::~MessageService()
{
}