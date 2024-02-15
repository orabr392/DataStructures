#include "TwoThreeTree.h"

TwoThreeTree::TwoThreeTree()
{
    root = NULL;
}

void TwoThreeTree::destroy(twoThreeNode *node)
{
    if (node != NULL)
    {
        destroy(node->leftNode);
        destroy(node->middleNode);
        destroy(node->rightNode);
        delete node;
    }
}

twoThreeNode *TwoThreeTree::search(int index)
{
    if (root == NULL)
        return root;
    twoThreeNode *temp = root;
    while (!TwoThreeTree::isLeaf(temp))
    {
        if (temp->childNumber == 2)
        {
            if (index < temp->k1)
            {
                temp = temp->leftNode;
            }
            else
            {
                temp = temp->middleNode;
            }
        }
        else if (temp->childNumber == 3)
        {
            if (index < temp->k1)
            {
                temp = temp->leftNode;
            }
            else if (index < temp->k2)
            {
                temp = temp->middleNode;
            }
            else
            {
                temp = temp->rightNode;
            }
        }
    }
    return temp;
}

bool TwoThreeTree::insertNode(int index, int data)
{
    if (root == NULL) // The tree is empty
    {
        root = initNode(NULL, index, data);
        return true;
    }
    twoThreeNode *temp = search(index);
    if (temp->k1 == index)
        return false; // Node with corresponding index already exists

    twoThreeNode *newNode = initNode(NULL, index, data);
    newNode->k1 = index;
    newNode->data = data;
    if (temp == root)
    {
        createNewRoot(newNode, temp);
        return true;
    }

    twoThreeNode *parent = temp->parentNode;
    newNode->parentNode = parent;

    // Insert newNode into parent
    insertIntoParent(parent, newNode);
    while (parent->childNumber == 4)
    {
        temp = splitNodeIntoTwo(parent);
        if (parent == root)
        {
            createNewRoot(parent, temp);
            return true;
        }
        else
        {
            temp->parentNode = parent->parentNode;
        }
        parent = parent->parentNode;
        insertIntoParent(parent, temp);
    }
    return true;
}

bool TwoThreeTree::removeNode(int index)
{
    return true;
}

bool TwoThreeTree::isLeaf(twoThreeNode *node)
{
    if (node == NULL)
    {
        return false;
    }
    else if (node->childNumber == 0)
    {
        return true;
    }
    return false;
}

twoThreeNode *TwoThreeTree::initNode(twoThreeNode *parent)
{
    twoThreeNode *node = new twoThreeNode;
    node->childNumber = 0;
    node->leftNode = node->middleNode = node->rightNode = node->tempNode = NULL;
    node->parentNode = parent;
    node->k1 = node->k2 = node->ktemp = 0;
    return node;
}

twoThreeNode *TwoThreeTree::initNode(twoThreeNode *parent, int index)
{
    twoThreeNode *node = new twoThreeNode;
    node->childNumber = 0;
    node->leftNode = node->middleNode = node->rightNode = node->tempNode = NULL;
    node->parentNode = parent;
    node->k1 = index;
    node->k2 = node->ktemp = 0;
    return node;
}

twoThreeNode *TwoThreeTree::initNode(twoThreeNode *parent, int index, int data)
{
    twoThreeNode *node = new twoThreeNode;
    node->childNumber = 0;
    node->leftNode = node->middleNode = node->rightNode = node->tempNode = NULL;
    node->parentNode = parent;
    node->k1 = index;
    node->k2 = node->ktemp = 0;
    node->data = data;
    return node;
}

void TwoThreeTree::createNewRoot(twoThreeNode *v1, twoThreeNode *v2)
{
    twoThreeNode *newRoot = initNode(NULL, v2->k1);
    v1->parentNode = v2->parentNode = newRoot;
    root = newRoot;
}

void TwoThreeTree::insertIntoParent(twoThreeNode *parent, twoThreeNode *newNode)
{
    if (newNode->k1 < parent->leftNode->k1)
    {
        parent->ktemp = parent->k2;
        parent->k2 = parent->k1;
        parent->k1 = newNode->k1;

        parent->tempNode = parent->rightNode;
        parent->rightNode = parent->middleNode;
        parent->middleNode = parent->leftNode;
        parent->leftNode = newNode;
    }
    else if (newNode->k1 < parent->k1)
    {
        parent->ktemp = parent->k2;
        parent->k2 = parent->k1;
        parent->k1 = newNode->k1;

        parent->tempNode = parent->rightNode;
        parent->rightNode = parent->middleNode;
        parent->middleNode = newNode;
    }
    else if (parent->k2 == 0 || newNode->k1 < parent->k2)
    {
        parent->ktemp = parent->k2;
        parent->k2 = newNode->k1;

        parent->tempNode = parent->rightNode;
        parent->rightNode = newNode;
    }
    else
    {
        parent->ktemp = newNode->k1;

        parent->tempNode = newNode;
    }
    parent->childNumber++;
}

twoThreeNode *TwoThreeTree::splitNodeIntoTwo(twoThreeNode *parent)
{
    twoThreeNode *v2 = initNode(NULL, parent->ktemp);
    v2->leftNode = parent->rightNode;
    v2->middleNode = parent->middleNode;

    parent->rightNode = parent->tempNode = NULL;
    parent->k2 = parent->ktemp = 0;

    parent->childNumber = v2->childNumber = 2;

    return v2;
}