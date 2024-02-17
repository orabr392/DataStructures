#include "AVLTree.h"
#include "Contestant.h"
#include "Country.h"
#include "Team.h"
#include "TwoKeysInt.h"

template <class dataType1, class dataType2>
AVLTree<dataType1, dataType2>::AVLTree()
{
    root = nullptr;
}

template <class dataType1, class dataType2>
AVLTree<dataType1, dataType2>::~AVLTree()
{
    if (root != nullptr)
        destroy(root);
}

template <class dataType1, class dataType2>
void AVLTree<dataType1, dataType2>::destroy(AVLNode<dataType1, dataType2> *node)
{
    if (node != nullptr)
    {
        destroy(node->leftNode);
        destroy(node->rightNode);
        delete node;
    }
}

template <class dataType1, class dataType2>
AVLNode<dataType1, dataType2> *AVLTree<dataType1, dataType2>::search(dataType1 key)
{
    // Check if the tree is empty
    if (root == nullptr)
        return nullptr;

    AVLNode<dataType1, dataType2> *temp = root;
    AVLNode<dataType1, dataType2> *parent;
    while (temp != nullptr)
    {
        parent = temp;
        // Return the node
        if (key == temp->key)
        {
            return temp;
        }
        // Traverse the correct direction
        else if (key < temp->key)
        {
            temp = temp->leftNode;
        }
        else
        {
            temp = temp->rightNode;
        }
    }
    return parent;
}

template <class dataType1, class dataType2>
bool AVLTree<dataType1, dataType2>::nodeExists(dataType1 key)
{
    AVLNode<dataType1, dataType2> *node = search(key);
    if (node == nullptr)
        return false;

    else if (key == node->key)
        return true;

    return false;
}

template <class dataType1, class dataType2>
bool AVLTree<dataType1, dataType2>::isTreeEmpty()
{
    if (root == nullptr)
        return true;
    return false;
}

template <class dataType1, class dataType2>
bool AVLTree<dataType1, dataType2>::insert(dataType1 key, dataType2 data)
{
    if (root == nullptr)
    {
        root = initNewNode(key, data);
        return true;
    }
    AVLNode<dataType1, dataType2> *target = search(key);

    // Check if the key already exists in the tree
    if (key == target->key)
        return false;

    // Insert to the left of target
    else if (key < target->key)
    {
        insertLeftNaive(target, key, data);
    }

    // Insert to the right of target
    else
    {
        insertRightNaive(target, key, data);
    }

    updateNodeParameters(target);
    // Traverse upwards and update parameters until you need to roll
    while (target != nullptr && target->balanceFactor != 2 && target->balanceFactor != -2)
    {
        target = target->parentNode;
        updateNodeParameters(target);
    }

    // Roll accordingly
    if (target != nullptr && (target->balanceFactor == 2 || target->balanceFactor == -2))
    {
        rollNode(target);
    }
    return true;
}

template <class dataType1, class dataType2>
bool AVLTree<dataType1, dataType2>::remove(dataType1 key)
{

    AVLNode<dataType1, dataType2> *toBeRemoved = search(key);
    if (toBeRemoved == nullptr)
        return false;
    else if (toBeRemoved->key != key)
    {
        return false;
    }

    // toBeRemoved has 2 children
    if (toBeRemoved->heightLeft >= 0 && toBeRemoved->heightRight >= 0)
    {
        AVLNode<dataType1, dataType2> *nextNode = findNextNode(toBeRemoved);
        swapTwoNodes(toBeRemoved, nextNode);

        toBeRemoved = nextNode;
    }

    AVLNode<dataType1, dataType2> *parent = toBeRemoved->parentNode;
    // If toBeRemoved is a leaf
    if (toBeRemoved->height == 0)
    {
        if (!removeLeaf(toBeRemoved))
            return false;
    }
    // toBeRemoved has a single child
    else if (toBeRemoved->height > 0 && (toBeRemoved->heightLeft == -1 || toBeRemoved->heightRight == -1))
    {
        if (!removeSingleChild(toBeRemoved))
            return false;
    }
    updateNodeParameters(parent);
    while (parent != nullptr)
    {
        if (parent->balanceFactor == 2 || parent->balanceFactor == -2)
        {
            if (!rollNode(parent))
                return false;
        }
        parent = parent->parentNode;
        updateNodeParameters(parent);
    }
    return true;
}

template <class dataType1, class dataType2>
bool AVLTree<dataType1, dataType2>::removeLeaf(AVLNode<dataType1, dataType2> *target)
{
    AVLNode<dataType1, dataType2> *parent = target->parentNode;
    // target is also the root
    if (parent == nullptr)
    {
        destroy(target);
        root = nullptr;
        return true;
    }
    else
    {
        if (target->isLeftChild)
        {
            parent->leftNode = nullptr;
            updateNodeParameters(parent);
            destroy(target);
        }
        else if (target->isRightChild)
        {
            parent->rightNode = nullptr;
            updateNodeParameters(parent);
            destroy(target);
        }
        else
        {
            // Throw an exception
            return false;
        }
    }
    return true;
}

template <class dataType1, class dataType2>
bool AVLTree<dataType1, dataType2>::removeSingleChild(AVLNode<dataType1, dataType2> *target)
{
    AVLNode<dataType1, dataType2> *parent = target->parentNode;

    // target has a single child on the right
    if (target->heightLeft == -1 && target->heightRight >= 0)
    {
        // target is also the root
        if (parent == nullptr)
        {
            root = target->rightNode;
            root->parentNode = nullptr;
            target->rightNode = nullptr;
            destroy(target);
        }
        else
        {
            target->rightNode->isLeftChild = target->isLeftChild;
            target->rightNode->isRightChild = target->isRightChild;
            if (target->isLeftChild)
            {
                parent->leftNode = target->rightNode;
                target->rightNode->parentNode = parent;
                target->rightNode = nullptr;
                updateNodeParameters(parent);
                destroy(target);
            }
            else if (target->isRightChild)
            {
                parent->rightNode = target->rightNode;
                target->rightNode->parentNode = parent;
                target->rightNode = nullptr;
                updateNodeParameters(parent);
                destroy(target);
            }
            else
            {
                // Throw an exception
                return false;
            }
        }
    }

    // target has a single child on the left
    else if (target->heightLeft >= 0 && target->heightRight == -1)
    {
        // target is also the root
        if (parent == nullptr)
        {
            root = target->leftNode;
            root->parentNode = nullptr;
            target->leftNode = nullptr;
            destroy(target);
        }
        else
        {
            target->leftNode->isLeftChild = target->isLeftChild;
            target->leftNode->isRightChild = target->isRightChild;
            if (target->isLeftChild)
            {
                parent->leftNode = target->leftNode;
                target->leftNode->parentNode = parent;
                target->leftNode = nullptr;
                updateNodeParameters(parent);
                destroy(target);
            }
            else if (target->isRightChild)
            {
                parent->rightNode = target->leftNode;
                target->leftNode->parentNode = parent;
                target->leftNode = nullptr;
                updateNodeParameters(parent);
                destroy(target);
            }
            else
            {
                // Throw an exception
                return false;
            }
        }
    }
    return true;
}

template <class dataType1, class dataType2>
AVLNode<dataType1, dataType2> *AVLTree<dataType1, dataType2>::findNextNode(AVLNode<dataType1, dataType2> *node)
{
    node = node->rightNode;
    while (node->leftNode != nullptr)
    {
        node = node->leftNode;
    }
    return node;
}

template <class dataType1, class dataType2>
void AVLTree<dataType1, dataType2>::swapTwoNodes(AVLNode<dataType1, dataType2> *v1, AVLNode<dataType1, dataType2> *v2)
{
    dataType1 key1, key2;
    key1 = v1->key;
    key2 = v2->key;

    dataType2 data1, data2;
    data1 = v1->data;
    data2 = v2->data;

    v1->key = key2;
    v2->key = key1;

    v1->data = data2;
    v2->data = data1;
}

template <class dataType1, class dataType2>
void AVLTree<dataType1, dataType2>::insertLeftNaive(AVLNode<dataType1, dataType2> *target, dataType1 key, dataType2 data)
{
    AVLNode<dataType1, dataType2> *newNode = initNewNode(key, data);
    newNode->isLeftChild = true;
    newNode->parentNode = target;
    target->leftNode = newNode;
}

template <class dataType1, class dataType2>
void AVLTree<dataType1, dataType2>::insertRightNaive(AVLNode<dataType1, dataType2> *target, dataType1 key, dataType2 data)
{
    AVLNode<dataType1, dataType2> *newNode = initNewNode(key, data);
    newNode->isRightChild = true;
    newNode->parentNode = target;
    target->rightNode = newNode;
}

template <class dataType1, class dataType2>
void AVLTree<dataType1, dataType2>::updateNodeParameters(AVLNode<dataType1, dataType2> *target)
{
    // Check that the target is not empty
    if (target == nullptr)
        return;

    // Update relevant parameters
    if (target->leftNode != nullptr)
    {
        target->heightLeft = target->leftNode->height;
    }
    else
    {
        target->heightLeft = -1;
    }

    if (target->rightNode != nullptr)
    {
        target->heightRight = target->rightNode->height;
    }
    else
    {
        target->heightRight = -1;
    }

    if (target->heightLeft > target->heightRight)
    {
        target->height = target->heightLeft + 1;
    }
    else
    {
        target->height = target->heightRight + 1;
    }

    target->balanceFactor = target->heightLeft - target->heightRight;
}

template <class dataType1, class dataType2>
void AVLTree<dataType1, dataType2>::rollRight(AVLNode<dataType1, dataType2> *node)
{
    AVLNode<dataType1, dataType2> *temp = node->leftNode;
    AVLNode<dataType1, dataType2> *tempRight = temp->rightNode;
    node->leftNode = tempRight;

    updateNodeParameters(node);
    if (tempRight != nullptr)
    {
        tempRight->parentNode = node;
        tempRight->isLeftChild = true;
        tempRight->isRightChild = false;
    }
    temp->rightNode = node;
    updateNodeParameters(temp);

    AVLNode<dataType1, dataType2> *parent = node->parentNode;
    // Update nodes parent that his child is now temp and not node
    if (parent != nullptr)
    {
        if (node->isLeftChild)
        {
            parent->leftNode = temp;
        }
        else if (node->isRightChild)
        {
            parent->rightNode = temp;
        }
        // Just to be safe
        else
        {
            // Throw an exception
        }
    }
    // If node was the root and we had to roll it to the right
    else
    {
        root = temp;
    }
    temp->isLeftChild = node->isLeftChild;
    temp->isRightChild = node->isRightChild;
    node->isLeftChild = false;
    node->isRightChild = true;
    node->parentNode = temp;
    temp->parentNode = parent;
}

template <class dataType1, class dataType2>
void AVLTree<dataType1, dataType2>::rollLeft(AVLNode<dataType1, dataType2> *node)
{
    AVLNode<dataType1, dataType2> *temp = node->rightNode;
    AVLNode<dataType1, dataType2> *tempLeft = temp->leftNode;
    node->rightNode = tempLeft;

    updateNodeParameters(node);
    if (tempLeft != nullptr)
    {
        tempLeft->parentNode = node;
        tempLeft->isLeftChild = false;
        tempLeft->isRightChild = true;
    }
    temp->leftNode = node;
    updateNodeParameters(temp);

    AVLNode<dataType1, dataType2> *parent = node->parentNode;
    // Update nodes parent that his child is now temp and not node
    if (parent != nullptr)
    {
        if (node->isLeftChild)
        {
            parent->leftNode = temp;
        }
        else if (node->isRightChild)
        {
            parent->rightNode = temp;
        }
        // Just to be safe
        else
        {
            // Throw an exception
        }
    }
    // If node was the root and we had to roll it to the right
    else
    {
        root = temp;
    }
    temp->isLeftChild = node->isLeftChild;
    temp->isRightChild = node->isRightChild;
    node->isLeftChild = true;
    node->isRightChild = false;
    node->parentNode = temp;
    temp->parentNode = parent;
}

template <class dataType1, class dataType2>
bool AVLTree<dataType1, dataType2>::rollNode(AVLNode<dataType1, dataType2> *node)
{
    AVLNode<dataType1, dataType2> *tempNode;
    if (node->balanceFactor == 2)
    {
        tempNode = node->leftNode;
        if (tempNode->balanceFactor >= 0)
        {
            // Roll LL
            rollRight(node);
        }
        else if (tempNode->balanceFactor == -1)
        {
            // Roll LR
            rollLeft(tempNode);
            rollRight(node);
        }
        else
        {
            // Throw an exception
            return false;
        }
    }
    else if (node->balanceFactor == -2)
    {
        tempNode = node->rightNode;
        if (tempNode->balanceFactor <= 0)
        {
            // Roll RR
            rollLeft(node);
        }
        else if (tempNode->balanceFactor == 1)
        {
            // Roll RL
            rollRight(tempNode);
            rollLeft(node);
        }
        else
        {
            // Throw an exception
            return false;
        }
    }
    else
    {
        // Throw an exception
        return false;
    }
    return true;
}

template <class dataType1, class dataType2>
AVLNode<dataType1, dataType2> *AVLTree<dataType1, dataType2>::initNewNode(dataType1 key, dataType2 data)
{
    AVLNode<dataType1, dataType2> *node = new AVLNode<dataType1, dataType2>;

    // Init node data
    node->key = key;
    node->data = data;
    node->height = 0;
    node->heightLeft = -1;
    node->heightRight = -1;
    node->balanceFactor = 0;

    node->isLeftChild = node->isRightChild = false;

    // Set links to be empty
    node->leftNode = node->rightNode = node->parentNode = nullptr;

    return node;
}

template <class dataType1, class dataType2>
void AVLTree<dataType1, dataType2>::printInOrder()
{
    printInOrderAux(root);
}

template <class dataType1, class dataType2>
dataType2 *AVLTree<dataType1, dataType2>::getRightMostNode()
{
    AVLNode<dataType1, dataType2> *temp = root;
    if (temp == nullptr)
        return nullptr;
    while (temp->rightNode != nullptr)
        temp = temp->rightNode;
    return &temp->data;
}

template <class dataType1, class dataType2>
dataType2 *AVLTree<dataType1, dataType2>::getLeftMostNode()
{
    AVLNode<dataType1, dataType2> *temp = root;
    while (temp->leftNode != nullptr)
        temp = temp->leftNode;
    return &temp->data;
}

template <class dataType1, class dataType2>
void AVLTree<dataType1, dataType2>::printInOrderAux(AVLNode<dataType1, dataType2> *node)
{
    if (node == nullptr)
        return;

    printInOrderAux(node->leftNode);
    std::cout << node->key << std::endl;
    printInOrderAux(node->rightNode);
}

template class AVLTree<int, Country>;
template class AVLTree<int, Team>;
template class AVLTree<int, Contestant>;
template class AVLTree<TwoKeysInt, Contestant>;