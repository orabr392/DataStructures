#ifndef AVLNODE_H_
#define AVLNODE_H_

struct AVLNode
{
    int key;
    int data;
    int height;
    int heightLeft;
    int heightRight;
    int balanceFactor;

    bool isLeftChild;
    bool isRightChild;

    AVLNode *leftNode;
    AVLNode *rightNode;

    AVLNode *parentNode;
};

#endif