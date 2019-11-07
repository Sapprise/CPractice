/*************************************************************************
	> File Name: hashtable.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年10月27日 星期日 14时51分29秒
 ************************************************************************/

#include<iostream>
using namespace std;

class Node {
public:
    Node() : flag(false), str(nullptr) {}
    bool get_flag() { return }
    void set_flag(bool flag) {
        this->flag = flag;
        return ;
    }
    char *get_str() {return this->str;}
    void set_str(const char *str) {
        this->str = strdup(str);
        return ;
    }
private:
    bool flag;
    char *str;
};

class HashFunc {
public:
    virtual int operator()(const char *str) const = 0;
};

class HashTable {
public:
    HashTable(const HashFunc &func);
    bool insert(const char *str);
    bool find(const char *str);
private:
    int __size;
    Node *data;
    const HashFunc *p_func;
};

int main() {
    

    return 0;
}

HashTable::HashTable(const HashFunc &func) : p_func(&func) {
    this->__size = 100;
    this->data = new Node[this->__size];
}

bool  HashTable::insert(const char *str) {
    int ind = (*p)
}



