/*************************************************************************
	> File Name: 1共享内存.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月28日 星期日 18时43分02秒
 ************************************************************************/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

struct sm_msg{
    char buff[1024];
    pthread_mutex_t sm_mutex;
    pthread_cond_t sm_sm_ready;
};

int main() {
    int statu;
    int shm_id = shmget(IPC_PRIVATE, 1024 * 4, 0664);
    char *p_mat, *c_mat;
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        c_mat = shmat(shm_id, NULL, 0);
        memset(c_mat, 0, sizeof(c_mat));
        while (1) {
            memset(c_mat, 0, sizeof(c_mat));
            printf("Children :");
            scanf("%s", c_mat);
            usleep(200000);
        }
    }

    if (pid > 0) {
        p_mat = shmat(shm_id, NULL, 0);
        int len = 0;
        while (1) {
            len = strlen(p_mat);
            if (len != 0) {
                printf("parent :%s\n", p_mat); 
                memset(p_mat, 0, sizeof(c_mat));
            } 
            usleep(100000);
        }
    }

    return 0;
}
