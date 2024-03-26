#include "StrTree.h"

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

StrTree::StrTree(StrNode<TwoKeysIntStr>* newRoot, int size) {
    root = newRoot;
    treeSize = size;
}

StrTree::StrTree(int height) {
    if (height >= 0) {
        root = new StrNode<TwoKeysIntStr>();

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

void StrTree::destroy(StrNode<TwoKeysIntStr>* node) {
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

StrNode<TwoKeysIntStr>* StrTree::search(TwoKeysIntStr key) {
    // Check if the tree is empty
    if (root == nullptr) return nullptr;

    StrNode<TwoKeysIntStr>* temp = root;
    StrNode<TwoKeysIntStr>* parent;
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

StrNode<TwoKeysIntStr>* StrTree::search(TwoKeysIntStr key, int* sumExtras) {
    // Check if the tree is empty
    if (root == nullptr) return nullptr;

    StrNode<TwoKeysIntStr>* temp = root;
    StrNode<TwoKeysIntStr>* parent;
    while (temp != nullptr) {
        *sumExtras += temp->extra;
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

/*
 *   findIndex: Find the rank of the node with corresponding key
 *
 *  @param key - The unique key of the node
 *  @return - The rank of the node
 */
int StrTree::findIndex(TwoKeysIntStr key) {
    // Check if the tree is empty or the node doesn't exist
    if (!nodeExists(key)) {
        return 0;
    }

    int rank = 0;
    int leftWeight;
    StrNode<TwoKeysIntStr>* temp = root;
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

StrNode<TwoKeysIntStr>* StrTree::getNodeByRank(int rank, int* sumExtras) {
    // Check that the tree contains a node with corresponding rank
    if (rank <= 0 || rank > treeSize) {
        return nullptr;
    }
    StrNode<TwoKeysIntStr>* target = root;
    int leftWeight;
    while (target != nullptr) {
        *sumExtras += target->extra;
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

bool StrTree::nodeExists(TwoKeysIntStr key) {
    StrNode<TwoKeysIntStr>* node = search(key);
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

bool StrTree::insert(TwoKeysIntStr key, int data, int medals) {
    if (root == nullptr) {
        root = new StrNode<TwoKeysIntStr>(key, data);
        root->medals = medals;
        root->maxRank = data + medals;

        treeSize++;
        return true;
    }
    int sumExtras = 0;
    StrNode<TwoKeysIntStr>* target = search(key, &sumExtras);

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
        sumExtras -= target->extra;
        target = target->parentNode;
        updateNodeParameters(target, sumExtras);
    }

    // Roll accordingly
    if (target != nullptr &&
        (target->balanceFactor == 2 || target->balanceFactor == -2)) {
        rollNode(target, sumExtras);
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
        sumExtras -= target->extra;

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

bool StrTree::remove(TwoKeysIntStr key) {
    int sumExtras = 0;
    StrNode<TwoKeysIntStr>* toBeRemoved = search(key, &sumExtras);
    if (toBeRemoved == nullptr)
        return false;
    else if (toBeRemoved->key != key) {
        return false;
    }

    // toBeRemoved has 2 children
    if (toBeRemoved->heightLeft >= 0 && toBeRemoved->heightRight >= 0) {
        int sumExtrasV1 = sumExtras;
        StrNode<TwoKeysIntStr>* nextNode =
            findNextNode(toBeRemoved, &sumExtras);
        int sumExtrasV2 = sumExtras;
        swapTwoNodes(toBeRemoved, nextNode, sumExtrasV1, sumExtrasV2);
    }

    StrNode<TwoKeysIntStr>* parent = toBeRemoved->parentNode;
    // If toBeRemoved is a leaf
    if (toBeRemoved->height == 0) {
        if (!removeLeaf(toBeRemoved, &sumExtras)) return false;
    }
    // toBeRemoved has a single child
    else if (toBeRemoved->height > 0 && (toBeRemoved->heightLeft == -1 ||
                                         toBeRemoved->heightRight == -1)) {
        if (!removeSingleChild(toBeRemoved, &sumExtras)) return false;
    }

    StrNode<TwoKeysIntStr>* tempNode = parent;
    updateNodeParameters(tempNode, sumExtras);
    while (tempNode != nullptr) {
        if (tempNode->balanceFactor == 2 || tempNode->balanceFactor == -2) {
            if (!rollNode(tempNode, sumExtras)) return false;
        }
        sumExtras -= tempNode->extra;
        tempNode = tempNode->parentNode;
        updateNodeParameters(tempNode, sumExtras);
    }
    treeSize--;

    // Maybe need to update the whole route again?
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

bool StrTree::removeLeaf(StrNode<TwoKeysIntStr>* target, int* sumExtras) {
    StrNode<TwoKeysIntStr>* parent = target->parentNode;
    *sumExtras -= target->extra;
    // target is also the root
    if (parent == nullptr) {
        root = nullptr;

        destroy(target);

        return true;
    } else {
        if (target->isLeftChild) {
            parent->leftNode = nullptr;
            updateNodeParameters(parent, *sumExtras);

            destroy(target);
        } else if (target->isRightChild) {
            parent->rightNode = nullptr;
            updateNodeParameters(parent, *sumExtras);

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

bool StrTree::removeSingleChild(StrNode<TwoKeysIntStr>* target,
                                int* sumExtras) {
    StrNode<TwoKeysIntStr>* parent = target->parentNode;
    *sumExtras -= target->extra;
    int extra = target->extra;
    // target has a single child on the right
    if (target->heightLeft == -1 && target->heightRight >= 0) {
        target->rightNode->extra += extra;
        target->rightNode->maxRank = target->maxRank;
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
                updateNodeParameters(parent, *sumExtras);

                destroy(target);
            } else if (target->isRightChild) {
                parent->rightNode = target->rightNode;
                target->rightNode->parentNode = parent;
                target->rightNode = nullptr;
                updateNodeParameters(parent, *sumExtras);

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
        target->leftNode->maxRank = target->maxRank;
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
                updateNodeParameters(parent, *sumExtras);
                destroy(target);
            } else if (target->isRightChild) {
                parent->rightNode = target->leftNode;
                target->leftNode->parentNode = parent;
                target->leftNode = nullptr;
                updateNodeParameters(parent, *sumExtras);

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

StrNode<TwoKeysIntStr>* StrTree::findNextNode(StrNode<TwoKeysIntStr>* node,
                                              int* sumExtras) {
    node = node->rightNode;
    *sumExtras += node->extra;
    while (node->leftNode != nullptr) {
        node = node->leftNode;
        if (node->leftNode != nullptr) {
            *sumExtras += node->extra;
        }
    }
    return node;
}

/*
 *  swapTwoNodes: Swaps two nodes
 *
 * @param v1 - The first node
 * @param v2 - The second node
 */

void StrTree::swapTwoNodes(StrNode<TwoKeysIntStr>* v1,
                           StrNode<TwoKeysIntStr>* v2, int sumExtrasV1,
                           int sumExtrasV2) {
    StrNode<TwoKeysIntStr>* leftNodeV1 = v1->leftNode;
    StrNode<TwoKeysIntStr>* rightNodeV1 = v1->rightNode;
    StrNode<TwoKeysIntStr>* parentNodeV1 = v1->parentNode;

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
    v2->medals += (v2->extra - v1->extra);
    int tempExtra = v1->extra;
    v1->extra = v2->extra;
    v2->extra = tempExtra;
    int tempMaxRank = v1->maxRank;
    v1->maxRank = v2->maxRank;
    v2->maxRank = v1->maxRank;
    updateNodeParameters(v1, sumExtrasV2);
    updateNodeParameters(v2, sumExtrasV1);
}

/*
 *  insertLeftNaive: Insert a new node as a left child of target with
 * given key and data
 *
 * @param target - The node to connect the new node to from the left
 * @param key - The unique key to the new node
 * @param data - The data of the new node
 */

void StrTree::insertLeftNaive(StrNode<TwoKeysIntStr>* target,
                              TwoKeysIntStr key, int data, int sumExtras,
                              int medals) {
    StrNode<TwoKeysIntStr>* newNode = new StrNode<TwoKeysIntStr>(key, data);
    newNode->medals = medals - sumExtras;
    newNode->maxRank = data + medals;
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

void StrTree::insertRightNaive(StrNode<TwoKeysIntStr>* target,
                               TwoKeysIntStr key, int data, int sumExtras,
                               int medals) {
    StrNode<TwoKeysIntStr>* newNode = new StrNode<TwoKeysIntStr>(key, data);
    newNode->medals = medals - sumExtras;
    newNode->maxRank = data + medals;
    newNode->isRightChild = true;
    newNode->parentNode = target;
    target->rightNode = newNode;
}

/*
 *  Currently Deprecated
 *  updateNodeParameters: Update the parameters of the node,
 *                         Such as height, heightLeft, heightRight, BF and
 * so on.
 *
 * @param target - The node to update its parameters
 */

// void StrTree::updateNodeParameters(StrNode<TwoKeysIntStr>* target) {
//     // Check that the target is not empty
//     if (target == nullptr) return;

//     int targetWeight = 1;

//     int targetStrength = target->key.key1;

//     // Update relevant parameters
//     if (target->leftNode != nullptr) {
//         target->heightLeft = target->leftNode->height;
//         targetWeight += target->leftNode->weight;
//         if (target->maxRank < target->leftNode->maxRank) {
//             target->maxRank = target->leftNode->maxRank;
//         }
//     } else {
//         target->heightLeft = -1;
//     }

//     if (target->rightNode != nullptr) {
//         target->heightRight = target->rightNode->height;
//         targetWeight += target->rightNode->weight;
//         if (target->maxRank < target->rightNode->maxRank) {
//             target->maxRank = target->rightNode->maxRank;
//         }
//     } else {
//         target->heightRight = -1;
//     }

//     if (target->heightLeft > target->heightRight) {
//         target->height = target->heightLeft + 1;
//     } else {
//         target->height = target->heightRight + 1;
//     }

//     target->balanceFactor = target->heightLeft - target->heightRight;
//     target->weight = targetWeight;
// }

void StrTree::updateNodeParameters(StrNode<TwoKeysIntStr>* target,
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

void StrTree::rollRight(StrNode<TwoKeysIntStr>* node, int sumExtras) {
    StrNode<TwoKeysIntStr>* temp = node->leftNode;
    StrNode<TwoKeysIntStr>* tempRight = temp->rightNode;

    int tmpExtra = temp->extra;
    temp->extra += node->extra;
    node->extra = -tmpExtra;

    node->leftNode = tempRight;
    if (tempRight != nullptr) {
        tempRight->extra += tmpExtra;
        sumExtras += tempRight->extra;
        tempRight->parentNode = node;
        tempRight->isLeftChild = true;
        tempRight->isRightChild = false;
        updateNodeParameters(tempRight, sumExtras);
        sumExtras -= tempRight->extra;
    }
    updateNodeParameters(node, sumExtras);
    temp->rightNode = node;
    sumExtras -= node->extra;
    updateNodeParameters(temp, sumExtras);

    StrNode<TwoKeysIntStr>* parent = node->parentNode;
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

void StrTree::rollLeft(StrNode<TwoKeysIntStr>* node, int sumExtras) {
    StrNode<TwoKeysIntStr>* temp = node->rightNode;
    StrNode<TwoKeysIntStr>* tempLeft = temp->leftNode;

    int tmpExtra = temp->extra;
    temp->extra += node->extra;
    node->extra = -tmpExtra;

    node->rightNode = tempLeft;

    if (tempLeft != nullptr) {
        tempLeft->extra += tmpExtra;
        sumExtras += tempLeft->extra;
        tempLeft->parentNode = node;
        tempLeft->isLeftChild = false;
        tempLeft->isRightChild = true;
        updateNodeParameters(tempLeft, sumExtras);
        sumExtras -= tempLeft->extra;
    }
    updateNodeParameters(node, sumExtras);
    temp->leftNode = node;
    sumExtras -= node->extra;
    updateNodeParameters(temp, sumExtras);

    StrNode<TwoKeysIntStr>* parent = node->parentNode;
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

bool StrTree::rollNode(StrNode<TwoKeysIntStr>* node, int sumExtras) {
    StrNode<TwoKeysIntStr>* tempNode;
    if (node->balanceFactor == 2) {
        tempNode = node->leftNode;
        if (tempNode->balanceFactor >= 0) {
            // Roll LL
            rollRight(node, sumExtras);
        } else if (tempNode->balanceFactor == -1) {
            // Roll LR
            sumExtras += tempNode->extra;
            rollLeft(tempNode, sumExtras);
            sumExtras -= tempNode->extra;
            rollRight(node, sumExtras);
        } else {
            // Throw an exception
            return false;
        }
    } else if (node->balanceFactor == -2) {
        tempNode = node->rightNode;
        if (tempNode->balanceFactor <= 0) {
            // Roll RR
            rollLeft(node, sumExtras);
        } else if (tempNode->balanceFactor == 1) {
            // Roll RL
            sumExtras += tempNode->extra;
            rollRight(tempNode, sumExtras);
            sumExtras -= tempNode->extra;
            rollLeft(node, sumExtras);
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

StrNode<TwoKeysIntStr>* StrTree::getRightMostNode() {
    StrNode<TwoKeysIntStr>* temp = root;
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

StrNode<TwoKeysIntStr>* StrTree::getLeftMostNode() {
    StrNode<TwoKeysIntStr>* temp = root;
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

void StrTree::printInOrderAux(StrNode<TwoKeysIntStr>* node) {
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

static int inorderToArray(StrNode<TwoKeysIntStr>* root,
                          StrNode<TwoKeysIntStr>* arr, int arrSize, int i) {
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

int inorderToTree(StrNode<TwoKeysIntStr>* node, StrNode<TwoKeysIntStr>* arr,
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
    StrNode<TwoKeysIntStr>* arrTree1 = new StrNode<TwoKeysIntStr>[size1];

    StrNode<TwoKeysIntStr>* arrTree2 = new StrNode<TwoKeysIntStr>[size2];

    inorderToArray(tree1.getRoot(), arrTree1, size1, 0);
    inorderToArray(tree2.getRoot(), arrTree2, size2, 0);

    StrNode<TwoKeysIntStr>* newArr =
        new StrNode<TwoKeysIntStr>[size1 + size2];

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

StrNode<TwoKeysIntStr>* StrTree::createEmptyTreeAux(
    StrNode<TwoKeysIntStr>* parentNode, int height, bool isLeftChild,
    bool isRightChild) {
    if (height < 0) return nullptr;
    StrNode<TwoKeysIntStr>* node = new StrNode<TwoKeysIntStr>();

    node->isLeftChild = isLeftChild;
    node->isRightChild = isRightChild;
    node->leftNode = createEmptyTreeAux(node, height - 1, true, false);
    node->rightNode = createEmptyTreeAux(node, height - 1, false, true);
    node->parentNode = parentNode;
    updateNodeParameters(node, 0);
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

void StrTree::adjustTreeSize(StrNode<TwoKeysIntStr>* node, int* toDelete) {
    if (node == nullptr || *toDelete <= 0) return;
    adjustTreeSize(node->rightNode, toDelete);
    if (node->rightNode == nullptr && node->leftNode == nullptr) {
        if (node->parentNode != nullptr && node->isRightChild == true) {
            node->parentNode->rightNode = nullptr;
            updateNodeParameters(node->parentNode, 0);
        } else if (node->parentNode != nullptr && node->isLeftChild == true) {
            node->parentNode->leftNode = nullptr;
            updateNodeParameters(node->parentNode, 0);
        }
        destroy(node);
        (*toDelete)--;
        treeSize--;
    } else
        adjustTreeSize(node->leftNode, toDelete);
}

StrNode<TwoKeysIntStr>* StrTree::getRoot() { return root; }
void StrTree::setRoot(StrNode<TwoKeysIntStr>* node) { root = node; }

void StrTree::Add(int endIndex, int value) {
    bool turnedRight = false;
    int sumExtras = 0;
    // Check that the tree contains a node with corresponding rank
    if (endIndex <= 0 || endIndex > treeSize) {
        return;
    }
    StrNode<TwoKeysIntStr>* target = root;
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
    // updateEndIndex

    int sumExtras = 0;
    StrNode<TwoKeysIntStr>* finalNode = getNodeByRank(endIndex, &sumExtras);
    updateTournamentRound(finalNode, sumExtras);
    Add(startIndex - 1, -value);

    // updateStartIndex
    sumExtras = 0;
    StrNode<TwoKeysIntStr>* startNode = getNodeByRank(startIndex, &sumExtras);
    updateTournamentRound(startNode, sumExtras);
}

TwoKeysIntStr StrTree::closestLowPowerAbove(int lowPower) {
    // Check if the tree is empty
    if (root == nullptr) {
        return TwoKeysIntStr();
    };

    StrNode<TwoKeysIntStr>* temp = root;
    StrNode<TwoKeysIntStr>* lastGreater = nullptr;
    while (temp != nullptr) {
        if (lowPower == temp->key.key1) {
            if (temp->leftNode != nullptr &&
                temp->leftNode->key.key1 == lowPower) {
                temp = temp->leftNode;
            } else {
                return temp->key;
            }
        }
        // Traverse the correct direction
        else if (lowPower < temp->key.key1) {
            lastGreater = temp;
            temp = temp->leftNode;
        } else {
            temp = temp->rightNode;
        }
    }
    if (lastGreater != nullptr) {
        return lastGreater->key;
    }
    return TwoKeysIntStr();
}

TwoKeysIntStr StrTree::closestHighPowerBelow(int highPower) {
    // Check if the tree is empty
    if (root == nullptr) {
        return TwoKeysIntStr(0, 0);
    };

    StrNode<TwoKeysIntStr>* temp = root;
    StrNode<TwoKeysIntStr>* lastLess = nullptr;
    while (temp != nullptr) {
        if (highPower == temp->key.key1) {
            if (temp->rightNode != nullptr &&
                temp->rightNode->key.key1 == highPower) {
                temp = temp->rightNode;
            } else {
                return temp->key;
            }
        }
        // Traverse the correct direction
        else if (highPower < temp->key.key1) {
            temp = temp->leftNode;
        } else {
            lastLess = temp;
            temp = temp->rightNode;
        }
    }
    if (lastLess != nullptr) {
        return lastLess->key;
    }
    return TwoKeysIntStr(0, 0);
}

void StrTree::updateTournamentRound(StrNode<TwoKeysIntStr>* node,
                                    int sumExtras) {
    StrNode<TwoKeysIntStr>* tmpNode = node;
    while (tmpNode != nullptr) {
        int finalMaxRank = tmpNode->data + tmpNode->medals + sumExtras;
        // Compare Left
        if (tmpNode->leftNode != nullptr) {
            int leftMaxRank = tmpNode->leftNode->maxRank + sumExtras;
            if (tmpNode == node) {
                leftMaxRank += tmpNode->leftNode->extra;
            }
            if (leftMaxRank > finalMaxRank) {
                finalMaxRank = leftMaxRank;
            }
        }
        // Compare Right
        if (tmpNode->rightNode != nullptr) {
            int rightMaxRank = tmpNode->rightNode->maxRank + sumExtras;
            if (tmpNode == node) {
                rightMaxRank += tmpNode->rightNode->extra;
            }
            if (rightMaxRank > finalMaxRank) {
                finalMaxRank = rightMaxRank;
            }
        }
        tmpNode->maxRank = finalMaxRank;
        sumExtras -= tmpNode->extra;
        tmpNode = tmpNode->parentNode;
    }
}

int StrTree::getWins(TwoKeysIntStr teamKey) {
    int sumExtras = 0;
    StrNode<TwoKeysIntStr>* temp = root;
    StrNode<TwoKeysIntStr>* parent;
    while (temp != nullptr) {
        parent = temp;
        sumExtras += temp->extra;
        // Return the wins
        if (teamKey == temp->key) {
            return sumExtras + temp->medals;
        }
        // Traverse the correct direction
        else if (teamKey < temp->key) {
            temp = temp->leftNode;
        } else {
            temp = temp->rightNode;
        }
    }
    // Should not reach here
    return 0;
}

int StrTree::getMaxRank() {
    if (isTreeEmpty()) {
        return -1;
    }
    return root->maxRank;
}