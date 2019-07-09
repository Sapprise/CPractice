#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/epoll.h>

int socket_connect(int port, char *host, struct timeval *tv) {
	int sockfd, retval;
	struct sockaddr_in dest_addr;
    struct timeval *time;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error");
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
    int nfds, epollfd;
    epollfd = epoll_create(2);
    ev.events = EPOLLOUT;
    ev.data.fd = sockfd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
    printf("1\n");
    connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    nfds = epoll_wait(epollfd, events, 1, 3*60*60);

    if (nfds == -1) {
        perror("epoll_wait()");
    } else if (nfds > 0){
        printf("2\n");
	    return sockfd;
    } else {
        printf("connect fail!");
        return -1;
    }
    close(epollfd);
}

int main(int argc, char *argv[]) {
	int  socket_fd;
    FILE * fd;
	struct passwd *pwd;
    struct timeval tv;
    pwd = getpwuid(getuid());	
	char ip_addr[20] = "192.168.1.108";
	int port = 8888;
    char username[20] = {0};
    tv.tv_sec = 0;
    tv.tv_usec = 30000;
    strcpy(username, pwd->pw_name);
	socket_fd = socket_connect(port, ip_addr, &tv);
    if (socket_fd > 0) {
        printf("success!\n");
    }
	close(socket_fd);
	return 0;
}


