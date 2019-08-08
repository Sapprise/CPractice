/*************************************************************************
	> File Name: my_sock.h
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年08月08日 星期四 19时46分51秒
 ************************************************************************/

#ifndef _MY_SOCK_H
#define _MY_SOCK_H

#include <stdio.h>
#include <sys/types.h>   
#include <sys/socket.h>  
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <errno.h>

int socket_fixed(const char * , int); // 服务器固定套接字
int socket_connect(char *, int); //客户端请求连接
int my_accept(int); //服务器监听连接
int epoll_socket_connect(int port, char *host); //无阻塞连接

#endif
