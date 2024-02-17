#ifndef AVLNODE_H_
#define AVLNODE_H_

template <class dataType1, class dataType2>
struct AVLNode
{
    dataType1 key;
    dataType2 data;
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