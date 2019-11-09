/*************************************************************************
	> File Name: 1.3constrexpr.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年11月09日 星期六 14时53分05秒
 ************************************************************************/

#include<iostream>
using namespace std;

constexpr int add(int i) {    //表示add可以是个常量表达式
    return i * 2;
}

struct ClassA {
    constexpr ClassA(int x, int y) : x(x), y(y) {}
    int x, y;
};

int main() {
    int n = 6;
    cin >> n;
    const int i = 1 + n;    //const可以作用于运行期,或者编译期
    constexpr int b = 1 + 7;    //constexpr必须在编译期确定
    cout << i << endl;
    constexpr ClassA a(1, 2);
    cout << a.x << " " << a.y << endl;
    return 0;
}
