#include <stdio.h>
#include <stdio.h>
   
typedef struct Node {
    int key;
    struct Node *lchild, *rchild;
} Node;

Node *getNewNode(int key) {
    Node *p = (Node *)malloc(sizeof(Node));
    p->key = key;
    p->lchild = p->rchild = NULL;
    return p;
}

Node *insert(Node *root, int key) {
    if (root == NULL) return getNewNode(key);
    if (root->key == key) return root;
    if (root->key > key) root->lchild = insert(root->lchilde, key);
    else root->rchild = insert(root->rchild, key);
    return root;
}

Node *erase(Node *root, int key) {
    if (root == NULL) return root;
    if (root->key > key) root->lchild = erase(root->lchild, key);
    else if (root->key < key) root->rchild = erase(root->rchild, key);
    else {
	if (root->lchild == NULL || root->rchild == NULL) {
	    Node *temp = root->lchild ? root->lchild : root->rchild;
	    free(root);
	    return temp;
	} else {
	    Node *temp = predecessor(root);
	    root->key = temp->key;
	    root->lchild = erase(root->lchild, temp->key);
	}
    }
    return root;
}

void __in_order(Node *root) {
    if (root == NULL）

}








void clear(Node *node) {
    if (node == NULL) return ;
    clear(node->lchild);
    clear(node->rchild);
}
