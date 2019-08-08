/*************************************************************************
	> File Name: sock.h
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年06月22日 星期六 18时25分41秒
 ************************************************************************/

#include "my_sock.h"

int my_accept(int listen_sock) {
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int new_sock = accept(listen_sock, (struct sockaddr *)&client, &len);
    if (new_sock < 0) {
        //perror("accept()");
        close(listen_sock);
        return -1;
    }
    return new_sock;
}

int socket_fixed(const char* _ip, int _port) {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		//perror("----sock----fail\n");
		return -1;
	}
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr(_ip);
	local.sin_port = htons(_port);

    int yes = 1;        //端口重用
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        //perror("setsockopt");
    }

	if (bind(sock, (struct sockaddr*)&local, sizeof(local)) != 0) {
		//perror("---bind---fail\n");
		close(sock);
        return -1;
		//exit(-2);
	}
	if (listen(sock, 1000) != 0) {
		//perror("----listen----fail\n");
		close(sock);
        return -1;
		//exit(-1);
	}
    //printf("bind success!\n");
	return sock;		
}

int socket_connect(char *host, int port) {
	int sockfd;
	struct sockaddr_in dest_addr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		//perror("socket() error");
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

int epoll_socket_connect(int port, char *host) {
    int sockfd, ret;
	struct sockaddr_in dest_addr;
    struct timeval *time;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		//perror("socket() error");
        return -1;  
	}
	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = inet_addr(host);

    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
    unsigned long i = 1;
    ioctl(sockfd, FIONBIO, &i);

    struct epoll_event ev, events[10];
    int nfds, epollfd, error = -1, len;
    len = sizeof(error);

    epollfd = epoll_create(2);
    ev.events = EPOLLOUT;
    ev.data.fd = sockfd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
    nfds = connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    nfds = epoll_wait(epollfd, events, 2, 100);
        if (nfds == -1) {
            //perror("epoll_wait()");
            ret = -1;
        } else if (nfds > 0){
            if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len) < 0) {
                close(sockfd);
                ret = -1;
            } 
            if (error == 0) {
                ret = sockfd;
            } else {
                close(sockfd);
                ret = -1;
            }
        } else {
            //printf("connect fail!");
            ret =  -1;
        }        
    close(epollfd);
    return ret;
}

