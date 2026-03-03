//
// Created by mikaib on 03-03-2026.
//

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <services/time.h>
#include <sys/socket.h>
#include <esp_log.h>
#include <stdbool.h>
#include <config.h>
#include <netdb.h>

void service_time_task(void* params) {
    for (;;) {
        service_time_sync();
        vTaskDelay(SYNC_TIME_EVERY * 1000 / portTICK_PERIOD_MS);
    }
}

void service_time_init(bool create_task) {
    if (create_task) {
        xTaskCreate(&service_time_task, "time_task", 4096, NULL, 0, NULL);
    } else {
        service_time_sync();
    }
}

// https://github.com/al-sultani/NTP-Client/blob/master/main.c
// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/lwip.html#bsd-sockets-api
// https://github.com/espressif/esp-idf/tree/v5.5.3/examples/protocols/sockets/udp_client
void service_time_sync() {
    ESP_LOGI("time", "service_time_sync called");

    time_packet_t packet;
    struct sockaddr_in sntp_addr;
    int sockfd;

    memset(&packet, 0, sizeof(time_packet_t)); // clear
    *((char *)&packet) = 0x1b; // li, vn, mode

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // socket
    if (sockfd < 0) {
        ESP_LOGE("time", "could not create socket");
        return;
    }

    struct hostent *dns = gethostbyname(SNTP_SERVER); // dns
    if (!dns) {
        ESP_LOGE("time", "failed dns lookup");
        close(sockfd);
        return;
    }

    memset(&sntp_addr, 0, sizeof(sntp_addr)); // clear
    sntp_addr.sin_family = AF_INET;
    sntp_addr.sin_port = htons(SNTP_PORT);
    memcpy(&sntp_addr.sin_addr.s_addr, dns->h_addr, dns->h_length); // copy dns info to sockaddr_in

    if (sendto(sockfd, &packet, sizeof(time_packet_t), 0, (struct sockaddr *)&sntp_addr, sizeof(sntp_addr)) < 0) { // send packet
        ESP_LOGE("time", "failed to send packet");
        close(sockfd);
        return;
    }

    socklen_t addr_len = sizeof(sntp_addr);
    if (recvfrom(sockfd, &packet, sizeof(time_packet_t), 0, (struct sockaddr *)&sntp_addr, &addr_len) < 0) {
        ESP_LOGE("time", "could not receive packet");
        close(sockfd);
        return;
    }

    close(sockfd);

    packet.txTm_s = ntohl(packet.txTm_s); // endianess
    time_t txTm = (time_t)(packet.txTm_s - SNTP_EPOCH + (SNTP_HOUR_OFFSET * 3600));

    ESP_LOGI("time", "received: %s", ctime(&txTm));

    struct timeval now = {
        .tv_sec = txTm,
        .tv_usec = 0
    };

    settimeofday(&now, NULL);
}