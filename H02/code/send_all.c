#include <arpa/inet.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080

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
    printf("Byte received real-time: %zd\n", byteSession);

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
    ssize_t returnValue = send_all(sockfd, messages[i], len);
    printf("[DEBUG] returnValue of send_all: %zd\n", returnValue);
    usleep(1000);
  }

  close(sockfd);
  return 0;
}
