/*************************************************************************
	> File Name: Client.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月20日 星期六 14时27分36秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include "sock.h"
#include "2xxx_practice.h"

#define Length 1024


void clear_shell_information(char *infor_file) {  //清空文件写入当前时间
    FILE *fp, *fd;
    char buff[Length];
    memset(buff, 0, sizeof(buff));
    fp = popen("hostname", "r");              
    fd = fopen(infor_file, "w+");
    fread(buff, 1, Length, fp);
    fwrite(buff, 1, strlen(buff), fd);
    pclose(fp);
    fclose(fd);
    return ;
}

void shell_information(char *infor_file, char *bash) { //执行一个脚本将信息加入文件
    FILE *fp, *fd;
    char buff[Length];
    memset(buff, 0, sizeof(buff));
    fp = popen(bash, "r");              
    fd = fopen(infor_file, "a+");
    fread(buff, 1, Length, fp);
    fwrite(buff, 1, strlen(buff), fd);
    pclose(fp);
    fclose(fd);
    return ;
}

void all_shell(char *infor_file) {                  //执行所有脚本
    char bash[50] = {0};
    clear_shell_information(infor_file);
    strcpy(bash, "bash Users.sh");
    shell_information(infor_file, bash);

    memset(bash, 0, sizeof(bash));
    strcpy(bash, "bash Proclog.sh");
    shell_information(infor_file, bash);

    memset(bash, 0, sizeof(bash));
    strcpy(bash, "bash SysInfo.sh");
    shell_information(infor_file, bash);

    memset(bash, 0, sizeof(bash));
    strcpy(bash, "bash MemLog.sh");
    shell_information(infor_file, bash);

    memset(bash, 0, sizeof(bash));
    strcpy(bash, "bash DiskLog.sh");
    shell_information(infor_file, bash);

    memset(bash, 0, sizeof(bash));
    strcpy(bash, "bash CpuLog.sh");
    shell_information(infor_file, bash);
    return ;
}


//给master返回信息
int send_information(char *file, char *infor_file) {
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
        printf("开始传输数据！\n");
        FILE *fp_lock = fopen("./file_lock.test", "w+");
        flock(fp_lock->_fileno, LOCK_EX);
        FILE *fp;
        char str[Length];
        int sread = 1;
        fp = fopen(infor_file, "rw");
        while (sread != 0) {
            memset(str, 0, sizeof(str));
            sread = fread(str, 1, 1, fp);
            if (sread > 0) {
                write(new_socket, str, strlen(str));
            } 
        }
        printf("read sucesses!\n");
        flock(fp_lock->_fileno, LOCK_UN);
        fclose(fp_lock);
        fclose(fp);
        close(new_socket);
    }
    return 0;
}

//master心跳端口
int lister_master(char *file, pid_t father_pid) {
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
    int nfds, epollfd;
    struct epoll_event ev, events[5];
    epollfd = epoll_create1(0);
    ev.events = EPOLLIN;
    ev.data.fd = listen_socket;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_socket, &ev);
    printf("－－－－接收master端心跳中－－－－！\n");
    while (1) {
        int timeout = 1000 * 10;
        nfds = epoll_wait(epollfd, events, 5, timeout);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        //等待心跳超时
        if (nfds == 0) {
            printf("心跳等待超时！\n");
            kill(father_pid, SIGUSR1);
        }
        for (int n = 0; n < nfds; n++) {
            int new_socket = my_accept(listen_socket);
            printf("masterheart:%d\n", new_socket);
            close(new_socket);
         }
    }
    return 0;
}

char file[20] = {0};
//连接master
void client_connet_master() {
    char val[100] = {0};
    char ip_master[50] = {0}, port_master[20] = {0};
    //获取master端IP和端口号
    strcpy(ip_master,"master");
    strcpy(port_master, "master_port");
    get_who_conf(file, ip_master);
    get_who_conf(file, port_master);
    int port = atoi(port_master);
    int master_socket = socket_connect(ip_master, port); 
    if (!master_socket) {
        printf("socket_connect fail!\n");
    } else {
        printf("master_socket:%d\n", master_socket);
        close(master_socket);
    }
    return ;
}

int main(int argc, char *argv[]) {
    strcpy(file, argv[1]);

    pid_t pid;
    pid = fork();

    if (pid == 0) {
        pid_t chlidren_pid;
        chlidren_pid = fork();
        if (chlidren_pid == 0) {     //孙子进程监听心跳
            pid_t father_pid;
            father_pid = getppid();
            lister_master(argv[1], father_pid);
        }
        if (chlidren_pid > 0) {     //孩子进程连接master端
            client_connet_master();
            signal(SIGUSR1, client_connet_master);
            while (1) {
                sleep(10);                       
            }
        }        

    } 
    if (pid > 0) {  //父进程等待传输数据给master端
        pid_t thepid;
        thepid = fork();

        if (thepid == 0) {
            FILE *fp = fopen("./file_lock.test", "w+");
            flock(fp->_fileno, LOCK_EX);
            all_shell(argv[2]);
            flock(fp->_fileno, LOCK_UN);
            fclose(fp);
            sleep(5);          
        }
        if(thepid > 0) {          
            char file[50] = {0};
            strncpy(file, argv[1], strlen(argv[1]));
            send_information(file, argv[2]);
        }
    }
    

    return 0;
}

