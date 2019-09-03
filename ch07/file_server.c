#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handle(const char *str)
{
    fprintf(stderr, "%s", str);
    exit(1);
}

// 服务端发送数据
int main(int argc, char *argv[])
{
    int server_sock, client_sock;
    int str_len = 0;
    struct sockaddr_in server_addr, client_addr;
    socklen_t clnt_addr_size;
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
    clnt_addr_size = sizeof(client_addr);
    FILE* send_file = fopen("send.py", "rb");

    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &clnt_addr_size);

    // 发送数据
    while(1) {
        // 往 buffer 里面读文件
        int read_len = fread(buffer, 1, BUF_SIZE, send_file);
        write(client_sock, buffer, read_len);
        if(read_len < BUF_SIZE) {
            shutdown(client_sock, SHUT_WR);
            break;
        }
    }

    size_t read_len = read(client_sock, buffer, BUF_SIZE);
    buffer[read_len] = 0;
    fprintf(stdout, "Message from %s", buffer);

    fclose(send_file);
    close(client_sock);
    close(server_sock);
    return 0;
}