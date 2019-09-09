#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int recv_sock;
    int str_len;
    char buf[BUF_SIZE];
    struct sockaddr_in adr;
    struct ip_mreq join_adr;

    if (argc != 3) {
        printf("Usage : %s <group> <port>\n", argv[0]);
        exit(1);
    }


    // 创建 recv_sock
    if ((recv_sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        error_handling("socket error");
    }

    memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY); // group addr
    adr.sin_port = htons(atoi(argv[2]));     // port

    if(bind(recv_sock,(struct sockaddr *)&adr,sizeof(adr))==-1) {
	    error_handling("bind error");
    }
    // 设置接收 组播
    join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_adr.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&join_adr, sizeof(join_adr));

    // 假设接受的数据一直小于 BUF_SIZE
    while(1) {
        str_len = recvfrom(recv_sock, buf, BUF_SIZE, 0, NULL, 0);
        buf[str_len] = 0;
        fprintf(stdout, "%s", buf);
        fflush(stdout);
        if (str_len == 0) {
            break;
        }
    }

    close(recv_sock);

    return 0;
}

void error_handling(char *message)
{

    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}