/*************************************************************************
	> File Name: class.cpp
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年07月19日 星期五 14时57分25秒
 ************************************************************************/

#include <iostream>
#include <stack>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
using namespace std;
#define MAX_N 100

int arr1[MAX_N + 5], arr2[MAX_N + 5], s[MAX_N + 5];
int top = 0;


void add(int *arr, int ind, int n, int val) {
    for (int i = ind; i < n; i++) arr[i] += val;
    return ;
}

int main() {
    srand(time(0));
    int n;
    cin >> n;
    s[++top] = 0;
    int p = rand() % n;
    for (int i = 1; i<= p; i++) {
        int pos = rand() % (top + 1);
        if (pos < top) {
            if (arr1[s[pos + 1]] - arr1[s[pos]] == 1) add(arr1, s[pos] + 1, i, rand() % 10 + 5);
            if (arr2[s[pos + 1]] - arr2[s[pos]] == 1) add(arr2, s[pos] + 1, i, rand() % 10 + 5);
        }
        int val1 = rand() % (pos == top ? 20 : arr1[s[pos + 1]] - arr1[s[pos]] - 1) + 1;
        int val2 = rand() % (pos == top ? 20 : arr2[s[pos + 1]] - arr2[s[pos]] - 1) + 1;
        arr1[i] = arr1[s[pos]] + val1;
        arr2[i] = arr2[s[pos]] + val2;
        while (arr1[s[top]] > arr1[i]) --top;
        s[++top] = i;
    }
    for (int i = p + 1; i <= n; i++) {
        arr1[i] = rand() % 10000;
        arr2[i] = rand() % 10000;
    }
    for (int i = 1; i <= n; i++) {
        cout << arr1[i] << " ";
    }
    cout << endl;
    for (int i = 1; i <= n; i++) {
        cout << arr2[i] << " ";
    }
    cout << endl;
    return 0;
}
