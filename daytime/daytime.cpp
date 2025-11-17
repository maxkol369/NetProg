#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define TARGET_IP "172.16.40.1"
#define TARGET_PORT 13
#define BUFFER_SIZE 256
#define TIMEOUT 10

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int bytes_received;
    
    printf("Daytime Client\n");
    printf("Target: %s:%d\n", TARGET_IP, TARGET_PORT);
    
    // Создаем UDP сокет
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
    
    // Заполняем структуру сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TARGET_PORT);
    
    // Преобразуем IP-адрес
    if (inet_pton(AF_INET, TARGET_IP, &server_addr.sin_addr) <= 0) {
        perror("invalid address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    printf("Connecting to %s:%d (UDP)...\n", TARGET_IP, TARGET_PORT);
    
    // Отправляем пустой запрос
    if (sendto(sockfd, "", 0, 0, 
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    printf("Request sent, waiting for response...\n");
    
    // Получаем ответ
    bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, NULL, NULL);
    
    if (bytes_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("Timeout: server did not respond within %d seconds\n", TIMEOUT);
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
