#ifndef HASHTABLEHashTableAVLNode_H_
#define HASHTABLEHashTableAVLNode_H_

#include "Team.h"

class Team;
class HashTableAVLNode {
   public:
    int key;
    Team* data;

    int height;
    int heightLeft;
    int heightRight;
    int balanceFactor;
    int weight;

    bool isLeftChild;
    bool isRightChild;

    HashTableAVLNode* leftNode;
    HashTableAVLNode* rightNode;
    HashTableAVLNode* parentNode;

    HashTableAVLNode() {
        // Init node data
        this->height = 0;
        this->heightLeft = -1;
        this->heightRight = -1;
        this->balanceFactor = 0;
        this->weight = 1;

        this->isLeftChild = false;
        this->isRightChild = false;

        // Set links to be empty
        this->leftNode = nullptr;
        this->rightNode = nullptr;
        this->parentNode = nullptr;
    }

    HashTableAVLNode(int key, Team* data) {
        // Init node data
        this->key = key;
        this->data = data;
        this->height = 0;
        this->heightLeft = -1;
        this->heightRight = -1;
        this->balanceFactor = 0;
        this->weight = 1;

        this->isLeftChild = false;
        this->isRightChild = false;

        // Set links to be empty
        this->leftNode = nullptr;
        this->rightNode = nullptr;
        this->parentNode = nullptr;
    }
};

#endif