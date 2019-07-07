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

int socket_connect(int port, char *host, struct timeval *tv) {
	int sockfd, retval;
	struct sockaddr_in dest_addr;
    struct timeval *time;
    fd_set rfds;
    time = (struct timeval *) tv;
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
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);

    int error = -1;
    int len;
    len = sizeof(error);
	//DBG("Connetion TO %s:%d\n",host,port);
	//fflush(stdout);
    printf("1\n");
    connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    retval = select(sockfd + 1, NULL, &rfds, NULL, time);


    if (retval == -1) {
        perror("select()");
    } else if (retval > 0){
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len)  < 0) {
            perror("getsockopt()");
            return -1;
        }
        printf("2\n");
	    return sockfd;
    } else {
        printf("connect fail!");
        return -1;
    }
		//perror("connect() error");
		//DBG("connect() error : %s!\n", stderror(errno));
	//return -1;
}

int main(int argc, char *argv[]) {
	int  socket_fd;
    FILE * fd;
	struct passwd *pwd;
    struct timeval tv;
    pwd = getpwuid(getuid());	
	char ip_addr[20] = "192.168.2.93";
	int port = 8888;
    char username[20] = {0};
    tv.tv_sec = 0;
    tv.tv_usec = 30000;
    strcpy(username, pwd->pw_name);
	socket_fd = socket_connect(port, ip_addr, &tv);
    if (socket_fd > 0) {
        printf("success!\n");
    }
    char buf[1024];
    char string;
    memset(buf, '\0', sizeof(buf));
    strcpy(buf, "ad");
    printf("%s\n", buf);
    write(socket_fd, buf, sizeof(buf));
    sleep(1);
    char str[1024];
    read(socket_fd, str, sizeof(str));
    printf("%s\n", str);
	close(socket_fd);
	return 0;
}


