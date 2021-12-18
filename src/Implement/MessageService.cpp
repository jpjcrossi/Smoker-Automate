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

    offset.OffSet = doc["OffSet"];
    offset.Tolerance = doc["Tolerance"];
    offset.MinValue = doc["MinValue"];
    offset.TimeToCorrect = doc["TimeToCorrect"];
    offset.TimeToStabilize = doc["TimeToStabilize"];

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

    mqttClient.subscribe(_subscribeTopic);
}

void MessageService::SendFeedBack(FeedBackModel feedBackModel)
{
    char out[128];
    StaticJsonDocument<256> doc;
    doc["Temperature_Value"] = feedBackModel.Temperature_Value;
    doc["State_Value"] = feedBackModel.State_Value;
    doc["PWM_Value"] = feedBackModel.PWM_Value;
    serializeJson(doc, out);

    mqttClient.publish(_operationTopic, out);
}

MessageService::~MessageService()
{
}