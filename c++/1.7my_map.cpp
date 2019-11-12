/*************************************************************************
	> File Name: 1.7my_map.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年11月12日 星期二 18时18分51秒
 ************************************************************************/

#include <iostream>
#include <utility>
using namespace std;

template<typename Key, typename T>
class My_Node {
public:
    My_Node<Key, T>() : lchild(nullptr), rchild(nullptr) {}
    My_Node<Key, T>(const Key &k, const T &t) : lchild(nullptr), rchild(nullptr), key(k, t) {}
    My_Node<Key, T> *lchild;
    My_Node<Key, T> *rchild;
    pair<Key, T> key;
};

template<typename K, typename T>
class my_map {
    typedef My_Node<K, T> Node;
public:
    my_map<K, T>() : root(nullptr) {}
    Node *getNewNode(pair<K, T> key) {
        Node *p = (Node *)malloc(sizeof(Node));
        p->key.first = key.first;
        p->key.second = key.second;
        p->lchild = p->rchild = nullptr;
        return p;
    }
    
    Node *insert(pair<K, T> key) {
        return _insert(this->root, key);
    }

    Node *_insert(Node *root, pair<K, T> key) {
        if (this->root == nullptr) {
            this->root = getNewNode(key);
            return this->root;
        }
        if (root == nullptr) return getNewNode(key);
        if (root->key.first == key.first) return root;
        if (root->key.first > key.first) root->lchild = _insert(root->lchild, key);
        else root->rchild = _insert(root->rchild, key);
        return root;
    }

    Node *predecessor(Node *root) {
        Node *temp = root->lchild;
        while (temp->rchild) temp = temp->rchild;
        return temp;
    }
    
    Node *erase(pair<K, T> key) {
        return _erase(this->root, key);
    }

    Node *_erase(Node *root, pair<K, T> key) {
        if (root == nullptr) return root;
        if (root->key.first > key.first) root->lchild = _erase(root->lchild, key);
        else if (root->key.first < key.first) root->rchild = _erase(root->rchild, key);
        else {
            if (root->lchild == nullptr || root->rchild == nullptr) {
                Node *temp = root->lchild ? root->lchild : root->rchild;
                free(root);
                return temp;
            } else {
                Node *temp = predecessor(root);
                root->key.first = temp->key.first;
                root->key.second = temp->key.second;
                root->lchild = _erase(root->lchild, temp->key);
            }
        }
        return root;
    }

    void _in_order(Node *root) {
        if (root == nullptr) return ;
        _in_order(root->lchild);
        cout << root->key.second << " ";
        _in_order(root->rchild);
        return ;
    }

    void in_order() {
        cout << "in order output :";
        _in_order(this->root);
        cout << endl;
        return ;
    }
private:
    Node *root;
};


int main() {
    int val;
    my_map<int, int> arr;
    pair<int, int> p;
    while(cin >> val >> p.first >> p.second) {
        switch (val) {
            case 1: arr.insert(p); break;
            case 2: arr.erase(p); break;
        }
        arr.in_order();
    }
    return 0;
}
