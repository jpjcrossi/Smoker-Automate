#ifndef NetworkService_H
#define NetworkService_H
#include <WiFi.h>
#include <Arduino.h>

class NetworkService
{
protected:
    static NetworkService *NetworkService_;
    WiFiClass _WiFi;

private:
    char *_ssid;
    char *_password;

public:
    NetworkService(WiFiClass WiFi);

    ~NetworkService(){};

    NetworkService(NetworkService &other) = delete;

    void operator=(const NetworkService &) = delete;

    static NetworkService *GetInstance(WiFiClass &WiFi);

    static NetworkService *GetInstance();

    void Connect(char *ssid, char *password);

    void Reconnect();

    IPAddress LocalIP();
};
#endif