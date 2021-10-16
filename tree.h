#ifndef L_SHARED_PTR_TREE_H
#define L_SHARED_PTR_TREE_H

#include "LShared.h"

struct Node: public Object {
    Node* parent;
    Node(Node* parent);
    static int countId;
    int id;
    ~Node();
    int depth = 0;
    Node* left;
    Node*right;
    void addChild(Node* parent);
};

void makeTree(Node* node, int depth);
void testTree();

#endif //L_SHARED_PTR_TREE_H
