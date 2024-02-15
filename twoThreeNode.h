#include <iostream>

#ifndef TWOTHREENODE_H
#define TWOTHREENODE_H

struct twoThreeNode
{
    int k1, k2, ktemp;
    int data;
    int childNumber;
    twoThreeNode *leftNode;
    twoThreeNode *middleNode;
    twoThreeNode *rightNode;
    twoThreeNode *tempNode;

    twoThreeNode *parentNode;
};

#endif