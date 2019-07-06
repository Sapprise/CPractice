/*************************************************************************
	> File Name: flock_len.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月04日 星期四 20时43分35秒
 ************************************************************************/

#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MAX 10000
#define LIN 20

void get_num(long long *num, char *file) {
    FILE *fp = fopen(file, "r");
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, fp);
    *num = atoll(line);
    free(line);
    fclose(fp);
}

void set_num(long long num, char *file) {
    FILE *fp = fopen(file, "w");
    fprintf(fp, "%lld", num);
    fclose(fp);
}

int main() {
    FILE *fp = fopen("num", "w");
    FILE *fp_res = fopen("res", "w");
    pid_t pid;
    flock(fp->_fileno, LOCK_EX);
    flock(fp_res->_fileno, LOCK_EX);
    set_num(0, "num");
    set_num(0, "res");
    printf("已加锁\n");

    for (int i = 0; i < LIN; i++) {
        pid = fork();
        if (pid == 0) break;
    }
    if (pid == 0) {
        fclose(fp);
        fclose(fp_res);
        //printf("in child!\n");
        long long num, res, num_add;
    while(1) {
        fp = fopen("num", "rw");
        fp_res = fopen("res", "rw");

        flock(fp->_fileno, LOCK_EX);
        get_num(&num, "num");
        num_add = num + 1;
        set_num(num_add, "num");
        //fflush(stdout);
        flock(fp->_fileno, LOCK_UN);
        fclose(fp);
        if (num > MAX) break;

        flock(fp_res->_fileno, LOCK_EX);
        get_num(&res, "res");
        res = res + num;
        set_num(res, "res");
        flock(fp_res->_fileno, LOCK_UN);
        fclose(fp_res);        
    }
        exit(0);
    } 
    fclose(fp);
    fclose(fp_res);
    for (int i = 0; i < LIN; i++) {
        waitpid(-1, NULL, 0);
    }
    long long result;
    get_num(&result, "res");
    printf("%lld\n", result);
    return 0;

}
