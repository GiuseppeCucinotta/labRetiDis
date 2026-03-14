#include <arpa/inet.h> // funzioni per la gestione di indirizzi di rete
#include <stdio.h>     // funzioni input/output
#include <stdlib.h>    // utility varie
#include <string.h>    // manipolazione stringhe
#include <sys/socket.h>
#include <unistd.h> // funzioni di sistema; read and write

#define PORT 8080

int main() {
  int server_fd;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  char *hello = "Hello World!";

  printf("Creating socket...\n");
  if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Creation of socket failed.");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr =
      INADDR_ANY;                 // accept requests from any address (0.0.0.0)
  address.sin_port = htons(PORT); // converts the port to a readable byte code

  printf("Binding socket...\n");
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed.");
    exit(EXIT_FAILURE);
  }

  printf("Reading client buffer...\n");
  recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&address,
           (socklen_t *)&addrlen);
  printf("%s\n", buffer);

  printf("Sending response...\n");
  if (sendto(server_fd, hello, strlen(hello), 0, (struct sockaddr *)&address,
             sizeof(address)) < 0) {
    printf("Error occured.");
    exit(EXIT_FAILURE);
  }
}
