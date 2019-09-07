/*************************************************************************
	> File Name: 2.mmap.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月29日 星期一 18时48分37秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int fd, statu;
    char *child, *father;
    fd = open("./mmap", O_RDWR);
    pid_t pid = fork();
    if (pid == 0) {
        //fd = open("./mmap", O_RDWR);
        child = mmap(NULL, 1024*4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        memset(child, 0, sizeof(child));
        while (1) {
            printf("Child write:");
            scanf("%s", child);
            usleep(200000);
        }
        munmap(child, 1024*4);
        close(fd);
    }else {
        //fd = open("mmap", O_RDWR);
        father = mmap(NULL, 1024*4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        int len = 0;
        while(1) {
            len = strlen(father);
            if (len != 0) {
                printf("Father printf:%s\n", father);
                memset(father, 0, sizeof(father));
            }
        }
        munmap(father, 1024*4);
        close(fd);
    }
    return 0;

}
