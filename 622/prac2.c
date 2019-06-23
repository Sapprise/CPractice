/*************************************************************************
	> File Name: prac2.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年06月22日 星期六 14时37分47秒
 ************************************************************************/

#include <stdio.h>
#include <curses.h>
#include <sys/types.h>   
#include <sys/socket.h>  
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>


#define LEFTEDGE 10
#define RIGHTEDGE 30
#define ROW 10

int length[7];
int big[7] = {10, 30};
struct my_st{
    int st_sock;
};

int startup(const char * _ip, int _port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("sock fail\n");
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
	if (listen(sock, 20) != 0) {
		perror("----listen----fail\n");
		close(sock);
		exit(-1);
	}
    printf("Listening!\n");
	return sock;		
}



void *ball() {
    char ball[2] = "O";
    char blank[2] = " ";
    int dir = 1;
    int pos = LEFTEDGE;
    initscr();
    clear();
    
    while (1) {
        move(ROW, pos);
        length[0] = (int)ROW;
        length[1] = pos;
        addstr(ball);
        move(LINES - 1, COLS - 1);
        refresh();
        sleep(1);
        move(ROW, pos);
        addstr(blank);
        length[0] = (int)ROW;
        length[1] = pos;

        pos += dir;

        if (pos >= RIGHTEDGE) dir = -1;
        if (pos <= LEFTEDGE) dir = 1;
    }
    printf("ball fail");
    return NULL;
}

void *fasong(void *str) {
    struct my_st *st;
    st = (struct my_st*) str;
    write(st->st_sock, big, sizeof(big));
    while(1) {
        write(st->st_sock, length, sizeof(length));
        sleep(1);
    }
    return NULL;
}


int main() {
        char ip_addr[20] = "192.168.2.93";
        int listen_sock = startup(ip_addr, 8888);
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        struct my_st str;
        int id = 1;
        pthread_t thr1[20000],thr2[20000];
        pthread_create(&thr1[id], NULL, ball, NULL);
        while(1) {
            int new_sock = accept(listen_sock,(struct sockaddr*)&client, &len);
            str.st_sock = new_sock;
            if (new_sock < 0) {
                perror("accept fail!\n");
                close(listen_sock);
                exit(-5);    
        }
            pthread_create(&thr2[id], NULL, fasong, &str);
            id++;
    }
    return 0;
}
