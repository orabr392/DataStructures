#include <cmath>
#include <iostream>

#include "StrNode.h"
#include "TwoKeysIntStr.h"

#ifndef STRTREE_H_
#define STRTREE_H_

class Team;

class StrTree {
   public:
    StrNode<TwoKeysIntStr>* root;
    int treeSize;

    StrTree();
    StrTree(StrNode<TwoKeysIntStr>* newRoot, int size);
    StrTree(int height);
    StrTree(StrTree& otherStrTree);

    ~StrTree();
    void destroy(StrNode<TwoKeysIntStr>* node);

    void disconnectRoot();
    StrNode<TwoKeysIntStr>* search(TwoKeysIntStr key);
    StrNode<TwoKeysIntStr>* search(TwoKeysIntStr key, int* sumExtras);
    int findIndex(TwoKeysIntStr key);
    StrNode<TwoKeysIntStr>* getNodeByRank(int rank);
    bool nodeExists(TwoKeysIntStr key);
    bool isTreeEmpty();
    bool insert(TwoKeysIntStr key, int data, int medals);
    bool remove(TwoKeysIntStr key);
    bool removeLeaf(StrNode<TwoKeysIntStr>* target);
    bool removeSingleChild(StrNode<TwoKeysIntStr>* target);
    StrNode<TwoKeysIntStr>* findNextNode(StrNode<TwoKeysIntStr>* node);
    void swapTwoNodes(StrNode<TwoKeysIntStr>* v1, StrNode<TwoKeysIntStr>* v2);
    void insertLeftNaive(StrNode<TwoKeysIntStr>* target, TwoKeysIntStr key,
                         int data, int sumExtras, int medals);
    void insertRightNaive(StrNode<TwoKeysIntStr>* target, TwoKeysIntStr key,
                          int data, int sumExtras, int medals);
    void updateNodeParameters(StrNode<TwoKeysIntStr>* target);
    void updateNodeParameters(StrNode<TwoKeysIntStr>* target, int sumExtras);
    void rollRight(StrNode<TwoKeysIntStr>* node);
    void rollLeft(StrNode<TwoKeysIntStr>* node);
    bool rollNode(StrNode<TwoKeysIntStr>* node);
    void printInOrder();
    StrNode<TwoKeysIntStr>* getRightMostNode();
    StrNode<TwoKeysIntStr>* getLeftMostNode();
    void printInOrderAux(StrNode<TwoKeysIntStr>* node);
    int getTreeSize();
    StrNode<TwoKeysIntStr>* getRoot();
    void adjustTreeSize(StrNode<TwoKeysIntStr>* node, int* toDelete);
    void setRoot(StrNode<TwoKeysIntStr>* node);
    StrNode<TwoKeysIntStr>* createEmptyTreeAux(
        StrNode<TwoKeysIntStr>* parentNode, int height, bool isLeftChild,
        bool isRightChild);

    void Add(int startIndex, int endIndex, int value);
    void Add(int endIndex, int value);
    int getWins(int teamId);
    int getMaxRank();
};

#endif