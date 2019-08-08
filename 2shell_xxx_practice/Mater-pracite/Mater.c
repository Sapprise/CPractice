/************************************mZ***********************************
	> File Name: Mater.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月20日 星期六 14时32分31秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/file.h>
#include <stdarg.h>
#include "my_sock.h"
#include "my_ip_infor.h"
#include "my_Linklist.h"

#define MAX_EVENTS 1000
#define Length 1024

struct my_st {
    char f[256];
    LinkList *List;
};

char Configurefile[128] = {0};
char filename[256] = {0};
char journal[256] = {0};
pthread_mutex_t mut;
LinkList *Clilist;

void my_perror(int r, char *jour, char *format, ...) {
    char result[1024] = {0}, error[512] = {0}, thetime[40] = {0}, war[20] = {0};
    va_list list;
    time_t t;
    struct tm *pt;
    FILE *fp;
    time(&t);
    pt = localtime(&t);
    sprintf(thetime,"%d-%d-%d %d:%d:%d  ", pt->tm_year + 1900, pt->tm_mon + 1, pt->tm_mday, pt->tm_hour, pt->tm_min, pt->tm_sec);
    va_start(list, format);
    vsprintf(error, format, list);
    va_end(list);
    
    if (r == 1) {
        strcpy(war, "[Note]");
    }
    if (r == 2) {
        strcpy(war, "[Waring]");
    }
    if (r == 3) {
        strcpy(war, "[True]");
    }
    sprintf(result, "%s %s %s\n", thetime, war, error);
    fp = fopen(jour, "a+");
    flock(fp->_fileno, LOCK_EX);
    fwrite(result, strlen(result), 1, fp);
    flock(fp->_fileno, LOCK_UN);
    fclose(fp);
    return ;
}


//UDP监听警告
void lister_udp(char *file) {
    char udp_port[20] = {0};
    int sockfd, port;
    strcpy(udp_port, "udp_port");
    get_who_conf(file, udp_port);
    port = atoi(udp_port);
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    char buff[512] = {0};
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        //perror("sockket");
        //exit(1);
        char *error;
        error = strerror(errno);
        my_perror(2, journal, "%s", error);
    }
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        //perror("connect");
        //exit(1);
        char *error;
        error = strerror(errno);
        my_perror(2, journal, "%s", error);
    }

    while(1) {
        bzero(buff, sizeof(buff));
        recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&addr, &len);
        char error1[10] = {0}, error2[256] = {0};
        sprintf(error1,"警告! :");
        sprintf(error2,"rectfrom %s  = %s \n", inet_ntoa(addr.sin_addr) , buff);
        my_perror(3, journal, "%s%s", error1, error2);
    }
    close(sockfd);
}


//监听client连接
int lister_client(char *file, LinkList *l) {
    char val[100] = {0};
    char ip_master[50] = {0}, port_master[20] = {0};
    strcpy(ip_master, "master");
    strcpy(port_master, "master_port");
    get_who_conf(file, ip_master);
    get_who_conf(file, port_master);
    int port = atoi(port_master);
    int listen_socket = socket_fixed(ip_master, port);

    printf("------监听client端是否连接:-------\n");
    int epollfd, nfds, conn_sock;
    struct epoll_event ev, events[MAX_EVENTS];
    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        //perror("epoll_create1");
        //exit(EXIT_FAILURE);
        char *error;
        error = strerror(errno);
        my_perror(2, journal, "%s", error);
    }
    
    ev.events = EPOLLIN;
    ev.data.fd = listen_socket;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_socket, &ev) == -1) {
        //perror("epoll_ctl: sock_listen");
        //exit(EXIT_FAILURE);
        char *error;
        error = strerror(errno);
        my_perror(2, journal, "%s", error);
    }
    for(;;) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            //perror("epoll_wait");
            //exit(EXIT_FAILURE);
            char *error;
            error = strerror(errno);
            my_perror(2, journal, "%s", error);
        }
        for (int n = 0; n < nfds; n++) {
            if (events[n].data.fd == listen_socket) {
                conn_sock = my_accept(listen_socket);
                
                if (conn_sock == -1) {
                    //perror("my_accept");
                    //exit(EXIT_FAILURE);
                    char *error;
                    error = strerror(errno);
                    my_perror(2, journal, "%s", error);
                }
                pthread_mutex_lock(&mut);  //上锁
                //将ip插入链表，并关闭连接。（查重）
                //printf("client_socket:%d\n", conn_sock);
                struct sockaddr_in peerAddr;
                int peerLen, repeat = 0;
                unsigned int ip;
                char ipAddr[INET_ADDRSTRLEN];
                ListNode *p = l->head.next;
                peerLen = sizeof(peerAddr);
                getpeername(conn_sock, (struct sockaddr *)&peerAddr, &peerLen);
                inet_ntop(AF_INET, &peerAddr.sin_addr, ipAddr, sizeof(ipAddr));
                char error[128] = {0};
                sprintf(error,"%s 成功请求连接", ipAddr);
                my_perror(3, journal, "%s", error);
                ip = my_inet_atoi(ipAddr); 
                while (p) {
                    if (p->data == ip) {
                        repeat = 1;
                        break;
                    }
                    p = p->next;
                }
                if (repeat == 0) {
                    insert(l, l->length, ip);
                } 
                pthread_mutex_unlock(&mut);                         //解锁
                close(conn_sock);
            } 
        }
    }
    return 0;
}

//无阻塞连接client,等待100毫秒 ok
int epoll_connect_client(char *ip_client, char *key) {
    char port_client[20] = {0};
    strcpy(port_client, key);
    get_who_conf(Configurefile, port_client);
    int port = atoi(port_client);
    int sockfd = epoll_socket_connect(port, ip_client);
    return sockfd;
}

//遍历整个链表删除不在线client ok
void look_allclient(LinkList *l) {
    pthread_mutex_lock(&mut);
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
            //printf("client_heart:%d\n", client_socket);
            close(client_socket);
        } else {
            q->next = p->next;
            free(p);
            l->length -= 1;
            p = q->next;
        }
        memset(str_client_ip, 0, sizeof(str_client_ip));
        //printf("%d\n", l->length);
    }
    pthread_mutex_unlock(&mut);           
    return ;    
}

//对client进行心跳检测 ok
int into_list_client(LinkList *l) {
    char client_st[30] = {0}, client_end[20] = {0};
    strcpy(client_st, "ip_client_st");
    strcpy(client_end, "ip_client_end");
    get_who_conf(Configurefile, client_st);
    get_who_conf(Configurefile, client_end);
    unsigned int star, end;

    star = my_inet_atoi(client_st);            //将ip转化为无符号整形
    end = my_inet_atoi(client_end);

    pthread_mutex_lock(&mut);
    for (unsigned int i = star; i <= end; i++) {
        if ((i % 256 == 0) || (i % 256 == 255)) {
            continue;
        }
        if (!insert(l, l->length, i)) {
            char error[128] = {0};
            sprintf(error,"insert %u fault!", i);
            my_perror(1, journal, "%s", error);
        }
    }
    pthread_mutex_unlock(&mut);                 
    //printf("-----对client进行心跳检测----------\n");
    look_allclient(l);
    
    while (1) {
        sleep(5);
        look_allclient(l);
        //printf("client number:%d\n", l->length);
    }
    return 1;
}

//接收文件
void receive_files(int infor_socket, char *ip) {
    char str[Length];
    char new_filename[7][256] = {0};
    FILE *fp;
    sprintf(new_filename[0], "%s%s/Users", filename, ip);
    sprintf(new_filename[1], "%s%s/Proclog", filename, ip);
    sprintf(new_filename[2], "%s%s/SysInfo", filename, ip);
    sprintf(new_filename[3], "%s%s/MemLog", filename, ip);
    sprintf(new_filename[4], "%s%s/DiskLog", filename, ip);
    sprintf(new_filename[5], "%s%s/CpuLog", filename, ip);
    for (int i = 0; i <= 5; i++) {
        fp = fopen(new_filename[i], "a+");
        while (1) {
            memset(str, 0, sizeof(str));
            int s = read(infor_socket, str, 1);
            if (s > 0) {
                if (str[0] == '#') {
                   // printf("write %s end!", new_filename[i]);

                    break;
                }
                fwrite(str, s, 1, fp);
            } else if (s == 0) {
                break;
            } else {
                char error[128] = {0};
                sprintf(error,"write %s fail!", new_filename[i]);
                my_perror(1, journal, "%s", error);
                break;
            }
        }
        fclose(fp);
    }
    return ;
}

//向client端请求数据
void informat_client(char *file, LinkList *l) {
    char ip[20] = {0}, my_port[20] = {0};
    int infor_socket, port;
    strcpy(my_port, "information_port");
    get_who_conf(file, my_port);
    //printf("-----开始接收文件-----\n");
    ListNode *p = l->head.next;
    
    pthread_mutex_lock(&mut);
    while (p) {
        int port;
        char dir[128] = {0};
        port = atoi(my_port);
        printf("%d\n", port);
        my_inet_ntoa(p->data, ip);
        infor_socket = socket_connect(ip, port);
        if (infor_socket) {              //接收文件
            char error[128] = {0};
            sprintf(error, "recev the file from %s!", ip);
            my_perror(3, journal, "%s", error);
            sprintf(dir, "%s%s", filename, ip);
            mkdir(dir, 0777);
            receive_files(infor_socket, ip);
        } 
        p = p->next;
        close(infor_socket);
    }
    pthread_mutex_unlock(&mut);
}


void *pthread_lister_client(void *str) {
    struct my_st *st;
    st = (struct my_st *)str;
    char *file;
    LinkList *l;
    file = st->f;
    l = st->List;
    lister_client(file, l); 
}

void *pthread_into_list_client(void *str) {
    struct my_st *st;
    st = (struct my_st *)str;
    LinkList *l;
    l = st->List;
    into_list_client(l);
}

void *pthread_lister_udp(void *str) {
    struct my_st *st;
    st = (struct my_st *)str;
    char *file = st->f;
    lister_udp(file);
}

void init_daemon() {
    pid_t pid = fork();
    if (pid < 0) {
        exit(0);
    } else if (pid > 0) {
        exit(0);
    } else {
        setsid();
        pid = fork();
        if (pid != 0) {
            exit(0);
        } else {
        chdir("/");
        umask(0);
        //close(STDIN_FILENO);
        //close(STDOUT_FILENO);
        //close(STDERR_FILENO);
        }
    }
}


int main (int argc, char *argv[]) {
    char master_dir[125] = {0};
    strcpy(master_dir, "master_dir");
    get_who_conf(argv[1], master_dir);

    sprintf(Configurefile, "%s/%s", master_dir, argv[1]);
    pthread_mutex_init(&mut, NULL);
    Clilist = init_linklist();             //初始化一个链表
    struct my_st str;
    memset(str.f, 0, sizeof(str.f));
    sprintf(str.f,"%s", Configurefile);
    str.List = Clilist;

    strcpy(filename, "filename");
    get_who_conf(argv[1], filename);
    strcpy(journal, "journallujin");
    get_who_conf(argv[1], journal);
    mkdir(journal, 0777);
    memset(journal,0,sizeof(journal));
    strcpy(journal, "journalfile");
    get_who_conf(argv[1], journal);
   
    init_daemon();

    pthread_t thr1, thr2, thr3;
    pthread_create(&thr1, NULL, pthread_lister_client, &str);
    pthread_create(&thr2, NULL, pthread_into_list_client, &str);
    pthread_create(&thr3, NULL, pthread_lister_udp, &str);
    while (1) {
        //char play = getchar();
        //if (play == 'r') {
            sleep(30);
            informat_client(Configurefile, Clilist);
        //}
    }
    pthread_join(thr1, NULL);    
    pthread_join(thr2, NULL);    
    pthread_join(thr3, NULL);    
    return 0;
}

