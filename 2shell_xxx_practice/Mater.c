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
    int port = atoi(port_client);
    int sockfd = epoll_socket_connect(port, ip_client);
    printf("epoll sockfd :%d\n", sockfd);
    
    return sockfd;
}

//对所以client进行心跳检测
int into_list_client(LinkList *l) {
    char client_st[30] = {0}, client_end[20] = {0};
    strcpy(client_st, "ip_client_st");
    strcpy(client_end, "ip_client_end");
    get_who_conf(Configurefile, client_st);
    get_who_conf(Configurefile, client_end);
    unsigned int star, end;
//将ip转化为无符号整形
    star = my_inet_atoi(client_st);
    end = my_inet_atoi(client_end);

    for (unsigned int i = star; i <= end; i++) {
        if (!insert(l, l->length, i)) {
            printf("insert fault!\n");
        }
    }

//遍历整个链表删除不在线client
    ListNode *p = l->head.next, *q = &l->head;
    int allclient = l->length;
    char port[20];
    strcpy(port, "client_port");
    for (int i = 0; i < allclient; i++) {
        unsigned int num_client_ip;
        char str_client_ip[20] = {0};
        int client_socket;
        num_client_ip = p->data;
        my_inet_ntoa(num_client_ip, str_client_ip);
        client_socket = epoll_connect_client(str_client_ip, port);
        if (client_socket != -1) {
            p = p->next;
            q = q->next;
            close(client_socket);
        } else {
            printf("%u\n", p->data);
            q->next = p->next;
            free(p);
            l->length -= 1;
            p = q->next;
        }
        memset(str_client_ip, 0, sizeof(str_client_ip));
        printf("%d\n", l->length);
    }
    return 1;
}


void *look_allclient(LinkList *list) {
    ListNode *p = list->head.next, *q = &list->head;
        
}

int main (int argc, char *argv[]) {
    strncpy(Configurefile, argv[1], strlen(argv[1]));
    char ip[20];
    strcpy(ip, "127.0.0.1");
    Clilist = init_linklist();//初始化一个链表
    into_list_client(Clilist);
    return 0;
}
