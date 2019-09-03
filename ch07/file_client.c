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

int main(int argc, char *argv[])
{
    int client_sock;
    int str_len;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE];

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // 创建套接字 socket
    if ((client_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        error_handle("socket error");
    }

    // 服务端 sockaddr_in
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        error_handle("connect error");
    }
    else {
        puts("Connected..............");
    }

    FILE* recv_file = fopen("recv.py", "wb");

    // 接收数据
    while(1) {
        size_t read_len = read(client_sock, buffer, BUF_SIZE);
        if(read_len < BUF_SIZE) {
            buffer[read_len] = 0;
            fwrite(buffer, 1, read_len, recv_file);
            break;
        }
        fwrite(buffer, 1, read_len, recv_file);
    }

    char final_str[] = "Thank You!";
    write(client_sock, final_str, strlen(final_str));
    fclose(recv_file);
    close(client_sock);


    system("python3 recv.py");
    return 0;
}