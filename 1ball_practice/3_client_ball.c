#include <stdio.h>
#include <curses.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <pthread.h>

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

int  socket_fd;

void *ball() {
    char people[2] = "K";
    char ball[2] = "o";
    char blank[2] = " ";
    char dian[2] = ".";
    char xin[2] = "*";
    char men[2] = "|";
    int dir = 1;
    int length[60];
    initscr();
    clear();
    for(int i = 10; i< 65; i++) {
        move(9, i);
        addstr(xin);
        refresh();
        move(27, i);
        addstr(dian);
        refresh();
        move(46, i);
        addstr(xin);
        refresh();
    }
    for(int j = 9; j < 46; j++) {
        move(j, 9);
        addstr(xin);
        refresh();
        move(j, 37);
        addstr(dian);
        refresh();
        move(j, 66);
        addstr(xin);
        refresh();
    } 
    for(int j = 26; j < 29; j++) {
        move(j, 9);
        addstr(men);
        refresh();
        move(j, 66);
        addstr(men);
        refresh();
    } 
    while(1) {
        read(socket_fd, length, sizeof(length));
        for (int id = 2; id <= length[0]; id++) {
            move(length[1], length[2]);
            addstr(ball);
            move(length[id * 2 - 1], length[id * 2]);
            addstr(people);
            move(LINES - 1, COLS - 1);
            refresh();
            usleep(4000);
            move(length[id * 2 - 1], length[id * 2]);
            addstr(blank);
            move(length[1], length[2]);
            addstr(blank);
        }
        //refresh();
    }
    return 0;
}

char signal[2];
void *fasong() {
    while(1) {
        signal[0] = getchar();    
        usleep(90000);
        signal[0] = 'q';
    }
    return NULL;
}
int main(int argc, char *argv[]) {
	struct passwd *pwd;
    pwd = getpwuid(getuid());	
	char ip_addr[20] = "192.168.1.112";
	int port = 8888;
    char username[20] = {0};
    strcpy(username, pwd->pw_name);
	socket_fd = socket_connect(port, ip_addr);
    if (socket_fd > 0) {
     printf("success!\n");
}
    //int length[60];
    int i = 2;
    pthread_t thr1, thr2;
    pthread_create(&thr1, NULL, ball, NULL);
    pthread_create(&thr2, NULL, fasong, NULL);
    while(1) {
        //read(socket_fd, length, sizeof(length));
        //printf("%d %d\n", length[1], length[2]);
        //signal[0] = getchar(); 
        write(socket_fd, signal, sizeof(signal));
        usleep(90000);
    }
	close(socket_fd);
	return 0;
}


