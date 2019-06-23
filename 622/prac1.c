/*************************************************************************
	> File Name: prac1.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年06月22日 星期六 14时23分29秒
 ************************************************************************/

#include <stdio.h>
#include <curses.h>
#include <unistd.h>
int main() {
    initscr();
    clear();
    move(10, 20);
    addstr("Hello \n");
    refresh();
    sleep(1);
    move(10, 20);
    addstr("          ");
    refresh();
    
    move(11, 21);
    addstr("Hello \n");
    refresh();


    getch();
    endwin();

    return 0;
}
