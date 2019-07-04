/*************************************************************************
	> File Name: flock_len.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月04日 星期四 20时43分35秒
 ************************************************************************/

#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>

int main() {
    FILE *fp = fopen("a", "w");
    pid_t pid;
    flock(fp->_fileno, LOCK_EX);

    printf("已加锁\n");

    pid = fork();
    if (pid == 0) {
        close(fp);
        printf("in child!\n");
        fp = fopen("a", "r");
        flock(fp->_fileno, LOCK_EX);
        printf("after try flock!\n");
        fflush(stdout);
        exit(0);
    }

}
