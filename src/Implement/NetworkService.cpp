#include "Headers/NetworkService.h"

NetworkService *NetworkService::NetworkService_ = nullptr;

NetworkService::NetworkService(WiFiClass WiFi)
{
    this->_WiFi = WiFi;
}

void NetworkService::Reconnect()
{
    _WiFi.reconnect();
}
IPAddress NetworkService::LocalIP()
{
    return _WiFi.localIP();
}

NetworkService *NetworkService::GetInstance(WiFiClass &WiFi)
{
    if (NetworkService_ == nullptr)
    {
        NetworkService_ = new NetworkService(WiFi);
    }
    return NetworkService_;
}
NetworkService *NetworkService::GetInstance()
{
    return NetworkService_;
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Connected to AP successfully!");
}
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Disconnected from WiFi access point");
    Serial.print("WiFi lost connection. Reason: ");
    Serial.println(info.disconnected.reason);
    Serial.println("Trying to Reconnect");
    NetworkService::GetInstance()->Reconnect();
}
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(NetworkService::GetInstance()->LocalIP());
}

void NetworkService::Connect(char *ssid, char *password)
{
    _ssid = ssid;
    _password = password;

    // delete old config
    this->_WiFi.disconnect(true);

    delay(1000);

    this->_WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
    this->_WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);
    this->_WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);

    this->_WiFi.begin(_ssid, _password);

    Serial.println();
    Serial.println();
    Serial.println("Wait for WiFi... ");
}
