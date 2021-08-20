#include <WiFi.h>
#include <Arduino.h>

class Connect_To_NetWork
{
private:
    char *_ssid;
    char *_password;


public:
    Connect_To_NetWork(char *ssid, char *password);
    void Connect();
    bool IsConnected();
    ~Connect_To_NetWork();
};

Connect_To_NetWork::Connect_To_NetWork(char *ssid, char *password)
{
    _ssid = ssid;
    _password = password;
}

Connect_To_NetWork::~Connect_To_NetWork()
{
}

bool Connect_To_NetWork::IsConnected()
{
    return WiFi.status() != WL_CONNECTED;
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Connected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Disconnected from WiFi access point");
    Serial.print("WiFi lost connection. Reason: ");
    Serial.println(info.disconnected.reason);
    Serial.println("Trying to Reconnect");

    WiFi.reconnect();
}

void Connect_To_NetWork::Connect()
{

    // delete old config
    WiFi.disconnect(true);

    delay(1000);

    WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
    WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);

    WiFi.begin(_ssid, _password);

    Serial.println();
    Serial.println();
    Serial.println("Wait for WiFi... ");
    
}
