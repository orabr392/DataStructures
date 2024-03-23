#include <cmath>
#include <iostream>

#include "StrNode.h"
#include "Team.h"
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
};

/*
 *   StrTree: Empty constructor
 */

StrTree::StrTree() {
    root = nullptr;
    treeSize = 0;
}

/*
 * Constructor with arguments
 */

StrTree::StrTree(StrNode<TwoKeysInt>* newRoot, int size) {
    root = newRoot;
    treeSize = size;
}

StrTree::StrTree(int height) {
    if (height >= 0) {
        root = new StrNode<TwoKeysInt>();

        root->leftNode = createEmptyTreeAux(root, height - 1, true, false);
        root->rightNode = createEmptyTreeAux(root, height - 1, false, true);
        root->parentNode = nullptr;
        updateNodeParameters(root, 0);
        treeSize = std::pow(2, height + 1) - 1;
    } else {
        root = nullptr;
        treeSize = 0;
    }
}

StrTree::StrTree(StrTree& otherStrTree) {
    this->root = otherStrTree.root;
    this->treeSize = otherStrTree.treeSize;
}

/*
 *   ~StrTree(): Destructor
 */

StrTree::~StrTree() {
    if (root != nullptr) destroy(root);
}

/*
 *   destroy: Removes each node using postorder search
 *
 *  @param node - Pointer to an StrNode, which is a either a leaf or an
 * internal node
 */

void StrTree::destroy(StrNode<TwoKeysInt>* node) {
    if (node != nullptr) {
        destroy(node->leftNode);
        destroy(node->rightNode);
        delete node;
    }
}

void StrTree::disconnectRoot() {
    this->root = nullptr;
    this->treeSize = 0;
}

/*
 *  search: Search for the node with a given key,
 *  in case it does not exist it returns closest node (from the left)
 *
 * @param key - The unique key of the node
 * @return
 * 	nullptr - The tree is empty
 *  StrNode* - The closest node to the target
 *
 */

StrNode<TwoKeysInt>* StrTree::search(TwoKeysInt key) {
    // Check if the tree is empty
    if (root == nullptr) return nullptr;

    StrNode<TwoKeysInt>* temp = root;
    StrNode<TwoKeysInt>* parent;
    while (temp != nullptr) {
        parent = temp;
        // Return the node
        if (key == temp->key) {
            return temp;
        }
        // Traverse the correct direction
        else if (key < temp->key) {
            temp = temp->leftNode;
        } else {
            temp = temp->rightNode;
        }
    }
    return parent;
}

StrNode<TwoKeysInt>* StrTree::search(TwoKeysInt key, int* sumExtras) {
    // Check if the tree is empty
    if (root == nullptr) return nullptr;

    StrNode<TwoKeysInt>* temp = root;
    StrNode<TwoKeysInt>* parent;
    while (temp != nullptr) {
        parent = temp;
        // Return the node
        if (key == temp->key) {
            *sumExtras += temp->extra;
            return temp;
        }
        // Traverse the correct direction
        else if (key < temp->key) {
            temp = temp->leftNode;
        } else {
            *sumExtras += temp->extra;
            temp = temp->rightNode;
        }
    }
    return parent;
}

/*
 *   findIndex: Find the rank of the node with corresponding key
 *
 *  @param key - The unique key of the node
 *  @return - The rank of the node
 */
int StrTree::findIndex(TwoKeysInt key) {
    // Check if the tree is empty or the node doesn't exist
    if (!nodeExists(key)) {
        return 0;
    }

    int rank = 0;
    int leftWeight;
    StrNode<TwoKeysInt>* temp = root;
    while (temp->key != key) {
        // Traverse the correct direction
        if (key < temp->key) {
            temp = temp->leftNode;
        } else {
            if (temp->leftNode == nullptr) {
                leftWeight = 0;
            } else {
                leftWeight = temp->leftNode->weight;
            }
            rank += leftWeight + 1;
            temp = temp->rightNode;
        }
    }
    if (temp->leftNode == nullptr) {
        leftWeight = 0;
    } else {
        leftWeight = temp->leftNode->weight;
    }
    return rank + leftWeight + 1;
}

StrNode<TwoKeysInt>* StrTree::getNodeByRank(int rank) {
    // Check that the tree contains a node with corresponding rank
    if (rank <= 0 || rank > treeSize) {
        return nullptr;
    }
    StrNode<TwoKeysInt>* target = root;
    int leftWeight;
    while (target != nullptr) {
        if (target->leftNode == nullptr) {
            leftWeight = 0;
        } else {
            leftWeight = target->leftNode->weight;
        }
        if (leftWeight == (rank - 1))
            return target;
        else if (leftWeight > (rank - 1)) {
            target = target->leftNode;
        } else {
            rank -= (leftWeight + 1);
            target = target->rightNode;
        }
    }

    // Just to be safe (shouldn't reach here)
    return nullptr;
}

/*
 *  nodeExists: Checks whether a node with the given key exists
 *
 * @param key - The unique key of the node
 * @return
 * 	true - There exists the node with this key
 *  false - There doesnt exists a node with this key
 */

bool StrTree::nodeExists(TwoKeysInt key) {
    StrNode<TwoKeysInt>* node = search(key);
    if (node == nullptr) {
        return false;
    }

    else if (key == node->key) {
        return true;
    }

    return false;
}

/*
 *  isTreeEmpty: Check whether the tree is empty or not
 *
 * @return
 * 	true - The tree is empty
 *  false - The tree is not empty
 */

bool StrTree::isTreeEmpty() {
    if (root == nullptr) return true;
    return false;
}

/*
 *  insert: Inserts a new node with given key and data
 *
 * @param key - The unique key of the node
 * @param data - The data of the node
 * @return
 * 	true - The new node was added
 *  false - The new node was not added, perhaps it already existed
 */

bool StrTree::insert(TwoKeysInt key, int data, int medals) {
    if (root == nullptr) {
        root = new StrNode<TwoKeysInt>(key, data);
        root->medals = medals;

        treeSize++;
        return true;
    }
    int sumExtras = 0;
    StrNode<TwoKeysInt>* target = search(key, &sumExtras);

    // Check if the key already exists in the tree
    if (key == target->key) {
        return false;
    }

    // Insert to the left of target
    else if (key < target->key) {
        insertLeftNaive(target, key, data, sumExtras, medals);
    }

    // Insert to the right of target
    else {
        insertRightNaive(target, key, data, sumExtras, medals);
    }

    updateNodeParameters(target, sumExtras);
    // Traverse upwards and update parameters until you need to roll
    while (target != nullptr && target->balanceFactor != 2 &&
           target->balanceFactor != -2) {
        if (target->isRightChild) {
            sumExtras -= target->extra;
        }
        target = target->parentNode;
        updateNodeParameters(target, sumExtras);
    }

    // Roll accordingly
    if (target != nullptr &&
        (target->balanceFactor == 2 || target->balanceFactor == -2)) {
        rollNode(target);
    }
    treeSize++;

    // Search for the newly inserted node
    target = search(key, &sumExtras);
    if (key != target->key) {
        return false;
    }

    // Go up the search route and update the parameters
    while (target != nullptr) {
        updateNodeParameters(target, sumExtras);
        if (target->isRightChild) {
            sumExtras -= target->extra;
        }
        target = target->parentNode;
    }
    return true;
}

/*
 *  remove: Removes a node with a given key
 *
 * @param key - The unique key of the node
 * @return
 * 	true - Managed to remove the node
 *  false - Did not manage to remove the node, perhaps it did not exist
 */

bool StrTree::remove(TwoKeysInt key) {
    int sumExtras = 0;
    StrNode<TwoKeysInt>* toBeRemoved = search(key, &sumExtras);
    if (toBeRemoved == nullptr)
        return false;
    else if (toBeRemoved->key != key) {
        return false;
    }

    // toBeRemoved has 2 children
    if (toBeRemoved->heightLeft >= 0 && toBeRemoved->heightRight >= 0) {
        StrNode<TwoKeysInt>* nextNode = findNextNode(toBeRemoved);
        swapTwoNodes(toBeRemoved, nextNode);
    }

    StrNode<TwoKeysInt>* parent = toBeRemoved->parentNode;
    // If toBeRemoved is a leaf
    if (toBeRemoved->height == 0) {
        if (!removeLeaf(toBeRemoved)) return false;
    }
    // toBeRemoved has a single child
    else if (toBeRemoved->height > 0 && (toBeRemoved->heightLeft == -1 ||
                                         toBeRemoved->heightRight == -1)) {
        if (!removeSingleChild(toBeRemoved)) return false;
    }

    StrNode<TwoKeysInt>* tempNode = parent;
    updateNodeParameters(tempNode);
    while (tempNode != nullptr) {
        if (tempNode->balanceFactor == 2 || tempNode->balanceFactor == -2) {
            if (!rollNode(tempNode)) return false;
        }
        tempNode = tempNode->parentNode;
        updateNodeParameters(tempNode);
    }
    treeSize--;

    // Go up the search route from parent to root
    while (parent != nullptr) {
        updateNodeParameters(parent, sumExtras);
        if (parent->isRightChild) {
            sumExtras -= parent->extra;
        }

        parent = parent->parentNode;
    }
    return true;
}

/*
 *  removeLeaf: Removes a node of type leaf
 *
 * @param target - A pointer to the node to be deleted
 * @return
 * 	true - Managed to remove the node
 *  false - Did not manage to remove the node, perhaps it did not exist
 */

bool StrTree::removeLeaf(StrNode<TwoKeysInt>* target) {
    StrNode<TwoKeysInt>* parent = target->parentNode;
    // target is also the root
    if (parent == nullptr) {
        root = nullptr;

        destroy(target);

        return true;
    } else {
        if (target->isLeftChild) {
            parent->leftNode = nullptr;
            updateNodeParameters(parent);

            destroy(target);
        } else if (target->isRightChild) {
            parent->rightNode = nullptr;
            updateNodeParameters(parent);

            destroy(target);
        } else {
            // Throw an exception
            return false;
        }
    }
    return true;
}

/*
 *  removeSingleChild: Removes a node that has only a single child
 *
 * @param target - A pointer to the node to be deleted
 * @return
 * 	true - Managed to remove the node
 *  false - Did not manage to remove the node, perhaps it did not exist
 */

bool StrTree::removeSingleChild(StrNode<TwoKeysInt>* target) {
    StrNode<TwoKeysInt>* parent = target->parentNode;
    int extra = target->extra;
    // target has a single child on the right
    if (target->heightLeft == -1 && target->heightRight >= 0) {
        target->rightNode->extra += extra;
        target->rightNode->medals -= extra;

        // target is also the root
        if (parent == nullptr) {
            root = target->rightNode;
            root->parentNode = nullptr;
            target->rightNode = nullptr;

            destroy(target);
        } else {
            target->rightNode->isLeftChild = target->isLeftChild;
            target->rightNode->isRightChild = target->isRightChild;
            if (target->isLeftChild) {
                parent->leftNode = target->rightNode;
                target->rightNode->parentNode = parent;
                target->rightNode = nullptr;
                updateNodeParameters(parent);

                destroy(target);
            } else if (target->isRightChild) {
                parent->rightNode = target->rightNode;
                target->rightNode->parentNode = parent;
                target->rightNode = nullptr;
                updateNodeParameters(parent);

                destroy(target);
            } else {
                // Throw an exception
                return false;
            }
        }
    }

    // target has a single child on the left
    else if (target->heightLeft >= 0 && target->heightRight == -1) {
        target->leftNode->extra += extra;
        target->leftNode->medals -= extra;

        // target is also the root
        if (parent == nullptr) {
            root = target->leftNode;
            root->parentNode = nullptr;
            target->leftNode = nullptr;

            destroy(target);
        } else {
            target->leftNode->isLeftChild = target->isLeftChild;
            target->leftNode->isRightChild = target->isRightChild;
            if (target->isLeftChild) {
                parent->leftNode = target->leftNode;
                target->leftNode->parentNode = parent;
                target->leftNode = nullptr;
                updateNodeParameters(parent);
                destroy(target);
            } else if (target->isRightChild) {
                parent->rightNode = target->leftNode;
                target->leftNode->parentNode = parent;
                target->leftNode = nullptr;
                updateNodeParameters(parent);

                destroy(target);
            } else {
                // Throw an exception
                return false;
            }
        }
    }
    return true;
}

/*
 *  findNextNode: Finds the next node, the one that is the least greater
 * than the current node
 *
 * @param node - The node that we have to find its neighboor
 * @return
 * 	StrNode* - The next node in the tree
 */

StrNode<TwoKeysInt>* StrTree::findNextNode(StrNode<TwoKeysInt>* node) {
    node = node->rightNode;
    while (node->leftNode != nullptr) {
        node = node->leftNode;
    }
    return node;
}

/*
 *  swapTwoNodes: Swaps two nodes
 *
 * @param v1 - The first node
 * @param v2 - The second node
 */

void StrTree::swapTwoNodes(StrNode<TwoKeysInt>* v1, StrNode<TwoKeysInt>* v2) {
    StrNode<TwoKeysInt>* leftNodeV1 = v1->leftNode;
    StrNode<TwoKeysInt>* rightNodeV1 = v1->rightNode;
    StrNode<TwoKeysInt>* parentNodeV1 = v1->parentNode;

    if (v1->rightNode == v2) {
        v1->rightNode = v2->rightNode;
        v1->leftNode = v2->leftNode;
        v1->parentNode = v2;

        v2->rightNode = v1;
        v2->leftNode = leftNodeV1;
        v2->parentNode = parentNodeV1;
    } else {
        v1->rightNode = v2->rightNode;
        v1->leftNode = v2->leftNode;
        v1->parentNode = v2->parentNode;

        v2->rightNode = rightNodeV1;
        v2->leftNode = leftNodeV1;
        v2->parentNode = parentNodeV1;
    }
    if (v2->parentNode == nullptr) {
        root = v2;
        v1->isLeftChild = v2->isLeftChild;
        v1->isRightChild = v2->isRightChild;
    } else {
        if (v1->isLeftChild) {
            v1->isLeftChild = v2->isLeftChild;
            v1->isRightChild = v2->isRightChild;
            v2->parentNode->leftNode = v2;

            v2->isLeftChild = true;
            v2->isRightChild = false;
        } else if (v1->isRightChild) {
            v1->isLeftChild = v2->isLeftChild;
            v1->isRightChild = v2->isRightChild;
            v2->parentNode->rightNode = v2;

            v2->isLeftChild = false;
            v2->isRightChild = true;
        }
    }

    if (v1->leftNode != nullptr) {
        v1->leftNode->parentNode = v1;
    }
    if (v1->rightNode != nullptr) {
        v1->rightNode->parentNode = v1;
    }
    if (v2->leftNode != nullptr) {
        v2->leftNode->parentNode = v2;
    }
    if (v2->rightNode != nullptr) {
        v2->rightNode->parentNode = v2;
    }
    int tempExtra = v1->extra;
    v1->extra = v2->extra;
    v2->extra = tempExtra;
    updateNodeParameters(v1);
    updateNodeParameters(v2);
}

/*
 *  insertLeftNaive: Insert a new node as a left child of target with
 * given key and data
 *
 * @param target - The node to connect the new node to from the left
 * @param key - The unique key to the new node
 * @param data - The data of the new node
 */

void StrTree::insertLeftNaive(StrNode<TwoKeysInt>* target, TwoKeysInt key,
                              int data, int sumExtras, int medals) {
    StrNode<TwoKeysInt>* newNode = new StrNode<TwoKeysInt>(key, data);
    newNode->medals = medals - sumExtras;

    newNode->isLeftChild = true;
    newNode->parentNode = target;
    target->leftNode = newNode;
}

/*
 *  insertLeftNaive: Insert a new node as a left child of target with
 * given key and data
 *
 * @param target - The node to connect the new node to from the left
 * @param key - The unique key to the new node
 * @param data - The data of the new node
 */

void StrTree::insertRightNaive(StrNode<TwoKeysInt>* target, TwoKeysInt key,
                               int data, int sumExtras, int medals) {
    StrNode<TwoKeysInt>* newNode = new StrNode<TwoKeysInt>(key, data);
    newNode->medals = medals - sumExtras;

    newNode->isRightChild = true;
    newNode->parentNode = target;
    target->rightNode = newNode;
}

/*
 *  updateNodeParameters: Update the parameters of the node,
 *                         Such as height, heightLeft, heightRight, BF and
 * so on.
 *
 * @param target - The node to update its parameters
 */

void StrTree::updateNodeParameters(StrNode<TwoKeysInt>* target) {
    // Check that the target is not empty
    if (target == nullptr) return;

    int targetWeight = 1;

    int targetStrength = target->key.key1;

    // Update relevant parameters
    if (target->leftNode != nullptr) {
        target->heightLeft = target->leftNode->height;
        targetWeight += target->leftNode->weight;
        if (target->maxRank < target->leftNode->maxRank) {
            target->maxRank = target->leftNode->maxRank;
        }
    } else {
        target->heightLeft = -1;
    }

    if (target->rightNode != nullptr) {
        target->heightRight = target->rightNode->height;
        targetWeight += target->rightNode->weight;
        if (target->maxRank < target->rightNode->maxRank) {
            target->maxRank = target->rightNode->maxRank;
        }
    } else {
        target->heightRight = -1;
    }

    if (target->heightLeft > target->heightRight) {
        target->height = target->heightLeft + 1;
    } else {
        target->height = target->heightRight + 1;
    }

    target->balanceFactor = target->heightLeft - target->heightRight;
    target->weight = targetWeight;
}

void StrTree::updateNodeParameters(StrNode<TwoKeysInt>* target,
                                   int sumExtras) {
    // Check that the target is not empty
    if (target == nullptr) return;

    int targetWeight = 1;

    int targetStrength = target->key.key1;
    int targetMaxRank = target->medals + targetStrength + sumExtras;

    // Update relevant parameters
    if (target->leftNode != nullptr) {
        target->heightLeft = target->leftNode->height;
        targetWeight += target->leftNode->weight;
        if (targetMaxRank < target->leftNode->maxRank) {
            targetMaxRank = target->leftNode->maxRank;
        }
    } else {
        target->heightLeft = -1;
    }

    if (target->rightNode != nullptr) {
        target->heightRight = target->rightNode->height;
        targetWeight += target->rightNode->weight;
        if (targetMaxRank < target->rightNode->maxRank) {
            targetMaxRank = target->rightNode->maxRank;
        }
    } else {
        target->heightRight = -1;
    }

    if (target->heightLeft > target->heightRight) {
        target->height = target->heightLeft + 1;
    } else {
        target->height = target->heightRight + 1;
    }

    target->balanceFactor = target->heightLeft - target->heightRight;
    target->weight = targetWeight;
    target->maxRank = targetMaxRank;
}

/*
 *  rollRight: Roll to the right (as seen in the recitation)
 *
 * @param node - The node to roll from
 */

void StrTree::rollRight(StrNode<TwoKeysInt>* node) {
    StrNode<TwoKeysInt>* temp = node->leftNode;
    StrNode<TwoKeysInt>* tempRight = temp->rightNode;

    // Change the extra of temp to be e_b - e_a
    node->extra = node->extra - temp->extra;

    node->leftNode = tempRight;

    updateNodeParameters(node);
    if (tempRight != nullptr) {
        tempRight->parentNode = node;
        tempRight->isLeftChild = true;
        tempRight->isRightChild = false;
    }
    temp->rightNode = node;
    updateNodeParameters(temp);

    StrNode<TwoKeysInt>* parent = node->parentNode;
    // Update nodes parent that his child is now temp and not node
    if (parent != nullptr) {
        if (node->isLeftChild) {
            parent->leftNode = temp;
        } else if (node->isRightChild) {
            parent->rightNode = temp;
        }
        // Just to be safe
        else {
            // Throw an exception
        }
    }
    // If node was the root and we had to roll it to the right
    else {
        root = temp;
    }
    temp->isLeftChild = node->isLeftChild;
    temp->isRightChild = node->isRightChild;
    node->isLeftChild = false;
    node->isRightChild = true;
    node->parentNode = temp;
    temp->parentNode = parent;
}

/*
 *  rollLeft: Roll to the left (as seen in the recitation)
 *
 * @param node - The node to roll from
 */

void StrTree::rollLeft(StrNode<TwoKeysInt>* node) {
    StrNode<TwoKeysInt>* temp = node->rightNode;
    StrNode<TwoKeysInt>* tempLeft = temp->leftNode;

    // Change the extra of temp to be e_b - e_a
    node->extra = node->extra + temp->extra;

    node->rightNode = tempLeft;

    updateNodeParameters(node);
    if (tempLeft != nullptr) {
        tempLeft->parentNode = node;
        tempLeft->isLeftChild = false;
        tempLeft->isRightChild = true;
    }
    temp->leftNode = node;
    updateNodeParameters(temp);

    StrNode<TwoKeysInt>* parent = node->parentNode;
    // Update nodes parent that his child is now temp and not node
    if (parent != nullptr) {
        if (node->isLeftChild) {
            parent->leftNode = temp;
        } else if (node->isRightChild) {
            parent->rightNode = temp;
        }
        // Just to be safe
        else {
            // Throw an exception
        }
    }
    // If node was the root and we had to roll it to the right
    else {
        root = temp;
    }
    temp->isLeftChild = node->isLeftChild;
    temp->isRightChild = node->isRightChild;
    node->isLeftChild = true;
    node->isRightChild = false;
    node->parentNode = temp;
    temp->parentNode = parent;
}

/*
 *  rollNode: Find the correct roll needed to fix the tree so that it
 * remains balanced
 *
 * @param node - The node to roll from
 */

bool StrTree::rollNode(StrNode<TwoKeysInt>* node) {
    StrNode<TwoKeysInt>* tempNode;
    if (node->balanceFactor == 2) {
        tempNode = node->leftNode;
        if (tempNode->balanceFactor >= 0) {
            // Roll LL
            rollRight(node);
        } else if (tempNode->balanceFactor == -1) {
            // Roll LR
            rollLeft(tempNode);
            rollRight(node);
        } else {
            // Throw an exception
            return false;
        }
    } else if (node->balanceFactor == -2) {
        tempNode = node->rightNode;
        if (tempNode->balanceFactor <= 0) {
            // Roll RR
            rollLeft(node);
        } else if (tempNode->balanceFactor == 1) {
            // Roll RL
            rollRight(tempNode);
            rollLeft(node);
        } else {
            // Throw an exception
            return false;
        }
    } else {
        // Throw an exception
        return false;
    }
    return true;
}

/*
 *  printInOrder: Print the tree inOrder - just wrapper function
 */

void StrTree::printInOrder() { printInOrderAux(root); }

/*
 *   getRightMostNode: Get the right most node
 *
 * @return
 *   nullptr - If the tree is empty
 *   S* - A pointer to the right most node
 */

StrNode<TwoKeysInt>* StrTree::getRightMostNode() {
    StrNode<TwoKeysInt>* temp = root;
    if (temp == nullptr) return nullptr;
    while (temp->rightNode != nullptr) temp = temp->rightNode;
    // return new S(temp->data);
    return temp;
}

/*
 *   getLeftMostNode: Get the left most node
 *
 * @return
 *   nullptr - If the tree is empty
 *   S* - A pointer to the left most node
 */

StrNode<TwoKeysInt>* StrTree::getLeftMostNode() {
    StrNode<TwoKeysInt>* temp = root;
    if (temp == nullptr) return nullptr;
    while (temp->leftNode != nullptr) temp = temp->leftNode;
    // return new S(temp->data);
    return temp;
}

/*
 *   printInOrderAux: Print inOrder - an auxiliary
 *
 * @param node - The node to print from
 */

void StrTree::printInOrderAux(StrNode<TwoKeysInt>* node) {
    if (node == nullptr) return;

    printInOrderAux(node->leftNode);
    std::cout << node->key << std::endl;
    printInOrderAux(node->rightNode);
}

int StrTree::getTreeSize() { return treeSize; }

/*
 * Running Inorder traversal and placing nodes into array
 * @param root - tree to be copied into array
 * @param arr - array to store the tree
 *  @return
 *          int
 */

static int inorderToArray(StrNode<TwoKeysInt>* root, StrNode<TwoKeysInt>* arr,
                          int arrSize, int i) {
    if (root == nullptr) return i;

    i = inorderToArray(root->leftNode, arr, arrSize, i);
    if (i > arrSize - 1) return i;
    arr[i++] = *root;
    return inorderToArray(root->rightNode, arr, arrSize, i);
}

/*
 * Running inorder traversal and placing arrays' items into the
 * nearly-complete empty tree
 *  @param root - tree to store the array
 *  @param arr - array to be copied into tree
 *  @return
 *          int
 */

int inorderToTree(StrNode<TwoKeysInt>* node, StrNode<TwoKeysInt>* arr,
                  int arrSize, int i) {
    if (node == nullptr) return i;

    i = inorderToTree(node->leftNode, arr, arrSize, i);
    if (i >= arrSize) return i;
    node->key = arr[i].key;
    node->data = arr[i].data;
    i++;
    return inorderToTree(node->rightNode, arr, arrSize, i);
}

/*
 * Combining tree1 and tree2 into newTree (which is nearly-complete and
 * empty) using Inorder arrays Might need operator= for Node placement
 * O(n1+n2)
 * @param - newTree, tree1, tree2
 * @return -
 *          void
 */

void combineStrTrees(StrTree& newTree, StrTree& tree1, StrTree& tree2) {
    int size1 = tree1.getTreeSize(), size2 = tree2.getTreeSize();
    StrNode<TwoKeysInt>* arrTree1 = new StrNode<TwoKeysInt>[size1];

    StrNode<TwoKeysInt>* arrTree2 = new StrNode<TwoKeysInt>[size2];

    inorderToArray(tree1.getRoot(), arrTree1, size1, 0);
    inorderToArray(tree2.getRoot(), arrTree2, size2, 0);

    StrNode<TwoKeysInt>* newArr = new StrNode<TwoKeysInt>[size1 + size2];

    int i, j, k;
    for (i = 0, j = 0, k = 0; i < size1 && j < size2 && k < size1 + size2;) {
        if (arrTree1[i].key <= arrTree2[j].key)
            newArr[k++] = arrTree1[i++];
        else if (arrTree1[i].key > arrTree2[j].key)
            newArr[k++] = arrTree2[j++];
    }
    if (i < size1) {
        for (; i < size1; i++, k++) newArr[k] = arrTree1[i];
    } else if (j < size2) {
        for (; j < size2; j++, k++) newArr[k] = arrTree2[j];
    }

    inorderToTree(newTree.getRoot(), newArr, size1 + size2, 0);

    delete[] arrTree1;
    delete[] arrTree2;
    delete[] newArr;
}

/*
 * Create new empty complete tree using Preorder traversal
 * @param height - the height of the complete tree
 * @return
 *          StrNode<T>*
 */

StrNode<TwoKeysInt>* StrTree::createEmptyTreeAux(
    StrNode<TwoKeysInt>* parentNode, int height, bool isLeftChild,
    bool isRightChild) {
    if (height < 0) return nullptr;
    StrNode<TwoKeysInt>* node = new StrNode<TwoKeysInt>();

    node->isLeftChild = isLeftChild;
    node->isRightChild = isRightChild;
    node->leftNode = createEmptyTreeAux(node, height - 1, true, false);
    node->rightNode = createEmptyTreeAux(node, height - 1, false, true);
    node->parentNode = parentNode;
    updateNodeParameters(node);
    return node;
}

/*
 * Remove the leafs from the complete tree, so it will be nearly complete,
 * using Inorder traversal
 * @param root - the current node that is being worked on
 * @param toDelete - the amount of nodes to remove
 * @return
 *          void
 */

void StrTree::adjustTreeSize(StrNode<TwoKeysInt>* node, int* toDelete) {
    if (node == nullptr || *toDelete <= 0) return;
    adjustTreeSize(node->rightNode, toDelete);
    if (node->rightNode == nullptr && node->leftNode == nullptr) {
        if (node->parentNode != nullptr && node->isRightChild == true) {
            node->parentNode->rightNode = nullptr;
            updateNodeParameters(node->parentNode);
        } else if (node->parentNode != nullptr && node->isLeftChild == true) {
            node->parentNode->leftNode = nullptr;
            updateNodeParameters(node->parentNode);
        }
        destroy(node);
        (*toDelete)--;
        treeSize--;
    } else
        adjustTreeSize(node->leftNode, toDelete);
}

StrNode<TwoKeysInt>* StrTree::getRoot() { return root; }
void StrTree::setRoot(StrNode<TwoKeysInt>* node) { root = node; }

void StrTree::Add(int endIndex, int value) {
    bool turnedRight = false;
    int sumExtras = 0;
    // Check that the tree contains a node with corresponding rank
    if (endIndex <= 0 || endIndex > treeSize) {
        return;
    }
    StrNode<TwoKeysInt>* target = root;
    int leftWeight;
    while (target != nullptr) {
        if (target->leftNode == nullptr) {
            leftWeight = 0;
        } else {
            leftWeight = target->leftNode->weight;
        }

        if (leftWeight == (endIndex - 1)) {
            if (!turnedRight) {
                target->extra += value;
            }
            if (target->rightNode != nullptr) {
                target->rightNode->extra -= value;
            }
            break;
        }
        // Search left
        else if (leftWeight > (endIndex - 1)) {
            if (turnedRight) {
                target->extra -= value;
                turnedRight = false;
            }
            target = target->leftNode;
        }
        // Search right
        else {
            if (!turnedRight) {
                target->extra += value;
                turnedRight = true;
            }
            endIndex -= (leftWeight + 1);
            target = target->rightNode;
        }
    }
}

void StrTree::Add(int startIndex, int endIndex, int value) {
    if (startIndex > endIndex || startIndex <= 0 || endIndex > treeSize) {
        return;
    }
    Add(endIndex, value);
    Add(startIndex - 1, -value);
}

#endif