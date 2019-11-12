/*************************************************************************
	> File Name: 1.6lambda.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年11月09日 星期六 20时15分27秒
 ************************************************************************/

#include<iostream>
using namespace std;

auto add = [](int a, int b) -> long long {
    return a + b;
}; 

auto Data = [](int a, int b) {
    return [=](auto func) {
        return func(a, b);
    };
};

auto First = [](int a, int b) {
    return a;
};

auto Second = [](int a, int b) {
    return b;  
};

int main() {
    cout << add(2, 4) << endl;
    auto a = Data(2, 5);
    cout << a(First) << endl;
    cout << a(Second) << endl;

    return 0;
}
