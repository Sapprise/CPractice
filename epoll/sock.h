/*************************************************************************
	> File Name: sock.h
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年06月22日 星期六 18时25分41秒
 ************************************************************************/

#ifndef _SOCK_H
#define _SOCK_H

#include <stdio.h>
#include <sys/types.h>   
#include <sys/socket.h>  
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int startup(const char * , int); // 服务器固定套接字
int socket_connect(int , char *); //客户端请求连接
int my_accept(int listen_sock); //服务器监听连接

int my_accept(int listen_sock) {
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int new_sock = accept(listen_sock, (struct sockaddr *)&client, &len);
    if (new_sock < 0) {
        perror("accept()");
        close(listen_sock);
        return -1;
    }
    return new_sock;
}

int startup(const char* _ip, int _port) {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("----sock----fail\n");
		exit(-1);
	}
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr(_ip);
	local.sin_port = htons(_port);
	if (bind(sock, (struct sockaddr*)&local, sizeof(local)) != 0) {
		perror("---bind---fail\n");
		close(sock);
		exit(-2);
	}
	if (listen(sock, 1000) != 0) {
		perror("----listen----fail\n");
		close(sock);
		exit(-1);
	}
	return sock;		
}

int socket_connect(int port, char *host) {
	int sockfd;
	struct sockaddr_in dest_addr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error");
        return -1;  
	}

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = inet_addr(host);

	//DBG("Connetion TO %s:%d\n",host,port);
	//fflush(stdout);
	if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
		//perror("connect() error");
		//DBG("connect() error : %s!\n", stderror(errno));
		return -1;
	}
	return sockfd;

}
#endif
