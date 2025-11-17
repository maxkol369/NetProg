#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#define DAYTIME_PORT 13
#define BUFFER_SIZE 256
#define TIMEOUT 10  // Увеличили таймаут

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE];
    int bytes_received;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        fprintf(stderr, "Example: %s time.nist.gov\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    printf("Resolving hostname: %s\n", argv[1]);
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Error: cannot resolve hostname '%s'\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    
    printf("Host resolved to IP: %s\n", 
           inet_ntoa(*(struct in_addr*)server->h_addr));
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Настройка таймаута
    struct timeval tv;
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DAYTIME_PORT);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    
    printf("Connecting to %s:%d (UDP)...\n", argv[1], DAYTIME_PORT);
    
    // Отправляем пустой запрос
    if (sendto(sockfd, "", 0, 0, 
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    printf("Request sent, waiting for response (timeout: %d seconds)...\n", TIMEOUT);
    
    // Получаем ответ
    bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, NULL, NULL);
    
    if (bytes_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("Timeout: server did not respond within %d seconds\n", TIMEOUT);
            printf("Possible reasons:\n");
            printf("1. Server does not support UDP daytime protocol\n");
            printf("2. Server is down or unreachable\n");
            printf("3. Firewall blocking the connection\n");
        } else {
            perror("recvfrom failed");
        }
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    buffer[bytes_received] = '\0';
    printf("Server response received:\n%s", buffer);
    
    close(sockfd);
    return 0;
}
