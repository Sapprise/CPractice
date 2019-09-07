/*************************************************************************
	> File Name: 12.double_array_trie.c
	> Author: liujunming 
	> Mail: 1292917361@qq.com 
	> Created Time: 2019年08月21日 星期三 14时15分41秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BASE 26
#define BEGIN_LETTER 'a'

typedef struct Node {
    int flag;
    struct Node *next[BASE];
} Node;

typedef struct DATrie {
    int *base, *check;
    int root, size;
} DATrie;

DATrie *getDATrie(int n) {
    DATrie *tree = (DATrie *)calloc(sizeof(DATrie), 1);
    tree->root = 1;
    tree->base = (int *)calloc(sizeof(int), n);
    tree->size = n;
    tree->check = (int *)calloc(sizeof(int), n);
    tree->check[0] = -1;
    return tree;
}

Node *getNewNode() {
    Node *p = (Node *)calloc(sizeof(Node), 1);
    return p;
}

int insert(Node *root, const char *str) {
    Node *p = root;
    int cnt = 0;
    for (int i = 0; str[i]; i++) {
        int ind = str[i] - BEGIN_LETTER;
        if (p->next[ind] == NULL) p->next[ind] = getNewNode(), cnt += 1;
        p = p->next[ind];
    }
    p->flag = 1;
    return cnt;
}

void clear(Node *root) {
    if (root == NULL) return ;
    for (int i = 0; i < BASE; i++) {
        clear(root->next[i]);
    }
    free(root);
    return ;
}

int getBase(Node *root, DATrie *tree) {
    int base = 1, flag = 1;
    do {
        flag = 1;
        base += 1;
        for (int i = 0; i < BASE; i++) {
            if(root->next[i] == NULL) continue;
            if (tree->check[base + i] == 0) continue;
            flag = 0;
        }
    } while(!flag);
    return base;
}

void buildDATrie(int ind, Node *root, DATrie *tree) {
    int base = tree->base[ind] = getBase(root, tree);
    for (int i = 0; i < BASE; i++) {
        if (root->next[i] == NULL) continue;
        tree->check[base + i] = ind;
    }
    for (int i = 0;i < BASE; i++) {
        if (root->next[i] == NULL) continue;
        buildDATrie(base + i, root->next[i], tree);
    }
    if (root->flag) tree->check[ind] = -tree->check[ind];
    return ;
}

int query(DATrie *tree, const char *str) {
    int p = tree->root;
    for (int i = 0; str[i]; i++) {
        int ind = str[i] - BEGIN_LETTER;
        if (abs(tree->check[tree->base[p] + ind]) != p) return 0;
        p = tree->base[p] + ind;
    }
    return tree->check[p] < 0;
}

void clearDA(DATrie *tree) {
    if (tree == NULL) return ;
    free(tree->base);
    free(tree->check);
    free(tree);
    return ;
}

int main() {
    int n, cnt = 1;
    char str[100];
    scanf("%d", &n);
    Node *root = getNewNode();
    while (n--) {
        scanf("%s", str);
        insert(root, str);
    }
    DATrie *tree = getDATrie(cnt * BASE);
    buildDATrie(tree->root, root, tree);
    while (~scanf("%s", str)) {
        printf("search %s, result = %s\n", str, query(tree, str)? "YES" : "NO");
    }
    clearDA(tree);
    clear(root);
    return 0;
}


