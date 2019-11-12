/*************************************************************************
	> File Name: 1.5auto.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年11月09日 星期六 16时31分17秒
 ************************************************************************/

#include <iostream>
#include <cstring>
#include <map>
#include <ctime>
using namespace std;

string randstring(int n) {
    string ret = "";
    for (int i = 0; i < n; i++) {
        char ch = rand() % 26 + 'A';
        ret += ch;
    }
    return ret;
}

int main() {
    map<string, int> ind ;
    for (int i = 0; i < 10; i++) {
        ind[randstring(rand() % 10 + 3)] = rand();
    }
    auto iter = ind.begin();
    for (; iter != ind.end(); iter++) {
        cout << iter->first << " " << iter->second << endl;
    }
    for (auto x : ind) {
        cout << x.first << " " << x.second << endl;
    }
    return 0;
}
