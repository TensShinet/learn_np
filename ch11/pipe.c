#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
    int fds1[2], fds2[2];
    char str1[] = "Love You!\n";
    char str2[] = "Thank you for your message\n";

    // 建立管道
    pipe(fds1);
    pipe(fds2);

    pid_t pid = fork();
    
    if(pid == 0) {
        // 子进程给父进程发消息
        char buf[BUF_SIZE];
        write(fds2[1], str1, sizeof(str1));
        size_t str_len = read(fds1[0], buf, BUF_SIZE);
        buf[str_len] = 0;
        fputs(buf, stdout);
    } else {
        char buf1[BUF_SIZE];
        size_t str_len = read(fds2[0], buf1, BUF_SIZE);
        buf1[str_len] = 0;
        fputs(buf1, stdout);
        write(fds1[1], str2, sizeof(str2));
    }

    return 0;
}