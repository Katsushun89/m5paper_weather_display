#pragma once

class WiFiConnection
{
public:
    WiFiConnection(){};
    ~WiFiConnection() = default;
    bool setupWiFi(void);
    bool downWiFi(void);
};

