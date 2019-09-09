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

int time_live = 64;
int main(int argc, char *argv[])
{
    int send_sock;
    pid_t pid;
    int str_len = 0, state;
    struct sockaddr_in mul_adr;
    char buffer[BUF_SIZE];
    FILE* fp;

    if (argc != 3)
    {
        fprintf(stdout, "Usage: %s <group> <port>", argv[0]);
        exit(1);
    }

    if ((send_sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
        error_handle("socket error");
    }
    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]); // group addr
    mul_adr.sin_port = htons(atoi(argv[2]));      // port

    // 设置 TTL
    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, &time_live, sizeof(time_live));

    fp = fopen("news.txt", "r");
    if(!fp) {
        error_handle("fopen error");
    }
    // 将文件内容组播出去
    while(!feof(fp)) {
        fgets(buffer, BUF_SIZE, fp);
        sendto(send_sock, buffer, strlen(buffer), 0, (struct sockaddr *)&mul_adr, sizeof(mul_adr));
        sleep(1);
    }
    // 发送完毕
    fclose(fp);
    close(send_sock);
    return 0;
}