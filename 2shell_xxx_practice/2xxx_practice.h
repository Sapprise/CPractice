/*************************************************************************
	> File Name: 2xxx_practice.h
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月20日 星期六 15时14分44秒
 ************************************************************************/

#ifndef _2XXX_PRACTICE_H
#define _2XXX_PRACTICE_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_conf(char *file, char *key, char *val);//获取配置文件信息
void get_who_conf(char *file, char *who);//获取配置文件特定信息
int my_inet_ntoa(unsigned int ,char *); //将无符号整形转化为IP地址
unsigned int my_inet_atoi(char *); //将ＩＰ转化为无符号整形
union My_IP {
    unsigned int num;
    struct oneip{
    unsigned char c1;
    unsigned char c2;
    unsigned char c3;
    unsigned char c4;
    }oneip;
};

union Ip {
    unsigned int num;
    char part[4];
};

int my_inet_ntoa(unsigned int num_ip, char *need_ip) {
    union My_IP myip;
    myip.num = num_ip;
    char ip[20] = {0};
    sprintf(ip,"%d.%d.%d.%d", myip.oneip.c4, myip.oneip.c3, myip.oneip.c2, myip.oneip.c1);
    strncpy(need_ip, ip, strlen(ip));
    return 0;
}

unsigned int my_inet_atoi(char *str_ip) {
    int num[4];
    sscanf(str_ip, "%d.%d.%d.%d", num + 3, num + 2, num + 1, num + 0);
    union Ip ip;
    for (int i = 3; i >= 0; --i) {
        ip.part[i] = num[i] & ((1 << 8) - 1);
    }
    return ip.num;
}

void get_who_conf(char *file, char *who) {
    char val[100] = {0};
    get_conf(file, who, val);
    memset(who, 0, strlen(who));
    strncpy(who, val, strlen(val));
    return ;
}

int get_conf(char *file, char *key, char *val) {
    FILE *fp;
    char *line = NULL, *substr = NULL;
    size_t n = 0, len = 0;
    ssize_t read;
    
    if (key == NULL || val == NULL) {
        perror("get_conf():");
        return -1;
    }

    fp = fopen(file, "r");

    if (fp == NULL) {
        return -1;
    }

    while ((read = getline(&line, &n, fp)) ) {
        substr = strstr(line, key);
        if (substr == NULL) {
            continue;
        } else {
            len = strlen(key);
            if (line[len] == '=') {
                strncpy(val, line + len + 1, (int)read - len - 2);
                break;
            } else continue;
        }
    }
    if (substr == NULL) {
        printf("%s not find in %s!\n", key, file);
        fclose(fp);
        free(line);
        return -1;
    } 
    fclose(fp);
    free(line);
    return 0;
}

#endif
