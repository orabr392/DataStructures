#include "AVLNode.h"
#include <iostream>

#ifndef AVLTREE_H_
#define AVLTREE_H_

class AVLTree
{
private:
    AVLNode *root;
    AVLNode *initNewNode(int key, int data);
    void rollLeft(AVLNode *node);
    void rollRight(AVLNode *node);
    bool rollNode(AVLNode *node);
    void destroy(AVLNode *node);
    void insertLeftNaive(AVLNode *target, int key, int data);
    void insertRightNaive(AVLNode *target, int key, int data);
    bool removeLeaf(AVLNode *target);
    bool removeSingleChild(AVLNode *target);
    void updateNodeParameters(AVLNode *target);
    void printInOrderAux(AVLNode *node);
    bool isLeaf(AVLNode *node);
    void swapTwoNodes(AVLNode *v1, AVLNode *v2);
    AVLNode *findNextNode(AVLNode *node);

public:
    AVLTree();
    ~AVLTree();
    AVLNode *search(int key);
    bool insert(int key, int data);
    bool remove(int key);
    void printInOrder();
    AVLNode *getRoot();
};

#endif