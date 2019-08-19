/*************************************************************************
	> File Name: quence.cpp
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月19日 星期五 17时39分35秒
 ************************************************************************/

#include <iostream>
#include <stdio.h>
using namespace std;

#define MAX_N 500000
int arr1[MAX_N + 5], arr2[MAX_N + 5];
int s1[MAX_N + 5], top1 = -1;
int s2[MAX_N + 5], top2 = -1;

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &arr1[i]);
    }
    for (int i = 1; i <= n; i++) {
        scanf("%d", &arr2[i]);
    }
    s1[++top1] = 0;
    s2[++top2] = 0;

    for (int i = 1; i <= n; i++) {
        while(arr1[i] <= s1[top1]) top1--;
        s1[++top1] = arr1[i];

        while(arr2[i] <= s2[top2]) top2--;
        s2[++top2] = arr2[i];
        if (top1 != top2) {
            printf("%d\n", i - 1);
            return 0;
        }
    }
    printf("%d\n", n);
    return 0;
}
