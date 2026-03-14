#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080

typedef struct {
    uint32_t length;
    uint8_t net_buffer[];
} mkp_packet;

/*
 *  @brief Send exactly 'n' byte to socket 'sockfd', from buffer 'buf'.
 *  @returns n if the reading was successful; 
 *  @returns (-1) in case of errors
 * 
 *  @param sockfd Socket file descriptor
 *  @param *buf Pointer to the buffer
 *  @param n Dimension of data to send
 *
 */
ssize_t send_all(int sockfd, const void *buf, size_t n) {
  ssize_t byteSent = 0, byteSession = 0;
  uint8_t *bufferPointer = (uint8_t *)buf;
  for (int i = 0; i < n; i += byteSession) {
    byteSession = send(sockfd, bufferPointer, n - byteSent, 0);
    printf("Byte sent real-time: %zd\n", byteSession);

    if (byteSession < 0) {
      printf("Error occured.");
      return -1;
    } else {
      byteSent += byteSession;
      bufferPointer += byteSession;
    }
  }
  return byteSent;
}

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

uint8_t* create_message(size_t msg_size) {
  uint8_t *msg = malloc(msg_size);
  memset(msg, 'A', msg_size);
  return msg;
}

int main(void) {
  int sockfd;
  struct sockaddr_in addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

  if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    printf("Error occured in connect. \n");
  }

  char *messages[] = {"Primo messaggio di testo",
                      "Secondo messaggio, piu lungo del primo", "Terzo"};

  int elementsNumber = sizeof(messages) / sizeof(messages[0]);
  for (int i = 0; i < elementsNumber; i++) {
    size_t len = strlen(messages[i]);
    int returnValue = mkp_send(sockfd, (const uint8_t *)messages[i], len);
    printf("[DEBUG] returnValue of mkp_send: %d\n", returnValue);
    usleep(1000);
  }

  uint8_t *big_message = create_message(64*1024);
  mkp_send(sockfd, big_message, 64*1024);
  free(big_message);

  close(sockfd);
  return 0;
}