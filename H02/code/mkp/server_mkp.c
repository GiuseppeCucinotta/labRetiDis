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

uint8_t calculate_checksum(uint8_t *buffer, size_t n) {
  uint8_t checksum = 0;
  for(int i=0; i<n; i++) {
    checksum ^= buffer[i];
  }
  return checksum;
}

/*
 *  @brief Receive exactly 'n' byte from socket 'sockfd', from buffer 'buf'.
 *  @returns n if the reading was successful; 
 *  @returns 0 if the connection was closed before receving n (or any) byte(s);
 *  @returns (-1) in case of errors
 * 
 *  @param sockfd Socket file descriptor
 *  @param *buf Pointer to the buffer to fill
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

int mkp_recv(int sockfd, uint8_t **out_buf, uint32_t *out_len) {
    uint32_t payload_length = 0;
    uint8_t checksum = 0;
    int byte_received = recv_all(sockfd, &payload_length, 4);
    if(byte_received < 0) {
        printf("Error occurred in recv_all()\n");
        return -1;
    } else if (byte_received == 0) {
      return -1;
    }
    payload_length = ntohl(payload_length);
    *out_buf = malloc(payload_length + 1);
    if(recv_all(sockfd, *out_buf, payload_length) <= 0) {
        printf("Error occurred in recv_all()\n");
        return -1;
    }
    (*out_buf)[payload_length] = '\0';
    *out_len = payload_length;
    if((recv_all(sockfd, &checksum, 1)) <= 0) {
      printf("Error occured in recv_all()\n");
    }
    
    return 0;
}



void print_buffer(uint8_t buf[], size_t buflen) {
  for (int i = 0; i < buflen; i++) {
    printf("%c", buf[i]);
  }
  printf("\n");
}

int main(void) {
  uint32_t BUFLEN = 2048;
  int server_fd, client_fd, return_value = 0;
  struct sockaddr_in addr;
  uint8_t *buffer = NULL;

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

  while(1) {
    return_value = mkp_recv(client_fd, &buffer, &BUFLEN);
    printf("[DEBUG] returnValue of mkp_recv: %d\n", return_value);
    if(return_value < 0) break;
    printf("[DEBUG] printing buffer:\n");
    if(BUFLEN < 100) {
      print_buffer(buffer, BUFLEN);
    } else {
      printf("Recevied message of %d bytes.\n", BUFLEN);
    }
    free(buffer);
  }
  

  close(client_fd);
  close(server_fd);
  return 0;
}