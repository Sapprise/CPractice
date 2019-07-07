/*************************************************************************
	> File Name: sevice_unblook.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月07日 星期日 19时02分39秒
 ************************************************************************/

#include <stdio.h>
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
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    client_fd = accept(server_listen, (struct sockadd *)&client, &len);

}

void recv_send(fd_set *readfds) {
    int client_fd;
    char buff[MAX_BUFF] = {0};
    int num;
    struct sockaddr_in client;
    socklen_t len;
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
                s_d->client_cnt--;
                continue;
            }
            getpeername(client_fd, (struct sockaddr *)&client, &len);
            printf("<%s> : %s", ip, buff);
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
            if (client_fds != -1) {
                FD_SET(client_fd, readfds);
            }
            s_d->maxfd  = (client_fd > s_d->maxfd ? client_fd : s_d->max);
        }

        ret = select(s_d->maxfd + 1, readfds, NULL, NULL, &tm);

        if (ret == 0) {
            
        }

        if (ret == -1) {
            
        }

    }
}

int main() {
    int server_listen;
    char ip_addr[20] = "192.168.2.93";
    s_d = (server_data *)malloc(sizeof(server_data));

    server_listen = startup(ip_addr, 8888);

    return 0;
}
