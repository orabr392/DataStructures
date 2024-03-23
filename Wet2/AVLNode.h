#ifndef AVLNODE_H_
#define AVLNODE_H_

template <class dataType1, class dataType2>
class AVLNode {
   public:
    dataType1 key;
    dataType2 data;

    int height;
    int heightLeft;
    int heightRight;
    int balanceFactor;
    int weight;

    bool isLeftChild;
    bool isRightChild;

    AVLNode* leftNode;
    AVLNode* rightNode;
    AVLNode* parentNode;

    AVLNode() {
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

    AVLNode(dataType1 key, dataType2 data) {
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