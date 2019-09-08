#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 1024

void error_handle(const char *str) {
    fprintf(stderr, "%s", str);
    exit(1);
}

// 服务端发送数据
int main(int argc, char *argv[])
{
    int server_sock, client_sock;
    pid_t pid;
    int str_len = 0, state;
    struct sockaddr_in server_addr, client_addr;
    socklen_t clnt_addr_size = sizeof(client_addr);
    char buffer[BUF_SIZE];

    if (argc != 2)
    {
        fprintf(stdout, "Usage: %s <port>", argv[0]);
        exit(1);
    }

    if ((server_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        error_handle("socket error");
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        error_handle("bind error");
    }
    if (listen(server_sock, 5) == -1)
    {
        error_handle("listen error");
    }
    

    // 创建 read fds 以及它的 copy
    fd_set reads, reads_copy;
    FD_ZERO(&reads);
    FD_SET(server_sock, &reads);
    int fd_max = server_sock;
    int result;

    // 设置 select 超时时间
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;

    while(1) {
        reads_copy = reads;
        result = select(fd_max + 1, &reads_copy, 0, 0, &timeout);

        if (result == -1) {
            error_handle("select error");
        } else if(result == 0) {
            continue;
        }
        // 遍历所有文件描述符
        for(int i = 0; i < fd_max+1; i++) {
            if(!FD_ISSET(i, &reads_copy)) {
                continue;
            }
            if(i == server_sock) {
                // 处理客户端请求
                client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &clnt_addr_size);
                FD_SET(client_sock, &reads);
                // 因为是循环遍历所有的 fd 所以要更新最大的 fd_max
                if(client_sock > fd_max) {
                    fd_max = client_sock;
                }
                fprintf(stdout, "New Client! %d ", client_sock);
                fflush(stdout);
            } else {
                // 而且 read 不会阻塞
                str_len = read(i, buffer, BUF_SIZE);
                buffer[str_len] = 0;
                
                if(str_len == 0) {
                    // 结束
                    FD_CLR(i, &reads);
                    close(i);
                } else {
                    write(i, buffer, str_len);
                }
                
            }
            
        }
    }
    close(server_sock);
    return 0;
}