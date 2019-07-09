/**********************************************************************mZ**
	> File Name: sevice_unblook.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月07日 星期日 19时02分39秒
 ************************************************************************/

#include <stdio.h>
#include "sock.h"
#include <sys/epoll.h>
#define MAX_EVENTS 10

int main() {
    int sock_listen, conn_sock[1024], nfds, epollfd;
    struct epoll_event ev, events[MAX_EVENTS];
    char ip_addr[20] = "192.168.2.93";
    sock_listen = startup(ip_addr, 7777);
    
    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    
    ev.events = EPOLLIN;
    ev.data.fd = sock_listen;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_listen, &ev) == -1) {
        perror("epoll_ctl : sock_listen");
        exit(EXIT_FAILURE);
    }

    char buf[1024][1024];
    int con = 1;
    memset(buf, 0, sizeof(buf));
    for(;;) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        for (int n = 0; n < nfds; ++n) {

            if (events[n].data.fd == sock_listen) {
                conn_sock[con] = my_accept(sock_listen);
                if (conn_sock[con] == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                printf("%d\n", conn_sock[con]);
                ev.events = EPOLLIN;
                ev.data.fd = conn_sock[con];
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock[con], &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }
                conn_sock[0] = con;
                con++;
            }
            for (int i = 1; i <= conn_sock[0]; i++) {
            if (events[n].data.fd == conn_sock[i]) {
                if (events[n].events == EPOLLIN) {
                read(conn_sock[i], buf[conn_sock[i]], 1024);
                ev.events = EPOLLOUT;
                epoll_ctl(epollfd, EPOLL_CTL_MOD, conn_sock[i], &ev);
                printf("EPOLLIN\n");
                } else if (events[n].events == EPOLLOUT){
                printf("EPOLLOUT\n");
                write(conn_sock[i], buf[conn_sock[i]], strlen(buf[conn_sock[i]]));
                memset(buf[conn_sock[i]], 0, 1024);
                ev.events = EPOLLIN;
                epoll_ctl(epollfd, EPOLL_CTL_MOD, conn_sock[i], &ev);    
                }
            }
            }    
        }
    }

    close(epollfd);
    return 0;
}
