/*************************************************************************
	> File Name: my_ip_infor.h
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年08月08日 星期四 20时09分48秒
 ************************************************************************/

#ifndef _MY_IP_INFOR_H
#define _MY_IP_INFOR_H

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

#endif
