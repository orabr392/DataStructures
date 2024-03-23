#ifndef STRNODE_H_
#define STRNODE_H_

template <class T>
class StrNode {
   public:
    T key;
    int data;

    int height;
    int heightLeft;
    int heightRight;
    int balanceFactor;

    // Ranked tree fields
    int weight;
    int maxRank;
    int medals;
    int extra;

    bool isLeftChild;
    bool isRightChild;

    StrNode* leftNode;
    StrNode* rightNode;
    StrNode* parentNode;

    StrNode() {
        // Init node data
        this->height = 0;
        this->heightLeft = -1;
        this->heightRight = -1;
        this->balanceFactor = 0;

        this->weight = 1;
        this->maxRank = 0;
        this->medals = 0;
        this->extra = 0;

        this->isLeftChild = false;
        this->isRightChild = false;

        // Set links to be empty
        this->leftNode = nullptr;
        this->rightNode = nullptr;
        this->parentNode = nullptr;
    }

    StrNode(T key, int data) {
        // Init node data
        this->key = key;
        this->data = data;
        this->height = 0;
        this->heightLeft = -1;
        this->heightRight = -1;
        this->balanceFactor = 0;

        this->weight = 1;
        this->maxRank = 0;
        this->medals = 0;
        this->extra = 0;

        this->isLeftChild = false;
        this->isRightChild = false;

        // Set links to be empty
        this->leftNode = nullptr;
        this->rightNode = nullptr;
        this->parentNode = nullptr;
    }
};

#endif