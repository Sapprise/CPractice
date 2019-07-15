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
    int sock_listen, conn_sock, nfds, epollfd;
    struct epoll_event ev, events[MAX_EVENTS];
    char ip_addr[20] = "127.0.0.1";
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

    char buf[100][1024];
    memset(buf, 0, sizeof(buf));
    for(;;) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        for (int n = 0; n < nfds; n++) {
            if (events[n].data.fd == sock_listen) {
                conn_sock = my_accept(sock_listen);
                if (conn_sock == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                printf("%d\n", conn_sock);
                ev.events = EPOLLIN;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }
            } else {
                if (events[n].events == EPOLLIN) {
                    read(events[n].data.fd, buf[events[n].data.fd], 1024);
                    printf("EPOLLIN conn_sock = %d: %s\n", events[n].data.fd ,buf[events[n].data.fd]);

                    ev.events = EPOLLOUT;
                    ev.data.fd = events[n].data.fd;
                    if (epoll_ctl(epollfd, EPOLL_CTL_MOD, events[n].data.fd, &ev) == -1) {
                        perror("epoll_ctl: EPOLLOUT");
                    }
                } else if (events[n].events == EPOLLOUT){
                    printf("EPOLLPOUT conn_sock = %d: %s\n", events[n].data.fd ,buf[events[n].data.fd]);
                    write(events[n].data.fd, buf[events[n].data.fd], strlen(buf[events[n].data.fd]));
                    memset(buf[events[n].data.fd], 0, sizeof(buf[events[n].data.fd]));

                    ev.events = EPOLLIN;
                    ev.data.fd = events[n].data.fd;
                    if (epoll_ctl(epollfd, EPOLL_CTL_MOD, events[n].data.fd, &ev) == -1) {
                        perror("epoll_clt : EPOLLIN");
                    }
            
                }
            }
        }
    }

    close(epollfd);
    return 0;
}
