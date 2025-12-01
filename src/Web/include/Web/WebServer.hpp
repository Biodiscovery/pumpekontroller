#pragma once
#include <cstdint>

class WebServer {
public:
    static void init();    // Start WiFi and HTTP server
    static void loop();    // Called repeatedly in main loop
};
