/*************************************************************************
	> File Name: my_Linklist.h
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年08月08日 星期四 19时41分26秒
 ************************************************************************/

#ifndef _MY_LINKLIST_H
#define _MY_LINKLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct ListNode {
    unsigned int data;
    struct ListNode *next;
} ListNode;

typedef struct LinkList {
    ListNode head;
    int length;
} LinkList;

ListNode *init_listnode(unsigned int); //初始化链表节点
LinkList *init_linklist(); //初始化一个链表
void clear_listnode(ListNode *); //删除一个节点
void clear_linklist(LinkList *); //删除链表
int insert(LinkList *, int, unsigned int); //在指定位置插入一个节点
int erase(LinkList *, int); //删除指定位置的一个节点

#endif
