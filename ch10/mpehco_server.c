#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handle(const char *str)
{
    fprintf(stderr, "%s", str);
    exit(1);
}

void rm_childproc(int sig) {
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("removed proc id : %d \n", pid);
}

// 服务端发送数据
int main(int argc, char *argv[])
{
    int server_sock, client_sock;
    pid_t pid;
    int str_len = 0, state;
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
    // 消灭僵尸子进程
    struct sigaction act;
    act.sa_handler = rm_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

    clnt_addr_size = sizeof(client_addr);
    while(1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &clnt_addr_size);
        if(client_sock == -1) {
            continue;
        } else {
            puts("New Client!");
        }

        pid = fork();

        if(pid == -1) {
            close(client_sock);
            continue;
        }
        else if(pid == 0) {
            // 子进程
            close(server_sock);
            // 回写
            while ((str_len = read(client_sock, buffer, BUF_SIZE)) != 0) {
                printf("str_len %d\n", str_len);
                fflush(stdout);
                write(client_sock, buffer, str_len);
            }
            close(client_sock);
            puts("Close client");
        } else {
            // 父进程
            close(client_sock);
        }

    }
    close(server_sock);
    return 0;
}