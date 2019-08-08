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
#include "my_sock.h"
#include "my_ip_infor.h"

#define Length 1024
#define SHORT 512
#define THEFILE "Configure"
char filename[7][128] = {0};
char buff[7][6][SHORT] = {0};
int memlog = 0;


void udp_recv(char *file, char *str) {
    char ip_client[20] = {0}, udp_port[20] = {0}, buff[SHORT] = {0};
    int port, len, sockfd;
    strcpy(ip_client, "master");
    strcpy(udp_port, "udp_port");
    get_who_conf(file, ip_client);
    get_who_conf(file, udp_port);
    port = atoi(udp_port);
    struct sockaddr_in addr;
    len = sizeof(struct sockaddr_in);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip_client);

    strcpy(buff, str);
    sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&addr, len);
    close(sockfd);
    return ;
}


void clear_shell_information(char *infor_file) {  //写入主机名
    FILE *fp, *fd;
    char buff[Length];
    memset(buff, 0, sizeof(buff));
    fp = popen("hostname", "r");              
    fd = fopen(infor_file, "a+");
    fread(buff, 1, Length, fp);
    fwrite(buff, 1, strlen(buff), fd);
    pclose(fp);
    fclose(fd);
    return ;
}

//执行一个脚本写入信息
void shell_information(char *infor_file, char *bash, int num, int who) { 
    FILE *fp, *fd;
    fp = popen(bash, "r");              

    if (who == 3) {
        char mem[10] = {0};  
        fgets(buff[who][num], SHORT, fp);
        fgets(mem, 10, fp);
        memlog = atoi(mem);
    } else {
        fread(buff[who][num], 1, SHORT, fp);
    }

    char keys[10] = {0};
    strcpy(keys, "warning");
    if (strstr(buff[who][num], keys) != NULL) {
        char file[20] = {0}, string[SHORT] = {0};
        strcpy(file, THEFILE);
        strncpy(string, buff[who][num], strlen(buff[who][num]));
        udp_recv(file, string);
    }

    if (who == 1 && strlen(buff[1][num]) > 20) {
        char file[20] = {0}, string[SHORT] = {0};
        strcpy(file, THEFILE);
        sprintf(string, "恶意进程:%s", buff[1][num]);
        udp_recv(file, string);
    }
                                                                        
    if (num == 4) {
        fd = fopen(infor_file, "a+");
        fwrite(buff[who][0], 1, strlen(buff[who][0]), fd);
        memset(buff[who][0], 0, sizeof(buff[who][0]));
        fwrite(buff[who][1], 1, strlen(buff[who][1]), fd);
        memset(buff[who][1], 0, sizeof(buff[who][1]));
        fwrite(buff[who][2], 1, strlen(buff[who][2]), fd);
        memset(buff[who][2], 0, sizeof(buff[who][2]));
        fwrite(buff[who][3], 1, strlen(buff[who][3]), fd);
        memset(buff[who][3], 0, sizeof(buff[who][3]));
        fwrite(buff[who][4], 1, strlen(buff[who][4]), fd);
        memset(buff[who][4], 0, sizeof(buff[who][4]));
        fclose(fd);
    }
    pclose(fp);
    return ;
}

void all_shell(int num) {                  //执行所有脚本
    char bash[50] = {0};

    //clear_shell_information("./infor_client/Users");
    strcpy(bash, "bash Users.sh");
    shell_information("./infor_client/Users", bash, num, 0);

    memset(bash, 0, sizeof(bash));
    //clear_shell_information("./infor_client/Proclog");
    strcpy(bash, "bash Proclog.sh");
    shell_information("./infor_client/Proclog", bash, num, 1);

    memset(bash, 0, sizeof(bash));
    //clear_shell_information("./infor_client/SysInfo");
    strcpy(bash, "bash SysInfo.sh");
    shell_information("./infor_client/SysInfo", bash, num, 2);

    memset(bash, 0, sizeof(bash));
    //clear_shell_information("./infor_client/MemLog");
    sprintf(bash, "bash MemLog.sh %d", memlog);
    shell_information("./infor_client/MemLog", bash, num, 3);

    memset(bash, 0, sizeof(bash));
    //clear_shell_information("./infor_client/DiskLog");
    strcpy(bash, "bash DiskLog.sh");
    shell_information("./infor_client/DiskLog", bash, num, 4);

    memset(bash, 0, sizeof(bash));
    //clear_shell_information("./infor_client/CpuLog");
    strcpy(bash, "bash CpuLog.sh");
    shell_information("./infor_client/CpuLog", bash, num, 5);

    return ;
}


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
        printf("开始传输数据！\n");
        FILE *fp_lock = fopen("./file_lock.test", "w+");
        flock(fp_lock->_fileno, LOCK_EX);

        FILE *fp;
        char str[Length];
        for (int i = 0; i <= 5; i++) {
            int sread = 1;
            printf("%s\n", filename[i]);
            fp = fopen(filename[i], "rw");
            while (1) {
                memset(str, 0, sizeof(str));
                sread = fread(str, 1, 1, fp);
                if (sread > 0) {
                    write(new_socket, str, strlen(str));
                } 
                if (sread == 0) {
                    write(new_socket, "#", 1);
                    break;
                }
            }  
            fclose(fp);
            fp = fopen(filename[i], "w");
            fclose(fp);
        }
        printf("read sucesses!\n");
        flock(fp_lock->_fileno, LOCK_UN);
        fclose(fp_lock);
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
    int timeout = 1000 * 10;
    while (1) {
        nfds = epoll_wait(epollfd, events, 5, timeout);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        //等待心跳超时
        if (nfds == 0) {
            printf("心跳等待超时！\n");
            timeout += 1000*10;
            kill(father_pid, SIGUSR1);
        }
        for (int n = 0; n < nfds; n++) {
            int new_socket = my_accept(listen_socket);
            printf("masterheart:%d\n", new_socket);
            timeout = 1000*10;
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
    if (argc != 2) {
        printf("参数错误!\n");
        return 0;
    }

    strcpy(file, argv[1]);
    strcpy(filename[0], "./infor_client/Users");
    strcpy(filename[1], "./infor_client/Proclog");
    strcpy(filename[2], "./infor_client/SysInfo");
    strcpy(filename[3], "./infor_client/MemLog");
    strcpy(filename[4], "./infor_client/DiskLog");
    strcpy(filename[5], "./infor_client/CpuLog");

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
            int num = 0;
            clear_shell_information("./infor_client/Users");
            clear_shell_information("./infor_client/Proclog");
            clear_shell_information("./infor_client/SysInfo");
            clear_shell_information("./infor_client/MemLog");
            clear_shell_information("./infor_client/DiskLog");
            clear_shell_information("./infor_client/CpuLog");
            while (1) {
                flock(fp->_fileno, LOCK_EX);
                all_shell(num);
                flock(fp->_fileno, LOCK_UN);
                num++;
                num %= 5; 
                sleep(30);          //间隔1min执行一次脚本
            }
            fclose(fp);
        }

        if(thepid > 0) {          
            char file[50] = {0};
            strncpy(file, argv[1], strlen(argv[1]));
            send_information(file);
        }
    }
    return 0;
}

