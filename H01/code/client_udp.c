#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define PORT 8080

int main() {
  int sock_fd = 0;
  struct sockaddr_in serv_addr;
  char *hello = "Hello from client!";
  char buffer[1024];
  socklen_t addrlen = sizeof(serv_addr);

  printf("Creating socket\n");
  if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    printf("Socket creation error!\n");
    exit(EXIT_FAILURE);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  printf("Converting address...\n");
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("Invalid address.");
    exit(EXIT_FAILURE);
  }

  printf("Sending data...\n");
  if (sendto(sock_fd, hello, strlen(hello), 0, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0) {
    perror("Error occured.");
    exit(EXIT_FAILURE);
  }

  printf("Receiving data...\n");
  if ((recvfrom(sock_fd, buffer, sizeof(buffer), 0,
                (struct sockaddr *)&serv_addr, &addrlen)) < 0) {
    perror("Error occured.");
    exit(EXIT_FAILURE);
  }
  printf("%s\n", buffer);
}
