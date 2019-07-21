/************************************mZ***********************************
	> File Name: Mater.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月20日 星期六 14时32分31秒
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
#include <sys/epoll.h>
#include "sock.h"
#include "2xxx_practice.h"
#include "Linklist.h"

char Configurefile[50] = {0};

LinkList *Clilist;

//无阻塞连接client,等待100毫秒 ok
int epoll_connect_client(char *ip_client, char *key) {
    char port_client[20] = {0};
    strcpy(port_client, key);
    get_who_conf(Configurefile, port_client);
    printf("%s\n", port_client);
    int port = atoi(port_client);
    int sockfd = epoll_socket_connect(port, ip_client);
    
    return sockfd;
}

//对所以client进行心跳检测
int into_list_client(LinkList *l) {
    char client_st[30] = {0}, client_end[20] = {0};
    strcpy(client_st, "ip_client_st");
    strcpy(client_end, "ip_client_end");
    printf("%s %s\n", client_st, client_end);
    get_who_conf(Configurefile, client_st);
    get_who_conf(Configurefile, client_end);
    printf("%s %s\n", client_st, client_end);
    unsigned int star, end;
    star = inet_addr(client_st);
    end = inet_addr(client_end);
    printf("%u %u\n", star, end);



    return 1;
}


void *look_allclient(LinkList *list) {
    ListNode *p = list->head.next, *q = &list->head;
        
}

int main (int argc, char *argv[]) {
    strncpy(Configurefile, argv[1], strlen(argv[1]));
    char port[20];
    strcpy(port, "client_port");
    char ip[20];
    strcpy(ip, "192.168.2.93");
    Clilist = init_linklist();
    into_list_client(Clilist);

    return 0;
}
