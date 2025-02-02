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
    Serial.println(info.wifi_sta_disconnected.reason);
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

    // Delete old config
    this->_WiFi.disconnect(true);

    delay(1000);

    // Start WiFi connection
    this->_WiFi.begin(_ssid, _password);
    this->_WiFi.setAutoConnect(true);

    Serial.println();
    Serial.println("Connecting to WiFi...");

    // Wait for connection
    int maxAttempts = 20; // Número máximo de tentativas
    int attempts = 0;

    while (this->_WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    // Check if connected
    if (this->_WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println("WiFi connected!");
        Serial.print("IP address: ");
        Serial.println(this->_WiFi.localIP());
    } else {
        Serial.println();
        Serial.println("Failed to connect to WiFi!");
    }
}
