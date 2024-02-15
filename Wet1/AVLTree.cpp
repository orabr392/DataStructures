#include "AVLTree.h"
using namespace std;

AVLTree::AVLTree()
{
    root = nullptr;
}

AVLTree::~AVLTree()
{
    if (root != nullptr)
        destroy(root);
}

void AVLTree::destroy(AVLNode *node)
{
    if (node != nullptr)
    {
        destroy(node->leftNode);
        destroy(node->rightNode);
        delete node;
    }
}

AVLNode *AVLTree::search(int key)
{
    // Check if the tree is empty
    if (root == nullptr)
        return nullptr;

    AVLNode *temp = root;
    AVLNode *parent;
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

bool AVLTree::insert(int key, int data)
{
    if (root == nullptr)
    {
        root = initNewNode(key, data);
        return true;
    }
    AVLNode *target = search(key);

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

bool AVLTree::remove(int key)
{

    AVLNode *toBeRemoved = search(key);
    if (toBeRemoved == nullptr)
        return false;
    else if (toBeRemoved->key != key)
    {
        return false;
    }

    // toBeRemoved has 2 children
    if (toBeRemoved->heightLeft >= 0 && toBeRemoved->heightRight >= 0)
    {
        AVLNode *nextNode = findNextNode(toBeRemoved);
        swapTwoNodes(toBeRemoved, nextNode);

        toBeRemoved = nextNode;
    }

    AVLNode *parent = toBeRemoved->parentNode;
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
    while (parent != nullptr)
    {
        if (parent->balanceFactor == 2 || parent->balanceFactor == -2)
        {
            if (!rollNode(parent))
                return false;
        }
        parent = parent->parentNode;
    }
    return true;
}

bool AVLTree::removeLeaf(AVLNode *target)
{
    AVLNode *parent = target->parentNode;
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

bool AVLTree::removeSingleChild(AVLNode *target)
{
    AVLNode *parent = target->parentNode;
    if (target->heightLeft == -1 && target->heightRight >= 0)
    {
        // toBeRemoved is also the root
        if (parent == nullptr)
        {
            root = target->rightNode;
            root->parentNode = nullptr;
            target->rightNode = nullptr;
            destroy(target);
        }
        else
        {
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

    // toBeRemoved has a single child on the left
    else if (target->heightLeft >= 0 && target->heightRight == -1)
    {
        // toBeRemoved is also the root
        if (parent == nullptr)
        {
            root = target->leftNode;
            root->parentNode = nullptr;
            target->leftNode = nullptr;
            destroy(target);
        }
        else
        {
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

AVLNode *AVLTree::findNextNode(AVLNode *node)
{
    node = node->rightNode;
    while (node->leftNode != nullptr)
    {
        node = node->leftNode;
    }
    return node;
}

void AVLTree::swapTwoNodes(AVLNode *v1, AVLNode *v2)
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

void AVLTree::insertLeftNaive(AVLNode *target, int key, int data)
{
    AVLNode *newNode = initNewNode(key, data);
    newNode->isLeftChild = true;
    newNode->parentNode = target;
    target->leftNode = newNode;
}

void AVLTree::insertRightNaive(AVLNode *target, int key, int data)
{
    AVLNode *newNode = initNewNode(key, data);
    newNode->isRightChild = true;
    newNode->parentNode = target;
    target->rightNode = newNode;
}

void AVLTree::updateNodeParameters(AVLNode *target)
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

void AVLTree::rollRight(AVLNode *node)
{
    AVLNode *temp = node->leftNode;
    AVLNode *tempRight = temp->rightNode;
    node->leftNode = tempRight;
    temp->rightNode = node;

    AVLNode *parent = node->parentNode;
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
    updateNodeParameters(parent);
    updateNodeParameters(temp->rightNode);
    updateNodeParameters(temp);
}

void AVLTree::rollLeft(AVLNode *node)
{
    AVLNode *temp = node->rightNode;
    AVLNode *tempLeft = temp->leftNode;
    node->rightNode = tempLeft;
    temp->leftNode = node;

    AVLNode *parent = node->parentNode;
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
    updateNodeParameters(parent);
    updateNodeParameters(temp->leftNode);
    updateNodeParameters(temp);
}

bool AVLTree::rollNode(AVLNode *node)
{
    AVLNode *tempNode;
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

AVLNode *AVLTree::initNewNode(int key, int data)
{
    AVLNode *node = new AVLNode;

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

void AVLTree::printInOrder()
{
    printInOrderAux(root);
}

void AVLTree::printInOrderAux(AVLNode *node)
{
    if (node == nullptr)
        return;

    printInOrderAux(node->leftNode);
    std::cout << node->key << std::endl;
    printInOrderAux(node->rightNode);
}

AVLNode *AVLTree::getRoot()
{
    return root;
}
