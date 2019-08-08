/*************************************************************************
	> File Name: Linklist.h
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月20日 星期六 17时48分42秒
 ************************************************************************/

#include "my_Linklist.h"


//初始化链表节点
ListNode *init_listnode(unsigned int val) {
    ListNode *p = (ListNode *)malloc(sizeof(ListNode));
    p->data = val;
    p->next = NULL;
    return p;
}
//初始化一个链表
LinkList *init_linklist() {
    LinkList *l = (LinkList *)malloc(sizeof(LinkList));
    l->head.next = NULL;
    l->length = 0;
    return l;
}
//删除一个节点
void clear_listnode(ListNode *node) {
    if (node == NULL) return ;
    free(node);
    return ;
}
//删除链表
void clear_linklist(LinkList *l) {
    if (l == NULL) return ;
    ListNode *p = l->head.next, *q;
    while (p) {
        q = p->next;
        clear_listnode(p);
        p = q;
    }
    free(l);
    return ;
}
//在指定位置插入一个节点
int insert(LinkList *l, int ind, unsigned int val) {
    if (l == NULL) return 0;
    if (ind < 0 || ind > l->length) return 0;
    ListNode *p = &(l->head), *node = init_listnode(val);
    while (ind--) {
        p = p->next;
    }
    node->next = p->next;
    p->next = node;
    l->length += 1;
    return 1;
}
//删除指定位置的一个节点
int erase(LinkList *l, int ind) {
    if (l == NULL) return 0;
    if (ind < 0 || ind >= l->length) return 0;
    ListNode *p = &(l->head), *q;
    while (ind--) {
        p = p->next;
    }
    q = p->next->next;
    clear_listnode(p->next);
    p->next = q;
    l->length -= 1;
    return 1;
}

