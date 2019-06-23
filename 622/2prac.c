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



int length[60];

struct my_st{
    int st_sock[30];
    int who;
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




void *ball(void *str) {
    struct my_st *st;
    st = (struct my_st*) str;
    char signal[2];
    int dir1, dir2;
    int id = st->who;
    printf("ball_id :%d ball_sock:%d", id, st->st_sock[id]);
    int le = 20, pos = 20;
    while (1) {
        read(st->st_sock[id], signal, sizeof(signal));
        //printf("第%d玩家操作:%s\n", id, signal );
        if (signal[0] == 'w') {
             dir1 = -1, dir2 = 0;
        }else if (signal[0] == 's') {
             dir1 = 1, dir2 =0;
        }else if (signal[0] == 'a') {
             dir1 = 0, dir2 = -1;
        }else if (signal[0] == 'd') {
             dir1 = 0, dir2 = 1;
        } else {
            dir1 = dir2 = 0;
        }
        if (le >= 45) dir1 = -1;
        if (le <= 9) dir1 = 1;
        if (pos >= 65) dir2 = -1;
        if (pos <= 10) dir2 = 1;
        le += dir1;
        pos += dir2;
        length[0] = st->who;
        length[id * 2 - 1] = le;
        length[id * 2] = pos;
        sleep(1);
    }
    printf("ball fail");
    return NULL;
}

void *fasong(void *str) {
    struct my_st *st;
    st = (struct my_st*) str;
    int id = st->who;
    printf("fasong_Id:%d\n", id);
    while(1) {
        write(st->st_sock[id], length, sizeof(length));
        printf(" 第%d玩家 %d %d\n", id, length[id * 2 -1], length[id * 2]);
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
        pthread_t thr1[30],thr2[30], thr3[30];
        int new_sock[30];
        while(1) {
            new_sock[id] = accept(listen_sock,(struct sockaddr*)&client, &len);
            str.st_sock[id] = new_sock[id];
            str.who = id;
            printf("new_sock:%d\n", new_sock[id]);
            if (new_sock[id] < 0) {
                perror("accept fail!\n");
                close(listen_sock);
                exit(-5);    
        }
            printf("第%d玩家连接\n", id);
            pthread_create(&thr1[id], NULL, ball, &str);
            pthread_create(&thr2[id], NULL, fasong, &str);
            id++;
    }
    return 0;
}
