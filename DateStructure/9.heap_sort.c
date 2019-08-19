/*************************************************************************
	> File Name: 9.heap_sort.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年08月09日 星期五 19时53分18秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define swap(a, b) {\
    __typeof(a) __temp = a;\
    a = b, b = __temp;\
}

void downUpdata(int *arr, int n, int ind) {
    while ((ind << 1) <= n) {
        int temp = ind, l = ind << 1, r = ind << 1 | 1;
        if (arr[l] > arr[temp]) temp = l;
        if (r <= n && arr[r] > arr[temp]) temp = r;
        if (temp == ind) break;
        swap(arr[temp], arr[ind]);
        ind = temp;
    }
    return ;
}

void head_sort(int *arr, int n) {
    arr -= 1;
    for (int i = n >> 1; i >= 1; --i) {
        downUpdata(arr, n, i);
    }
    for (int i = n; i > 1; --i) {
        swap(arr[1], arr[i]);
        downUpdata(arr, i - 1, 1);
    }
    return ;
}

void output(int *arr, int n) {
    printf("arr[%d] = [", n);
    for (int i = 0; i < n; i++) {
        printf(" %d", arr[i]);
    }
    printf("]\n");
    return ;
}

int main() {
    srand(time(0));
    int *arr = (int *)malloc(sizeof(int) * 20);
    for (int i = 0; i < 20; i++) {
        arr[i] = rand() % 200;
    }
    output(arr, 20);
    head_sort(arr, 20);
    output(arr, 20);
    return 0;
}
