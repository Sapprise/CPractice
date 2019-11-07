/*************************************************************************
	> File Name: text3.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年11月07日 星期四 18时46分14秒
 ************************************************************************/

#include<iostream>
using namespace std;

template<typename T, typename ...REST>
struct ARG {
    typedef T __type;
    typedef ARG<REST...> __rest;
};

template<typename T>
struct ARG<T> {
    typedef T __type;  
    typedef T __final;  //限制REST...只能为2个
};

template<typename T, typename ...ARGS> struct Test;    //普通版
template<typename T, typename ...ARGS>    //偏特化版本
struct Test<T(ARGS...)> {
    T operator()(
        typename ARG<ARGS...>::__type a, 
        typename ARG<ARGS...>::__rest::__final b) {
        return a + b;
    }  
};

int main() {
    Test<int(int, int)> func_a;
    return 0;
}
