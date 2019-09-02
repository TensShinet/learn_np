#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

// 客户端发送数据
int main(int argc, char *argv[])
{
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;

    if (argc != 3)
    {
        fprintf(stderr, "Usage is %s <ip> <port>", argv[0]);
        exit(1);
    }

    client_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (server_sock == -1)
    {
        error_handling("socket error");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    char message1[] = "Hello!";
    char message2[] = "Nice To Meet You!";
    char message3[] = "Bye Bye!";
    printf("%s", message2);
    sendto(client_sock, message1, sizeof(message1)+1, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    sendto(client_sock, message2, sizeof(message2)+1, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    sendto(client_sock, message3, sizeof(message3)+1, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    close(client_sock);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}
