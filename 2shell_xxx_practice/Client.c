/*************************************************************************
	> File Name: Client.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月20日 星期六 14时27分36秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "sock.h"
#include "2xxx_practice.h"

//给master返回信息
int send_information(char *file) {
    char val[100] = {0};
    char ip_client[50] = {0}, port_client[20] = {0};
    strcpy(ip_client, "client");
    strcpy(port_client, "information_port");
    get_who_conf(file, ip_client);
    get_who_conf(file, port_client);

    //固定套接字
    int port = atoi(port_client);
    printf("%d %s\n", port, ip_client);
    int listen_socket = socket_fixed(ip_client, port);
    //监听端口
    printf("－－－－监听master端是否接收信息中－－－－！\n");
    while (1) {
        int new_socket = my_accept(listen_socket);

    }
}

//master心跳端口
int lister_master(char *file) {
    char val[100] = {0};
    char ip_client[50] = {0}, port_client[20] = {0};
    strcpy(ip_client, "client");
    strcpy(port_client, "client_port");
    get_who_conf(file, ip_client);
    get_who_conf(file, port_client);

    //固定套接字
    int port = atoi(port_client);
    printf("%d %s\n", port, ip_client);
    int listen_socket = socket_fixed(ip_client, port);
    //监听端口
    printf("－－－－监听master端心跳中－－－－！\n");
    while (1) {
        int new_socket = my_accept(listen_socket);
        printf("master:%d\n", new_socket);
    }
    return 0;
}

int main(int argc, char *argv[]) {

    pid_t pid;
    pid = fork();

    if (pid == 0) {
        char val[100] = {0};
        char ip_master[50] = {0}, port_master[20] = {0};
    
        //获取master端IP和端口号
        strcpy(ip_master,"master");
        strcpy(port_master, "master_port");
        get_who_conf(argv[1], ip_master);
        get_who_conf(argv[1], port_master);
        //连接master端
        int port = atoi(port_master);
        int master_socket = socket_connect(ip_master, port); 
        
        lister_master(argv[1]);

        exit(0);
    } 
    if (pid > 0) {
        char file[50] = {0};
        strncpy(file, argv[1], strlen(argv[1]));
        send_information(file);

    }

    return 0;
}

