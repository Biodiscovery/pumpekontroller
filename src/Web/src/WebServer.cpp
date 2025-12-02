
#include "Web/WebServer.hpp"

#include "pico/cyw43_arch.h"
#include "lwip/apps/httpd.h"
#include "lwip/apps/mdns.h"
#include <cstdio>

// This comes from main.cpp
namespace pump_control {
    extern int power;
    extern int step;
}


// Forward declare CGI handler
static const char* cgi_handler(int index, int numParams, char* params[], char* values[]);
pump_control::PumpController* WebServer::ctrl = nullptr;
// ---------------------------
// WebServer::init()
// ---------------------------
void WebServer::init(pump_control::PumpController& controller) {
    ctrl = &controller;
    cyw43_arch_init();
    cyw43_arch_enable_sta_mode();
    static absolute_time_t wifi_connected_time;

    printf("Connecting to WiFi...\n");
    int result = cyw43_arch_wifi_connect_timeout_ms(
        "Stijp-D",
        "creativecowboys",
        CYW43_AUTH_WPA2_AES_PSK,
        30000
    );

    if (result == 0) {
        printf("WiFi: Connected!\n");

        cyw43_arch_lwip_begin();

        // Print IP address
        const ip4_addr_t *ip = netif_ip4_addr(netif_list);
        printf("Pico IP address: %s\n", ip4addr_ntoa(ip));
        cyw43_arch_lwip_end();

        printf("mDNS: visit http://pico.local\n");
    }
    else {
        printf("WiFi: FAILED (error = %d)\n", result);

        // Helpful error decoding
        switch (result) {
            case CYW43_EV_AUTH:
                printf("Reason: AUTH FAILED (wrong password?)\n");
                break;

            case CYW43_ETIMEDOUT:
                printf("Reason: TIMEOUT (AP not found?)\n");
                break;
            default:
                printf("Reason: UNKNOWN ERROR\n");
                break;
    }
}


    // Start HTTP server
    cyw43_arch_lwip_begin();
    httpd_init();

    static const tCGI cgi_table[] = {
        { "/start",     cgi_handler },
        { "/stop",      cgi_handler },
        { "/upspeed",   cgi_handler },
        { "/downspeed", cgi_handler },
        { "/status",    cgi_handler }
    };

    http_set_cgi_handlers(cgi_table, 5);
     // mDNS setup
    mdns_resp_init();
    mdns_resp_add_netif(&cyw43_state.netif[CYW43_ITF_STA], "pico");
    mdns_resp_add_service(
        &cyw43_state.netif[CYW43_ITF_STA],
        
        "http", "_http", DNSSD_PROTO_TCP, 80, NULL, NULL
    );
    cyw43_arch_lwip_end();
    printf("HTTP server ready.\n");
}

// ---------------------------
// WebServer::loop()
// ---------------------------
void WebServer::loop() {
    // Nothing – lwIP runs in background thread
}

// ---------------------------
// CGI Handler
// ---------------------------
static const char* cgi_handler(int index, int, char**, char**) {
    switch(index) {
        case 0: WebServer::ctrl->start(); break;
        case 1: WebServer::ctrl->stop(); break;
        case 2: WebServer::ctrl->up(); break;
        case 3: WebServer::ctrl->down(); break;

        case 4: {
            static char buf[16];
            snprintf(buf, sizeof(buf), "%d", WebServer::ctrl->getPower());
            return buf;
        }
    }

    return "/index.html"; // can be changed later
}
