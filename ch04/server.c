#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handle(const char* str) {
    fprintf(stderr, "%s", str);
    exit(1);
}

int main(int argc, char* argv[]) {
    int server_sock, client_sock;
    int str_len = 0;
    struct sockaddr_in server_addr, client_addr;
    socklen_t clnt_addr_size;
    char buffer[BUF_SIZE];


    if(argc != 2) {
        fprintf(stdout, "Usage: %s <port>", argv[0]);
        exit(1);
    }

    // 创建套接字 socket
    if((server_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        error_handle("socket error");
    }
    // 给套接字分配 ip bind
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));
    if(bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        error_handle("bind error");
    }
    // 监听请求 listen
    if(listen(server_sock, 5) == -1) {
        error_handle("listen error");
    }
    // 得到新的套接字 accept
    clnt_addr_size = sizeof(client_addr);
    printf("waitting:\n");
    while(1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &clnt_addr_size);
        if (client_sock == -1) {
            error_handle("accept error");
        }
        // 读取数据
        while((str_len = read(client_sock, buffer, BUF_SIZE)) != 0) {
            write(client_sock, buffer, str_len-1);
        }
        close(client_sock);
    }
    close(server_sock);
    return 0;
}