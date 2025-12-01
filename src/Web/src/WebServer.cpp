#include "Web/WebServer.hpp"

#include "pico/cyw43_arch.h"
#include "lwip/apps/httpd.h"
#include <cstdio>

void WebServer::init() {
    // Init WiFi + LwIP
    cyw43_arch_init();

    // Connect to WiFi
    cyw43_arch_enable_sta_mode();
    printf("Connecting to WiFi...\n");

    int result = cyw43_arch_wifi_connect_timeout_ms(
        "Stijp-D",
        "creativecowboys",
        CYW43_AUTH_WPA2_AES_PSK,
        30000
    );

    if (result != 0) {
        printf("Failed to connect. Error %d\n", result);
    } else {
        printf("Connected!\n");
    }

    // Start HTTP server
    printf("Starting HTTP server...\n");
    httpd_init();

    printf("HTTP server started.\n");
}

void WebServer::loop() {
    // Nothing needed for now — background lwIP thread handles traffic
}
