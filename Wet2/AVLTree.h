#include <cmath>
#include <iostream>

#include "AVLNode.h"

#ifndef AVLTREE_H_
#define AVLTREE_H_

template <class dataType1, class dataType2>
class AVLTree {
   public:
    AVLNode<dataType1, dataType2>* root;
    int treeSize;

    /*
     *   AVLTree: Empty constructor
     */

    AVLTree() {
        root = nullptr;
        treeSize = 0;
    }

    /*
     * Constructor with arguments
     */

    AVLTree(AVLNode<dataType1, dataType2>* newRoot, int size) {
        root = newRoot;
        treeSize = size;
    }

    AVLTree(int height) {
        if (height >= 0) {
            root = new AVLNode<dataType1, dataType2>();

            root->leftNode =
                createEmptyTreeAux(root, height - 1, true, false);
            root->rightNode =
                createEmptyTreeAux(root, height - 1, false, true);
            root->parentNode = nullptr;
            updateNodeParameters(root);
            treeSize = std::pow(2, height + 1) - 1;
        } else {
            root = nullptr;
            treeSize = 0;
        }
    }

    AVLTree(AVLTree<dataType1, dataType2>& OtherAVLTree) {
        this->root = OtherAVLTree.root;
        this->treeSize = OtherAVLTree.treeSize;
    }

    /*
     *   ~AVLTree(): Destructor
     */

    ~AVLTree() {
        if (root != nullptr) {
            destroy(root);
        }
    }

    /*
     *   destroy: Removes each node using postorder search
     *
     *  @param node - Pointer to an AVLNode, which is a either a leaf or an
     * internal node
     */

    void destroy(AVLNode<dataType1, dataType2>* node) {
        if (node != nullptr) {
            destroy(node->leftNode);
            destroy(node->rightNode);
            delete node;
        }
    }

    void disconnectRoot() {
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
     *  AVLNode* - The closest node to the target
     *
     */

    AVLNode<dataType1, dataType2>* search(dataType1 key) {
        // Check if the tree is empty
        if (root == nullptr) return nullptr;

        AVLNode<dataType1, dataType2>* temp = root;
        AVLNode<dataType1, dataType2>* parent;
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

    /*
     *   findRank: Find the rank of the node with corresponding key
     *
     *  @param key - The unique key of the node
     *  @return - The rank of the node
     */
    int findRank(dataType1 key) {
        // Check if the tree is empty or the node doesn't exist
        if (!nodeExists(key)) {
            return 0;
        }

        int rank = 0;
        int leftWeight;
        AVLNode<dataType1, dataType2>* temp = root;
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

    AVLNode<dataType1, dataType2>* getNodeByRank(int rank) {
        // Check that the tree contains a node with corresponding rank
        if (rank <= 0 || rank > treeSize) {
            return nullptr;
        }
        AVLNode<dataType1, dataType2>* target = root;
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

    bool nodeExists(dataType1 key) {
        AVLNode<dataType1, dataType2>* node = search(key);
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

    bool isTreeEmpty() {
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

    bool insert(dataType1 key, dataType2 data) {
        if (root == nullptr) {
            root = new AVLNode<dataType1, dataType2>(key, data);

            // root = initNewNode(key, data);
            treeSize++;
            return true;
        }
        AVLNode<dataType1, dataType2>* target = search(key);

        // Check if the key already exists in the tree
        if (key == target->key) {
            return false;
        }

        // Insert to the left of target
        else if (key < target->key) {
            insertLeftNaive(target, key, data);
        }

        // Insert to the right of target
        else {
            insertRightNaive(target, key, data);
        }

        updateNodeParameters(target);
        // Traverse upwards and update parameters until you need to roll
        while (target != nullptr && target->balanceFactor != 2 &&
               target->balanceFactor != -2) {
            target = target->parentNode;
            updateNodeParameters(target);
        }

        // Roll accordingly
        if (target != nullptr &&
            (target->balanceFactor == 2 || target->balanceFactor == -2)) {
            rollNode(target);
        }
        treeSize++;

        // Search for the newly inserted node
        target = search(key);
        if (key != target->key) {
            return false;
        }

        // Go up the search route and update the parameters
        while (target != nullptr) {
            updateNodeParameters(target);
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

    bool remove(dataType1 key) {
        AVLNode<dataType1, dataType2>* toBeRemoved = search(key);
        if (toBeRemoved == nullptr)
            return false;
        else if (toBeRemoved->key != key) {
            return false;
        }

        // toBeRemoved has 2 children
        if (toBeRemoved->heightLeft >= 0 && toBeRemoved->heightRight >= 0) {
            AVLNode<dataType1, dataType2>* nextNode =
                findNextNode(toBeRemoved);
            swapTwoNodes(toBeRemoved, nextNode);
        }

        AVLNode<dataType1, dataType2>* parent = toBeRemoved->parentNode;
        // If toBeRemoved is a leaf
        if (toBeRemoved->height == 0) {
            if (!removeLeaf(toBeRemoved)) return false;
        }
        // toBeRemoved has a single child
        else if (toBeRemoved->height > 0 &&
                 (toBeRemoved->heightLeft == -1 ||
                  toBeRemoved->heightRight == -1)) {
            if (!removeSingleChild(toBeRemoved)) return false;
        }

        AVLNode<dataType1, dataType2>* tempNode = parent;
        updateNodeParameters(tempNode);
        while (tempNode != nullptr) {
            if (tempNode->balanceFactor == 2 ||
                tempNode->balanceFactor == -2) {
                if (!rollNode(tempNode)) return false;
            }
            tempNode = tempNode->parentNode;
            updateNodeParameters(tempNode);
        }
        treeSize--;

        // Go up the search route from parent to root
        while (parent != nullptr) {
            updateNodeParameters(parent);
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

    bool removeLeaf(AVLNode<dataType1, dataType2>* target) {
        AVLNode<dataType1, dataType2>* parent = target->parentNode;
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

    bool removeSingleChild(AVLNode<dataType1, dataType2>* target) {
        AVLNode<dataType1, dataType2>* parent = target->parentNode;

        // target has a single child on the right
        if (target->heightLeft == -1 && target->heightRight >= 0) {
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
     * 	AVLNode* - The next node in the tree
     */

    AVLNode<dataType1, dataType2>* findNextNode(
        AVLNode<dataType1, dataType2>* node) {
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

    void swapTwoNodes(AVLNode<dataType1, dataType2>* v1,
                      AVLNode<dataType1, dataType2>* v2) {
        AVLNode<dataType1, dataType2>* leftNodeV1 = v1->leftNode;
        AVLNode<dataType1, dataType2>* rightNodeV1 = v1->rightNode;
        AVLNode<dataType1, dataType2>* parentNodeV1 = v1->parentNode;

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

    void insertLeftNaive(AVLNode<dataType1, dataType2>* target, dataType1 key,
                         dataType2 data) {
        AVLNode<dataType1, dataType2>* newNode =
            new AVLNode<dataType1, dataType2>(key, data);

        // AVLNode<dataType1, dataType2> *newNode = initNewNode(key, data);
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

    void insertRightNaive(AVLNode<dataType1, dataType2>* target,
                          dataType1 key, dataType2 data) {
        AVLNode<dataType1, dataType2>* newNode =
            new AVLNode<dataType1, dataType2>(key, data);

        // AVLNode<dataType1, dataType2> *newNode = initNewNode(key, data);
        newNode->isRightChild = true;
        newNode->parentNode = target;
        target->rightNode = newNode;
    }

    void updateTreePostOrder(AVLNode<dataType1, dataType2>* node) {
        if (node == nullptr) {
            return;
        }

        updateTreePostOrder(node->leftNode);
        updateTreePostOrder(node->rightNode);
        updateNodeParameters(node);
    }

    /*
     *  updateNodeParameters: Update the parameters of the node,
     *                         Such as height, heightLeft, heightRight, BF and
     * so on.
     *
     * @param target - The node to update its parameters
     */

    void updateNodeParameters(AVLNode<dataType1, dataType2>* target) {
        // Check that the target is not empty
        if (target == nullptr) return;

        int target_weight = 1;

        // Update relevant parameters
        if (target->leftNode != nullptr) {
            target->heightLeft = target->leftNode->height;
            target_weight += target->leftNode->weight;
        } else {
            target->heightLeft = -1;
        }

        if (target->rightNode != nullptr) {
            target->heightRight = target->rightNode->height;
            target_weight += target->rightNode->weight;
        } else {
            target->heightRight = -1;
        }

        if (target->heightLeft > target->heightRight) {
            target->height = target->heightLeft + 1;
        } else {
            target->height = target->heightRight + 1;
        }

        target->balanceFactor = target->heightLeft - target->heightRight;
        target->weight = target_weight;
    }

    /*
     *  rollRight: Roll to the right (as seen in the recitation)
     *
     * @param node - The node to roll from
     */

    void rollRight(AVLNode<dataType1, dataType2>* node) {
        AVLNode<dataType1, dataType2>* temp = node->leftNode;
        AVLNode<dataType1, dataType2>* tempRight = temp->rightNode;
        node->leftNode = tempRight;

        updateNodeParameters(node);
        if (tempRight != nullptr) {
            tempRight->parentNode = node;
            tempRight->isLeftChild = true;
            tempRight->isRightChild = false;
        }
        temp->rightNode = node;
        updateNodeParameters(temp);

        AVLNode<dataType1, dataType2>* parent = node->parentNode;
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

    void rollLeft(AVLNode<dataType1, dataType2>* node) {
        AVLNode<dataType1, dataType2>* temp = node->rightNode;
        AVLNode<dataType1, dataType2>* tempLeft = temp->leftNode;
        node->rightNode = tempLeft;

        updateNodeParameters(node);
        if (tempLeft != nullptr) {
            tempLeft->parentNode = node;
            tempLeft->isLeftChild = false;
            tempLeft->isRightChild = true;
        }
        temp->leftNode = node;
        updateNodeParameters(temp);

        AVLNode<dataType1, dataType2>* parent = node->parentNode;
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

    bool rollNode(AVLNode<dataType1, dataType2>* node) {
        AVLNode<dataType1, dataType2>* tempNode;
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

    void printInOrder() { printInOrderAux(root); }

    /*
     *   getRightMostNode: Get the right most node
     *
     * @return
     *   nullptr - If the tree is empty
     *   dataType2* - A pointer to the right most node
     */

    AVLNode<dataType1, dataType2>* getRightMostNode() {
        AVLNode<dataType1, dataType2>* temp = root;
        if (temp == nullptr) return nullptr;
        while (temp->rightNode != nullptr) temp = temp->rightNode;
        // return new dataType2(temp->data);
        return temp;
    }

    /*
     *   getLeftMostNode: Get the left most node
     *
     * @return
     *   nullptr - If the tree is empty
     *   dataType2* - A pointer to the left most node
     */

    AVLNode<dataType1, dataType2>* getLeftMostNode() {
        AVLNode<dataType1, dataType2>* temp = root;
        if (temp == nullptr) return nullptr;
        while (temp->leftNode != nullptr) temp = temp->leftNode;
        // return new dataType2(temp->data);
        return temp;
    }

    /*
     *   printInOrderAux: Print inOrder - an auxiliary
     *
     * @param node - The node to print from
     */

    void printInOrderAux(AVLNode<dataType1, dataType2>* node) {
        if (node == nullptr) return;

        printInOrderAux(node->leftNode);
        std::cout << node->key << std::endl;
        printInOrderAux(node->rightNode);
    }

    int getTreeSize() { return treeSize; }

    /*
     * Running Inorder traversal and placing nodes into array
     * @param root - tree to be copied into array
     * @param arr - array to store the tree
     *  @return
     *          int
     */

    static int inorderToArray(AVLNode<dataType1, dataType2>* root,
                              AVLNode<dataType1, dataType2>* arr, int arrSize,
                              int i) {
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

    int inorderToTree(AVLNode<dataType1, dataType2>* node,
                      AVLNode<dataType1, dataType2>* arr, int arrSize,
                      int i) {
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

    void combineAVLTrees(AVLTree<dataType1, dataType2>& newTree,
                         AVLTree<dataType1, dataType2>& tree1,
                         AVLTree<dataType1, dataType2>& tree2) {
        int size1 = tree1.getTreeSize(), size2 = tree2.getTreeSize();
        AVLNode<dataType1, dataType2>* arrTree1 =
            new AVLNode<dataType1, dataType2>[size1];

        AVLNode<dataType1, dataType2>* arrTree2 =
            new AVLNode<dataType1, dataType2>[size2];

        inorderToArray(tree1.getRoot(), arrTree1, size1, 0);
        inorderToArray(tree2.getRoot(), arrTree2, size2, 0);

        AVLNode<dataType1, dataType2>* newArr =
            new AVLNode<dataType1, dataType2>[size1 + size2];

        int i, j, k;
        for (i = 0, j = 0, k = 0;
             i < size1 && j < size2 && k < size1 + size2;) {
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
     *          AVLNode<T>*
     */

    AVLNode<dataType1, dataType2>* createEmptyTreeAux(
        AVLNode<dataType1, dataType2>* parentNode, int height,
        bool isLeftChild, bool isRightChild) {
        if (height < 0) return nullptr;
        AVLNode<dataType1, dataType2>* node =
            new AVLNode<dataType1, dataType2>();

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

    void adjustTreeSize(AVLNode<dataType1, dataType2>* node, int* toDelete) {
        if (node == nullptr || *toDelete <= 0) return;
        adjustTreeSize(node->rightNode, toDelete);
        if (node->rightNode == nullptr && node->leftNode == nullptr) {
            if (node->parentNode != nullptr && node->isRightChild == true) {
                node->parentNode->rightNode = nullptr;
                updateNodeParameters(node->parentNode);
            } else if (node->parentNode != nullptr &&
                       node->isLeftChild == true) {
                node->parentNode->leftNode = nullptr;
                updateNodeParameters(node->parentNode);
            }
            destroy(node);
            (*toDelete)--;
            treeSize--;
        } else
            adjustTreeSize(node->leftNode, toDelete);
    }

    AVLNode<dataType1, dataType2>* getRoot() const { return root; }
    void setRoot(AVLNode<dataType1, dataType2>* node) { root = node; }
    void setTreeSize(int newTreeSize) { treeSize = newTreeSize; }
};

#endif