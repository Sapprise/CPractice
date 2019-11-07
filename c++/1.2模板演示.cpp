/*************************************************************************
	> File Name: 1.2模板演示.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年11月07日 星期四 19时51分50秒
 ************************************************************************/

#include<iostream>
using namespace std;

template<int n, typename T, typename ...ARGS>
struct ARG {
    typedef typename ARG<n - 1, ARGS...>::getT getT;
};

template<typename T, typename ...ARGS>
struct ARG<1, T, ARGS...> {
    typedef T getT;
};

template<typename T, typename ...ARGS> struct Test;
template<typename T, typename ...ARGS> 
struct Test<T(ARGS...)> {
    T operator()(
        typename ARG<1, ARGS...>::getT a,
        typename ARG<2, ARGS...>::getT b
    ) {

    }
};

struct ClassA {
    ClassA(int x) : x(x) {}
    int operator+(int x) {
        return this->x + x;
    }
    int x;
};

template<typename T, typename U>
auto add(T &&a, U &&b) -> decltype(a + b) {   //模板里&&表示引用，左右值需要自己判断
    return a + b;
}

int main() {
    int a = 1, b = 2;
    cout << add(a, b) << endl;

    return 0;
}
