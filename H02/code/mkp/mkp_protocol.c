#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

typedef struct {
    uint32_t length;
    uint8_t net_buffer[];
} mkp_packet;

int mkp_send(int sockfd, const uint8_t *data_buffer, uint32_t len) {
    uint32_t total_length = len + sizeof(uint32_t);
    mkp_packet *packet = malloc(total_length);
    if(packet == NULL) {
        printf("Error occured.\n");
        return -1;
    }
    packet->length = htonl(len);
    memcpy(packet->net_buffer, data_buffer, len);
    if(send_all(sockfd, packet, total_length) < 0) {
        printf("Error occured in send_all()\n");
        return -1;
    }
    free(packet);
    return 0;
}

int mkp_recv(int sockfd, uint8_t **out_buf, uint32_t *out_len) {
    uint32_t payload_length = 0;
    if(recv_all(sockfd, &payload_length, 4) < 0) {
        printf("Error occurred in recv_all()\n");
        return -1;
    }
    payload_length = ntohl(payload_length);
    *out_buf = malloc(payload_length + 1);
    if(recv_all(sockfd, *out_buf, payload_length) < 0) {
        printf("Error occurred in recv_all()\n");
        return -1;
    }
    (*out_buf)[payload_length] = '\0';
    *out_len = payload_length;
    return 0;
}
