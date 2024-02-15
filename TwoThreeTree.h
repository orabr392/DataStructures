#include "twoThreeNode.h"

#ifndef TWOTHREETREE_H
#define TWOTHREETREE_H

class TwoThreeTree
{
private:
    twoThreeNode *root;

public:
    TwoThreeTree();
    ~TwoThreeTree();

    void destroy(twoThreeNode *node);
    twoThreeNode *search(int index);
    bool insertNode(int index, int data);
    bool removeNode(int index);
    bool isLeaf(twoThreeNode *node);
    twoThreeNode *initNode(twoThreeNode *parent);
    twoThreeNode *initNode(twoThreeNode *parent, int index);
    twoThreeNode *initNode(twoThreeNode *parent, int index, int data);
    void createNewRoot(twoThreeNode *v1, twoThreeNode *v2);
    void insertIntoParent(twoThreeNode *parent, twoThreeNode *newNode);
    twoThreeNode *splitNodeIntoTwo(twoThreeNode *parent);
};

#endif