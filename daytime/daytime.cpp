#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    const char* host = "172.16.40.1"; 
    int port = 13; 
    
    if (argc > 1) {
        host = argv[1];
    }
    
    std::cout << "Daytime клиент (UDP)" << std::endl;
    std::cout << "Подключение к: " << host << ":" << port << std::endl;
    
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return 1;
    }
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
        std::cerr << "Неверный адрес: " << host << std::endl;
        close(sockfd);
        return 1;
    }
    
    const char* request = "";
    ssize_t sent_bytes = sendto(sockfd, request, strlen(request), 0,
                               (struct sockaddr*)&server_addr, 
                               sizeof(server_addr));
    
    if (sent_bytes < 0) {
        perror("не удалось выполнить отправку");
        close(sockfd);
        return 1;
    }
    
    std::cout << "Запрос отправлен успешно" << std::endl;
    
    char buffer[1024];
    socklen_t addr_len = sizeof(server_addr);
    
    ssize_t recv_bytes = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                                 (struct sockaddr*)&server_addr, 
                                 &addr_len);
    
    if (recv_bytes < 0) {
        perror("восстановление после сбоя");
        close(sockfd);
        return 1;
    }
     
    buffer[recv_bytes] = '\0';
    
    std::cout << std::endl << "Ответ сервера (" << recv_bytes << " bytes):" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << buffer;
    std::cout << "----------------------------------------" << std::endl;
    
    close(sockfd);
    return 0;
}
