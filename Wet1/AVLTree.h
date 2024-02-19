#include "AVLNode.h"
#include <iostream>

#ifndef AVLTREE_H_
#define AVLTREE_H_

template <class dataType1, class dataType2>
class AVLTree
{
private:
    AVLNode<dataType1, dataType2> *root;
    int treeSize;
    AVLNode<dataType1, dataType2> *initNewNode(dataType1 key, dataType2 data);
    void rollLeft(AVLNode<dataType1, dataType2> *node);
    void rollRight(AVLNode<dataType1, dataType2> *node);
    bool rollNode(AVLNode<dataType1, dataType2> *node);
    void destroy(AVLNode<dataType1, dataType2> *node);
    void insertLeftNaive(AVLNode<dataType1, dataType2> *target, dataType1 key, dataType2 data);
    void insertRightNaive(AVLNode<dataType1, dataType2> *target, dataType1 key, dataType2 data);
    bool removeLeaf(AVLNode<dataType1, dataType2> *target);
    bool removeSingleChild(AVLNode<dataType1, dataType2> *target);
    void updateNodeParameters(AVLNode<dataType1, dataType2> *target);
    void printInOrderAux(AVLNode<dataType1, dataType2> *node);
    void swapTwoNodes(AVLNode<dataType1, dataType2> *v1, AVLNode<dataType1, dataType2> *v2);
    AVLNode<dataType1, dataType2> *findNextNode(AVLNode<dataType1, dataType2> *node);

public:
    AVLTree();
    ~AVLTree();
    AVLNode<dataType1, dataType2> *search(dataType1 key);
    bool nodeExists(dataType1 key);
    bool isTreeEmpty();
    bool insert(dataType1 key, dataType2 data);
    bool remove(dataType1 key);
    dataType2 *getRightMostNode();
    dataType2 *getLeftMostNode();
    void printInOrder();
    int getTreeSize();
    static int inorderToArray(AVLNode<dataType1, dataType2> *root, AVLNode<dataType1, dataType2> *arr, int arrSize, int i);
    static int inorderToTree(AVLNode<dataType1, dataType2> *root, AVLNode<dataType1, dataType2> *arr, int arrSize, int i);
    static void combineAVLTrees(AVLTree<dataType1, dataType2> &newTree, AVLTree<dataType1, dataType2> &tree1, AVLTree<dataType1, dataType2> &tree2);
    static AVLNode<dataType1, dataType2> *createEmptyTree(int height);
    static void adjustTreeSize(AVLTree<dataType1, dataType2> &tree, AVLNode<dataType1, dataType2> *root, int *toDelete);
    AVLNode<dataType1, dataType2> *getRoot() const
    {
        return root;
    }
    void setRoot(AVLNode<dataType1, dataType2> *node)
    {
        root = node;
    }
};

#endif