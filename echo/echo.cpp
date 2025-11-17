#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TARGET_IP "172.16.40.1"
#define ECHO_PORT 7
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    
    printf("=== Echo Client ===\n");
    printf("Server: %s:%d\n", TARGET_IP, ECHO_PORT);
    printf("Type 'quit' to exit\n\n");
    
    // Создаем TCP сокет
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }
    
    // Настраиваем сервер
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(ECHO_PORT);
    inet_pton(AF_INET, TARGET_IP, &server_addr.sin_addr);
    
    // Подключаемся
    printf("Connecting...\n");
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return 1;
    }
    
    printf("Connected!\n\n");
    
    // Основной цикл
    while (1) {
        printf("Enter message: ");
        fflush(stdout);
        
        if (fgets(message, BUFFER_SIZE, stdin) == NULL) break;
        
        message[strcspn(message, "\n")] = 0;
        
        if (strcmp(message, "quit") == 0) break;
        if (strlen(message) == 0) continue;
        
        // Отправляем
        int sent = send(sockfd, message, strlen(message), 0);
        if (sent < 0) {
            perror("send");
            break;
        }
        
        // Получаем ответ
        int received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (received < 0) {
            perror("recv");
            break;
        }
        
        buffer[received] = '\0';
        printf("Echo: %s\n\n", buffer);
    }
    
    close(sockfd);
    printf("Goodbye!\n");
    return 0;
}
