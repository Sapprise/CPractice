/*************************************************************************
	> File Name: 1.vector.cpp
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年06月30日 星期日 15时18分08秒
 ************************************************************************/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Vector {
    int *data;
    int size, length;
} Vector;

#define default_value(n, val) ((#n)[0] ? n + 0 : val)
#define init(n) __init(default_value(n, 10))

Vector *__init(int n) {
    Vector *vec = (Vector *)malloc(sizeof(Vector));
    vec->data  = (int *)malloc(sizeof(int) * n);
    vec->size = n;
    vec->length = 0;
    return vec;
}

int expand(Vector *vec) {
    int new_size = vec->size * 2;
    int *p = (int *)realloc(vec->data, sizeof(int) * new_size);
    if (p == NULL) return 0;
    vec->size = new_size;
    vec->data = p;
    return 1;
}

int insert(Vector *vec, int ind, int val) {
    if (vec == NULL) return 0;
    if (vec->length == vec->size) {
        if (!expand(vec)) return 0;
    };
    if (ind < 0 || ind > vec->length) return 0;
    for (int i = vec->length; i > ind; i--) {
        vec->data[i] = vec->data[i - 1];
    }
    vec->data[ind] = val;
    vec->length += 1;
    return 1;
}

int erase(Vector *vec, int ind) {
    if (vec == NULL) return 0;
    if (vec->length == 0) return 0;
    if (ind < 0 || ind >= vec->length) return 0;
    for (int i = ind + 1; i < vec->length; i++) {
        vec->data[i - 1] = vec->data[i];
    }
    vec->length -= 1;
    return 1;
}

int find(Vector *vec, int val) {
    if (vec == NULL) return -1;
    for (int i = 0; i < vec->length; i++) {
        if (val == vec->data[i]) return i;
    }
    return -1;
}

void output(Vector *vec) {
    printf("array(%d) = [", vec->length);
    for (int i = 0; i < vec->length; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("%d", vec->data[i]);
    }
    printf("]\n");
    return ;
}

void find__output(Vector *vec, int val_i) {
    int num, all = 0;
    num = printf("array(%d) = [", vec->length);
    all += num - 1;
    for (int i = 0; i < vec->length; i++) {
        if (i != 0) {
            num = printf(", ");
            if( i <= val_i) all += num;
        }
        num = printf("%d", vec->data[i]);
        if( i <= val_i) all += num;
    }
    printf("]\n");
    for(int k = 0; k < all; k++) {
        printf(" ");
    }
    printf("^\n");
    return ;
}

void clear(Vector *vec) {
    if (vec == NULL) return;
    free(vec->data);
    free(vec);
    return;
}

int main() {
    srand(time(0));
    #define MAX_OP 20
    Vector *vec = init(1);
    int op, ind, val;
    for (int i = 0; i < MAX_OP; i++) {
        op = rand() % 3;
        ind = rand() % (vec->length + 1);
        val = rand() % 100;
        switch (op) {
            case 2:
            case 0: {
                printf("insert %d at %d to vector\n", val, ind);
                insert(vec, ind, val);
            }break;
            case 1: {
                printf("erase item at %d from Vector\n", ind);
                erase(vec, ind);
            }break;
        }
        output(vec);
        printf("\n");
    }
    int find_val; 
    while(~scanf("%d", &find_val)) {
       int find_i = find(vec, find_val);
       find__output(vec, find_i); 
    }
    return 0;
}

