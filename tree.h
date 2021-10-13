//
// Created by andrzej on 10/13/21.
//

#ifndef L_SHARED_PTR_TREE_H
#define L_SHARED_PTR_TREE_H

struct Node {
    Node* parent;
    Node(Node* parent);
    int depth = 0;
    Node* left;
    Node*right;
    void addChild(Node* parent);
};

void makeTree(Node* node, int depth);

#endif //L_SHARED_PTR_TREE_H
