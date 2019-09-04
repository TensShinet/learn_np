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
    int sock_type1, sock_type2;
    int state;
    socklen_t optlen = sizeof(sock_type1);
    int tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    int udp_sock = socket(PF_INET, SOCK_DGRAM, 0);

    fprintf(stdout, "tcp type %d\n", SOCK_STREAM);
    fprintf(stdout, "udp type %d\n", SOCK_DGRAM);

    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, &sock_type1, &optlen);
    if(state) {
        error_handle("getsockopt error");
    }
    fprintf(stdout, "socket type one %d\n", sock_type1);



    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, &sock_type2, &optlen);
    if(state) {
        error_handle("getsockopt error");
    }
    fprintf(stdout, "socket type one %d\n", sock_type2);

    return 0;
}