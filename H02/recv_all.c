#include <arpa/inet.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * size_t -> unsigned object lenght
 * ssize_t -> signed object length (useful for TCP bytes)
 * uint8_t -> 8 bit (1 byte)
 */

/*
 *  @brief Receive exactly 'n' byte from socket 'sockfd', from buffer 'buf'.
 *  @returns n if the reading was successful; 
 *  @returns 0 if the connection was closed before receving n (or any) byte(s);
 *  @returns (-1) in case of errors
 * 
 *  @param sockfd Socket file descriptor
 *  @param *buf Pointer to the buffer
 *  @param n Dimension of data to receive
 *
 */
ssize_t recv_all(int sockfd, void *buf, size_t n) {
  ssize_t byteRecv = 0, byteSession = 0;
  uint8_t *bufferPointer = (uint8_t *)buf;
  for (int i = 0; i < n; i += byteSession) {
    byteSession = recv(sockfd, bufferPointer, n - byteRecv, 0);
    printf("Byte received real-time: %zd\n", byteSession);

    if (byteSession < 0) {
      printf("Error occured.\n");
      return -1;
    } else if (byteSession == 0) {
      printf("Connection closed. Received %zd bytes.\n", byteRecv);
      return byteRecv;
    } else {
      byteRecv += byteSession;
      bufferPointer += byteSession;
    }
  }
  return byteRecv;
}

void print_buffer(char buf[], size_t buflen) {
  for (int i = 0; i < buflen; i++) {
    printf("%c", buf[i]);
  }
}

#define BUFLEN 2048
#define PORT 8080

int main(void) {
  int server_fd, client_fd;
  struct sockaddr_in addr;
  char buf[BUFLEN] = {0};

  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    printf("Error occured in bind.\n");
  }
  listen(server_fd, 5);

  printf("Server listening to: %d\n", PORT);
  client_fd = accept(server_fd, NULL, NULL);

  ssize_t returnValue = recv_all(client_fd, buf, BUFLEN);
  printf("[DEBUG] returnValue of recv_all: %lu\n", returnValue);
  printf("[DEBUG] printing buffer:\n");
  print_buffer(buf, returnValue);

  close(client_fd);
  close(server_fd);
  return 0;
}
