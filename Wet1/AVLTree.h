#include "AVLNode.h"
#include <iostream>

#ifndef AVLTREE_H_
#define AVLTREE_H_

template <class dataType>
class AVLTree
{
private:
    AVLNode<dataType> *root;
    AVLNode<dataType> *initNewNode(int key, dataType data);
    void rollLeft(AVLNode<dataType> *node);
    void rollRight(AVLNode<dataType> *node);
    bool rollNode(AVLNode<dataType> *node);
    void destroy(AVLNode<dataType> *node);
    void insertLeftNaive(AVLNode<dataType> *target, int key, dataType data);
    void insertRightNaive(AVLNode<dataType> *target, int key, dataType data);
    bool removeLeaf(AVLNode<dataType> *target);
    bool removeSingleChild(AVLNode<dataType> *target);
    void updateNodeParameters(AVLNode<dataType> *target);
    void printInOrderAux(AVLNode<dataType> *node);
    void swapTwoNodes(AVLNode<dataType> *v1, AVLNode<dataType> *v2);
    AVLNode<dataType> *findNextNode(AVLNode<dataType> *node);

public:
    AVLTree();
    ~AVLTree();
    AVLNode<dataType> *search(int key);
    bool insert(int key, dataType data);
    bool remove(int key);
    void printInOrder();
};

#endif