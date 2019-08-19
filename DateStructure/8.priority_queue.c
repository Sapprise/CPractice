/*************************************************************************
	> File Name: 8.priority_queue.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年08月09日 星期五 18时38分44秒
 ************************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#define swap(a, b) {\
    __typeof(a) __temp = a;\
    a = b, b = __temp;\
}

typedef struct priority_queque {
    int *data;
    int cnt, size;
} priority_queque;

priority_queque *init(int n) {

    priority_queque *q = (priority_queque *)malloc(sizeof(priority_queque));
    q->data = (int *)malloc(sizeof(int) * (n + 1));
    q->cnt = 0, q->size = n;
    return q;
}

int empty(priority_queque *q) {
    return q->cnt == 0;
}

int top(priority_queque *q) {
    return q->data[1];
}

int push(priority_queque *q, int val) {
   if (q->cnt == q->size) return 0;
    q->cnt += 1;
    q->data[q->cnt] = val;
    int ind = q->cnt;
    while (ind >> 1 && q->data[ind] > q->data[ind >> 1]) {
        swap(q->data[ind], q->data[ind >> 1]);
        ind >>= 1;
    }
    return 1;
}

int pop(priority_queque *q) {
    if (empty(q)) return 0;
    q->data[1] = q->data[q->cnt--];
    int ind = 1;
    while ((ind << 1) <= q->cnt) {
        int temp = ind;
        if (q->data[ind << 1] > q->data[temp]) temp = ind << 1;
        if ((ind << 1 | 1) <= q->cnt && q->data[ind << 1 | 1] > q->data[temp]) {
            temp = ind << 1 | 1;
        }
        if (temp == ind) break;
        swap(q->data[ind], q->data[temp]);
        ind = temp;
    }
    return 1;
}



void clear(priority_queque * q) {
    if (q == NULL) return ;
    free(q->data);
    free(q);
    return ;
}

int main() {
    srand(time(0));
    #define MAX_OP 20
    priority_queque *q = init(MAX_OP);
    for (int i = 0; i < MAX_OP; i++) {
        int val = rand() % 100;
        push(q, val);
        printf("insert %d to queue\n", val);
    }
    for (int i = 0; i < MAX_OP; i++) {
        printf("%d ", top(q));
        pop(q);
    }
    return 0;
}
