/*************************************************************************
	> File Name: server.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年06月18日 星期二 13时08分18秒
 ************************************************************************/


#include <stdio.h>
#include <sys/types.h>   
#include <sys/socket.h>  
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
 
struct my_st{
    int st_sock;
    FILE *st_fp;
};

int startup(const char* _ip, int _port)
{
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
	if (listen(sock, 20) != 0) {
		perror("----listen----fail\n");
		close(sock);
		exit(-1);
	}
    printf("Listening!\n");
	return sock;		
}
 
void *creatflie(void* str) {
    FILE *fd;
    char *path;
    char buf[1024];
    struct my_st *st;
    st = (struct my_st*) str;
    read(st->st_sock, buf, 10);
    path = (char *)malloc(sizeof(char) * (sizeof(buf) + 1));
    strncpy(path, buf, sizeof(buf));
    fd = fopen(path, "w+");
    st->st_fp = fd;
    memset(buf, '\0', sizeof(buf));
    free(path);
    printf("creat file success!\n");
    return NULL;
}

void *writeflie(void * str) {
    struct my_st *st;
    st = (struct my_st*) str;
    char buf[1024];
    while(1) {
    memset(buf, '\0', sizeof(buf));
    int s = read(st->st_sock, buf, 1);
    if (s > 0) {
        fwrite(buf, s, 1, st->st_fp);
    } else if ( s == 0 ) {
        printf("client quit \n");
        break;
    } else break;
    }
    printf("wrire success!\n");
    return NULL;
}


int main()
{
    char ip_addr[20] = "192.168.2.93";
	int listen_sock = startup(ip_addr, 8888);
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
    struct my_st str;
    int id = 1;
    pthread_t thr1[20000],thr2[20000];
	while(1) {
		int new_sock = accept(listen_sock,(struct sockaddr*)&client, &len);
        printf("链接成功\n");
        str.st_sock = new_sock;
		if (new_sock < 0) {
			perror("accept fail!\n");
			close(listen_sock);
			exit(-5);
		}
        printf("第%d次\n", id);
        sleep(1);
        pthread_create(&thr1[id], NULL, creatflie, &str);
        pthread_join(thr1[id], NULL);
        pthread_create(&thr2[id], NULL, writeflie, &str); 
        pthread_join(thr2[id], NULL);
        fclose(str.st_fp);
        close(new_sock);
        id++;
    }
	return 0;
}
