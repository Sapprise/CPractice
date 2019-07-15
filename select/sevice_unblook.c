/*************************************************************************
	> File Name: sevice_unblook.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月07日 星期日 19时02分39秒
 ************************************************************************/

#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include "sock.h"


#define MAX_CLIENTS 40
#define PORT 7777
#define MAX_BUFF 1024

typedef struct server_data {
    int client_cnt;
    int client_fds[MAX_CLIENTS];
    fd_set all_fds;
    int maxfd;
} server_data;

server_data *s_d = NULL;


void accept_client(int server_listen) {
    int client_fd = -1;
    printf("befor accept!\n");
    fflush(stdout);
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    client_fd = accept(server_listen, (struct sockaddr *)&client, &len);
    if (client_fd == -1) {
        return ;
    }

    for (int i = 0; i <MAX_CLIENTS; i++) {
        if (i >= MAX_CLIENTS) {
            printf("Too many clients!\n");
            return ;
        }
        if (s_d->client_fds[i] < 0) {
            s_d->client_fds[i] = client_fd;
            s_d->client_cnt++;
            break;
        }
    }
}

void recv_send(fd_set *readfds) {
    int client_fd;
    char buff[MAX_BUFF] = {0};
    int num;
    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    for (int i = 0; i < s_d->client_cnt; i++) {
        client_fd = s_d->client_fds[i];
        if (client_fd < 0) {
            continue;
        }
        if (FD_ISSET(client_fd, readfds)) {
            num = recv(client_fd, buff, MAX_BUFF, 0);
            if (num <= 0) {
                FD_CLR(client_fd, &(s_d->all_fds));
                close(client_fd);
                s_d->client_fds[i] = -1;
                continue;
            }
            getpeername(client_fd, (struct sockaddr *)&client, &len);
            printf("recv <%s> : %s", inet_ntoa(client.sin_addr), buff);
            num = send(client_fd, buff, strlen(buff), 0);
            if (num > 0) {
                printf(" [sent true]\n");
            } else {
                printf(" [sent false]\n");
            }
        }
    }
}

void do_client(int server_listen) {
    int client_fd = -1;
    int ret = 0;
    fd_set *readfds = &(s_d->all_fds);
    struct timeval tm;
    while (1) {
        FD_ZERO(readfds);
        FD_SET(server_listen, readfds);
        s_d->maxfd  = server_listen;

        tm.tv_sec = 5;
        tm.tv_usec = 0;

        for (int i = 0; i < s_d->client_cnt; i++) {
            client_fd  = s_d->client_fds[i];
            if (client_fd != -1) {
                FD_SET(client_fd, readfds);
            }
            s_d->maxfd  = (client_fd > s_d->maxfd ? client_fd : s_d->maxfd);
        }

        ret = select(s_d->maxfd + 1, readfds, NULL, NULL, &tm);

        if (ret == 0) {
           printf("Time out!\n");
            return ;
        }

        if (ret == -1) {
            printf("select");
            return ;
        }

        if (FD_ISSET(server_listen, readfds)) {
            printf("befor accept!\n");
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            client_fd = accept(server_listen, (struct sockaddr *)&client, &len);
        if (client_fd == -1) {
            return ;
        }
            printf("after accept!\n");     
        } else {
            recv_send(readfds);
        }

    }
}

int main() {
    int server_listen;
    char ip_addr[20] = "192.168.2.93";
    s_d = (server_data *)malloc(sizeof(server_data));
    memset(s_d, 0, sizeof(server_data));
    
    for(int i = 0; i < MAX_CLIENTS; i++) {
        s_d->client_fds[i] = -1;
    }

    s_d->client_cnt = 0;

    server_listen = startup(ip_addr, 8888);
    if (server_listen < 0) {
        perror("startup");
        exit(0);
    }
    printf("Init Listen socket %d!\n", server_listen);
    do_client(server_listen);
    return 0;
}
