/*************************************************************************
	> File Name: 3.PV.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年08月01日 星期四 09时31分54秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

int sem_id = 0;
char buff[1024] = {0};

int set_semvalue() {
    union semun sem_union;
    sem_union.val = 1;
    if(semctl(sem_id, 0, SETVAL, sem_union) == -1) {
        return 0;
    }
    return 1;
}

void del_semvalue() {
    union semun sem_union;
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
        printf("Falied to delect!\n");
    }
}

int semaphore_p() {
    struct sembuf sem_p;
    sem_p.sem_num = 0;
    sem_p.sem_op = -1;
    sem_p.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_p, 1) == -1) {
        printf("semaphore_p failed!\n");
        return 0;
    }
    return 1;
}

int semaphore_v() {
    struct sembuf sem_v;
    sem_v.sem_num = 0;
    sem_v.sem_op = 1;
    sem_v.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_v, 1) == -1) {
        printf("semaphore_v failed!\n");
        return 0;
    }
    return 1;
}

int main() {
    sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
    pid_t pid = fork();
    if (pid == 0) {
        while (1) {
            if (!semaphore_p()) {
                exit(1);
            }
            printf("child1 %s\n", buff);
            memset(buff, 0 , sizeof(buff));
            semaphore_v();
        }
        del_semvalue();
    } else {
        pid = fork();
        if (pid == 0) {
            printf("adad");
            while (1) {
                if (!semaphore_p()) {
                    exit(1);
                }
                printf("child2 %s\n", buff);
                memset(buff, 0 , sizeof(buff));
                semaphore_v();
            }
            del_semvalue();
        } else {
            //sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
            if (!set_semvalue()) {
                printf("set_semvalue failed!\n");
                exit(1);
            }
            usleep(10000);
            while (1) {
                semaphore_p();
                printf("father ->");
                scanf("%s", buff);
                if(!semaphore_v()) {
                    exit(0);
                }
            }
            del_semvalue();
        }
    }
    return 0;
}
