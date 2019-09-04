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
    int recv_buf, send_buf;
    int state;
    socklen_t optlen = sizeof(int);
    int tcp_sock = socket(PF_INET, SOCK_STREAM, 0);

    state = getsockopt(tcp_sock, SOL_SOCKET, SO_RCVBUF, &recv_buf, &optlen);
    if (state)
    {
        error_handle("getsockopt error");
    }
    fprintf(stdout, "socket recv_buf  %d\n", recv_buf);

    recv_buf = 10;
    state = setsockopt(tcp_sock, SOL_SOCKET, SO_RCVBUF, &recv_buf, optlen);
    if (state) {
        error_handle("getsockopt error");
    }
    fprintf(stdout, "socket recv_buf  %d\n", recv_buf);

    state = getsockopt(tcp_sock, SOL_SOCKET, SO_RCVBUF, &recv_buf, &optlen);
    if (state)
    {
        error_handle("getsockopt error");
    }
    fprintf(stdout, "socket recv_buf  %d\n", recv_buf);

    return 0;
}