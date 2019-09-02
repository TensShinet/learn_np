#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

// 服务端接收数据并输出
int main(int argc, char *argv[]) {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;

    if(argc != 2) {
        fprintf(stderr, "Usage is %s <port>", argv[0]);
        exit(1);
    }

    server_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(server_sock == -1) {
        error_handling("socket error");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if(bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        error_handling("bind error");
    }

    char buffer[BUF_SIZE];
    socklen_t addr_size = sizeof(client_addr);
    for(int i = 0; i < 3; i++) {
        int str_len = recvfrom(server_sock, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &addr_size);
        //your_adr这个地址需要从客户端传递过来，然后在这里传递给该变量
        printf("Message %d: %s \n", i + 1, buffer);
        sleep(1);
    }
    close(client_sock);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}
