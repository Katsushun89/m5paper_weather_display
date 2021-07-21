#include <WiFi.h>
#include "../config.h"
#include "wifi_connection.hpp"

bool WiFiConnection::setupWiFi(void)
{
    Serial.printf("connecting: %s, %s\r\n", SSID, PASS);

    WiFi.begin(SSID, PASS);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Check if connected to wifi
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No WiFi!");
        return false;
    }

    Serial.println("Connected to WiFi, Connecting to server.");
    return true;
}

bool WiFiConnection::downWiFi(void)
{
    return WiFi.disconnect();
}
 