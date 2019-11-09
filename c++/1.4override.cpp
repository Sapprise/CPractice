/*************************************************************************
	> File Name: 1.4override.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年11月09日 星期六 15时48分29秒
 ************************************************************************/

#include<iostream>
using namespace std;

struct A final{    //final 禁止继承类
    virtual void func1() {
        cout << "Class A func 1" << endl;
    }
    void funcl() final {    //禁止重写函数
        cout << "Class A func l(L)" << endl;
    }
};

//virtual虚函数关键字，配合override使用可以避免重载非虚函数
struct B : A {
    void func1() override {
        cout << "Class B func1" << endl;
    }  
    void funcl() {
        cout << "Class B func l(L)" << endl;    
    }
};

int main() {
    B b;
    A *a = &b;
    a->funcl();
    return 0;
}
