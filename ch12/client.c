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

    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        error_handle("connect error");
    }
    else
    {
        puts("Connected..............");
    }

    while (1)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(buffer, BUF_SIZE, stdin);

        if (!strcmp(buffer, "q\n") | !strcmp(buffer, "Q\n"))
        {
            break;
        }
        write(client_sock, buffer, strlen(buffer));
        read(client_sock, buffer, BUF_SIZE);
        printf("Message from server: %s \n", buffer);
    }
    close(client_sock);
    return 0;
}