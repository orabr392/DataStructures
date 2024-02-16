#ifndef AVLNODE_H_
#define AVLNODE_H_

template <class dataType>
struct AVLNode
{
    int key;
    dataType data;
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