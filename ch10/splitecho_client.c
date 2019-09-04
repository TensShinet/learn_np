#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
char buffer[BUF_SIZE];
void read_event(int sock);
void write_event(int sock);
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
    pid_t pid = fork();
    if(pid != 0) {
        // 父进程
        write_event(client_sock);
    } else {
        // 子进程
        read_event(client_sock);
    }
    close(client_sock);
    return 0;
}

void read_event(int sock) {
    // 没有做过多的处理
    int str_len;
    while ((str_len = read(sock, buffer, BUF_SIZE)) != 0) {
        buffer[str_len] = 0;
        fprintf(stdout, "Message From Server: %s", buffer);
        fflush(stdout);
    }
}

void write_event(int sock) {
    while (1) {
        fprintf(stdout, "Input message(Q to quit): ");
        fflush(stdout);
        fgets(buffer, BUF_SIZE, stdin);
        if (!strcmp(buffer, "q\n") | !strcmp(buffer, "Q\n")) {
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buffer, strlen(buffer)); 
    }
}