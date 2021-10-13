//
// Created by andrzej on 10/13/21.
//

#include "tree.h"

int Node::countId = 0;

Node::Node(Node* parent) {
    id = countId;
    countId++;
    this->parent = parent;
    //lshared_assign((Object**)&this->parent, (Object*)parent); weak!
    if (parent!=nullptr)
        depth = parent->depth+1;
    left = right = nullptr;
}

Node::~Node() {
    cout << "delete Node depth=" << depth << " id="<< id <<endl;
    //lshared_release_atomic((Object**)&parent);weak!
    lshared_release_atomic((Object**)&left);
    lshared_release_atomic((Object**)&right);
}

void Node::addChild(Node* parent) {
    left = new Node(parent);
    right = new Node(parent);
}

void makeTree(Node* node, int depth) {
    if (depth==0) return;
    node->left = new Node(node);
    lshared_init_elem(node->left);
    makeTree(node->left, depth-1);
    node->right = new Node(node);
    lshared_init_elem(node->right);
    makeTree(node->right, depth-1);
}


void testTree() {
    Node *root = new Node(nullptr);
    lshared_init_elem(root);
    makeTree(root, 5);
    lshared_release_atomic((Object**)&root);
}
