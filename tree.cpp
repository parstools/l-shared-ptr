//
// Created by andrzej on 10/13/21.
//

#include "tree.h"

Node::Node(Node* parent) {
    this->parent = parent;
    if (parent!=nullptr)
        depth = parent->depth+1;
}

void Node::addChild(Node* parent) {
    left = new Node(parent);
    right = new Node(parent);
}

void makeTree(Node* node, int depth) {
    if (depth==0) return;
    node->left = new Node(node);
    makeTree(node->left, depth-1);
    node->right = new Node(node);
    makeTree(node->right, depth-1);
}


void testTree() {
    Node *root = new Node(nullptr);
    makeTree(root, 5);
}
