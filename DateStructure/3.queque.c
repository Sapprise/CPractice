/*************************************************************************
	> File Name: 3.queque.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月17日 星期三 14时11分12秒
 ************************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct Queue {
    int *data;
    int head, tail;
    int length, cnt;
} Queue;

Queue *init(int n) {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->data = (int *)malloc(sizeof(int) * n);
    q->head = q->tail = q->cnt = 0;
    q->length = n;
    return q;
}

int empty(Queue *q) {
    return q->cnt == 0;
}

int front(Queue *q) {
    return q->data[q->head];
}

int push(Queue *q, int val) {
    if (q == NULL) return 0;
    if (q->cnt == q->length) return 0;
    q->data[q->tail++] = val;
    if (q->tail == q->length) {
        q->tail -= q->length;
    }
    q->cnt += 1;
    return 1;
}

int pop(Queue *q) {
    if (q == NULL) return 0;
    if (empty(q)) return 0;
    q->head += 1;
    if (q->head == q->length) {
        q->head -= q->length;
    }
    q->cnt -= 1;
    return 1;
}

void clear(Queue *q) {
    if (q == NULL) return;
    free(q->data);
    free(q);
    return;
}

void output(Queue *q) {
    printf("queue = [");
    for (int i = 0; i < q->cnt; i++) {
        int ind = (q->head + i) % q->length;
        printf(" %d", q->data[ind]);
    }
    printf("]\n");
    return ;
}

int main() {
    srand(time(0));
    #define MAX_OP 20
    Queue *q = init(5);
    for (int i = 0; i < MAX_OP; i++) {
        int val = rand() % 100, op = rand() % 4;
        switch (op) {
            case 2:
            case 3:
            case 0: {
                printf("push %d to queue = %d\n", val, push(q, val));
            } break;
            case 1: {
                printf("pop %d to Queue = %d\n", front(q), pop(q));
            } break;
        }
        output(q);
    }
    clear(q);
    return 0;
}
