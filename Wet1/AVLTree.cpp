#include "AVLTree.h"
using namespace std;

template <class dataType>
AVLTree<dataType>::AVLTree()
{
    root = nullptr;
}

template <class dataType>
AVLTree<dataType>::~AVLTree()
{
    if (root != nullptr)
        destroy(root);
}

template <class dataType>
void AVLTree<dataType>::destroy(AVLNode<dataType> *node)
{
    if (node != nullptr)
    {
        destroy(node->leftNode);
        destroy(node->rightNode);
        delete node;
    }
}

template <class dataType>
AVLNode<dataType> *AVLTree<dataType>::search(int key)
{
    // Check if the tree is empty
    if (root == nullptr)
        return nullptr;

    AVLNode<dataType> *temp = root;
    AVLNode<dataType> *parent;
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

template <class dataType>
bool AVLTree<dataType>::insert(int key, dataType data)
{
    if (root == nullptr)
    {
        root = initNewNode(key, data);
        return true;
    }
    AVLNode<dataType> *target = search(key);

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

template <class dataType>
bool AVLTree<dataType>::remove(int key)
{

    AVLNode<dataType> *toBeRemoved = search(key);
    if (toBeRemoved == nullptr)
        return false;
    else if (toBeRemoved->key != key)
    {
        return false;
    }

    // toBeRemoved has 2 children
    if (toBeRemoved->heightLeft >= 0 && toBeRemoved->heightRight >= 0)
    {
        AVLNode<dataType> *nextNode = findNextNode(toBeRemoved);
        swapTwoNodes(toBeRemoved, nextNode);

        toBeRemoved = nextNode;
    }

    AVLNode<dataType> *parent = toBeRemoved->parentNode;
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

template <class dataType>
bool AVLTree<dataType>::removeLeaf(AVLNode<dataType> *target)
{
    AVLNode<dataType> *parent = target->parentNode;
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

template <class dataType>
bool AVLTree<dataType>::removeSingleChild(AVLNode<dataType> *target)
{
    AVLNode<dataType> *parent = target->parentNode;

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

template <class dataType>
AVLNode<dataType> *AVLTree<dataType>::findNextNode(AVLNode<dataType> *node)
{
    node = node->rightNode;
    while (node->leftNode != nullptr)
    {
        node = node->leftNode;
    }
    return node;
}

template <class dataType>
void AVLTree<dataType>::swapTwoNodes(AVLNode<dataType> *v1, AVLNode<dataType> *v2)
{
    int key1, key2;
    key1 = v1->key;
    key2 = v2->key;

    int data1, data2;
    data1 = v1->data;
    data2 = v2->data;

    v1->key = key2;
    v2->key = key1;

    v1->data = data2;
    v2->data = data1;
}

template <class dataType>
void AVLTree<dataType>::insertLeftNaive(AVLNode<dataType> *target, int key, dataType data)
{
    AVLNode<dataType> *newNode = initNewNode(key, data);
    newNode->isLeftChild = true;
    newNode->parentNode = target;
    target->leftNode = newNode;
}

template <class dataType>
void AVLTree<dataType>::insertRightNaive(AVLNode<dataType> *target, int key, dataType data)
{
    AVLNode<dataType> *newNode = initNewNode(key, data);
    newNode->isRightChild = true;
    newNode->parentNode = target;
    target->rightNode = newNode;
}

template <class dataType>
void AVLTree<dataType>::updateNodeParameters(AVLNode<dataType> *target)
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

template <class dataType>
void AVLTree<dataType>::rollRight(AVLNode<dataType> *node)
{
    AVLNode<dataType> *temp = node->leftNode;
    AVLNode<dataType> *tempRight = temp->rightNode;
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

    AVLNode<dataType> *parent = node->parentNode;
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

template <class dataType>
void AVLTree<dataType>::rollLeft(AVLNode<dataType> *node)
{
    AVLNode<dataType> *temp = node->rightNode;
    AVLNode<dataType> *tempLeft = temp->leftNode;
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

    AVLNode<dataType> *parent = node->parentNode;
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

template <class dataType>
bool AVLTree<dataType>::rollNode(AVLNode<dataType> *node)
{
    AVLNode<dataType> *tempNode;
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

template <class dataType>
AVLNode<dataType> *AVLTree<dataType>::initNewNode(int key, dataType data)
{
    AVLNode<dataType> *node = new AVLNode<dataType>;

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

template <class dataType>
void AVLTree<dataType>::printInOrder()
{
    printInOrderAux(root);
}

template <class dataType>
void AVLTree<dataType>::printInOrderAux(AVLNode<dataType> *node)
{
    if (node == nullptr)
        return;

    printInOrderAux(node->leftNode);
    std::cout << node->key << std::endl;
    printInOrderAux(node->rightNode);
}

template class AVLTree<int>;