/*************************************************************************
	> File Name: home.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月05日 星期五 21时20分14秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>


int deal_num(char filename) {
    FILE *fp1, *fp2;
    int *num, result, num_add;
    char *line = NULL;
    size_t len = 0;
    fp1 = fopen("num", "rw");
    fp2 = fopen("res", "rw");
    flock(fp1, LOCK_EX);
    getline(&line, &len, fp1);
    *num = atoi(line);
    num_add = num + 1;
    if (0 == flock(fp2->_fileno, LOCK_EX)) {

    }
}

int main() {
    pid_t pid;
    for (int i = 0; i < 10; i++) {
        pid = fork();
        if (pid == 0) break;
    }

    if (pid == 0) {
        FILE *fp1, *fp2;
        fp1 = fopen("num", "rw");
        fp2 = fopem("res", "rw");
        while (1) {
            if (flock(fp1->_fileno, LOCK_EX) == 0 && flock(fp2->_fileno, LOCK_EX) == 0) {
                
            }
        }

    }


    return 0;
}
