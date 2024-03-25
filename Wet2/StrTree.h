#include <cmath>
#include <iostream>

#include "StrNode.h"
#include "TwoKeysInt.h"

#ifndef STRTREE_H_
#define STRTREE_H_

class Team;

class StrTree {
   public:
    StrNode<TwoKeysInt>* root;
    int treeSize;

    StrTree();
    StrTree(StrNode<TwoKeysInt>* newRoot, int size);
    StrTree(int height);
    StrTree(StrTree& otherStrTree);

    ~StrTree();
    void destroy(StrNode<TwoKeysInt>* node);

    void disconnectRoot();
    StrNode<TwoKeysInt>* search(TwoKeysInt key);
    StrNode<TwoKeysInt>* search(TwoKeysInt key, int* sumExtras);
    int findIndex(TwoKeysInt key);
    StrNode<TwoKeysInt>* getNodeByRank(int rank);
    bool nodeExists(TwoKeysInt key);
    bool isTreeEmpty();
    bool insert(TwoKeysInt key, int data, int medals);
    bool remove(TwoKeysInt key);
    bool removeLeaf(StrNode<TwoKeysInt>* target);
    bool removeSingleChild(StrNode<TwoKeysInt>* target);
    StrNode<TwoKeysInt>* findNextNode(StrNode<TwoKeysInt>* node);
    void swapTwoNodes(StrNode<TwoKeysInt>* v1, StrNode<TwoKeysInt>* v2);
    void insertLeftNaive(StrNode<TwoKeysInt>* target, TwoKeysInt key,
                         int data, int sumExtras, int medals);
    void insertRightNaive(StrNode<TwoKeysInt>* target, TwoKeysInt key,
                          int data, int sumExtras, int medals);
    void updateNodeParameters(StrNode<TwoKeysInt>* target);
    void updateNodeParameters(StrNode<TwoKeysInt>* target, int sumExtras);
    void rollRight(StrNode<TwoKeysInt>* node);
    void rollLeft(StrNode<TwoKeysInt>* node);
    bool rollNode(StrNode<TwoKeysInt>* node);
    void printInOrder();
    StrNode<TwoKeysInt>* getRightMostNode();
    StrNode<TwoKeysInt>* getLeftMostNode();
    void printInOrderAux(StrNode<TwoKeysInt>* node);
    int getTreeSize();
    StrNode<TwoKeysInt>* getRoot();
    void adjustTreeSize(StrNode<TwoKeysInt>* node, int* toDelete);
    void setRoot(StrNode<TwoKeysInt>* node);
    StrNode<TwoKeysInt>* createEmptyTreeAux(StrNode<TwoKeysInt>* parentNode,
                                            int height, bool isLeftChild,
                                            bool isRightChild);

    void Add(int startIndex, int endIndex, int value);
    void Add(int endIndex, int value);
    int getWins(int teamId);
    int getMaxRank();
};

#endif