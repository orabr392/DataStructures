#ifndef AVLNODE_H_
#define AVLNODE_H_

template <class dataType1, class dataType2>
class AVLNode
{
public:
    dataType1 key;
    dataType2 data;
    int height;
    int heightLeft;
    int heightRight;
    int balanceFactor;

    bool isLeftChild;
    bool isRightChild;

    AVLNode *leftNode;
    AVLNode *rightNode;

    AVLNode *parentNode;

    //    AVLNode<dataType1,dataType2>& operator=(const AVLNode<dataType1,dataType2>& other);

    AVLNode()
    {
        // Init node data
        height = 0;
        heightLeft = -1;
        heightRight = -1;
        balanceFactor = 0;

        isLeftChild = isRightChild = false;

        // Set links to be empty
        leftNode = rightNode = parentNode = nullptr;
    }

    AVLNode(dataType1 key, dataType2 data)
    {
        // Init node data
        this->key = key;
        this->data = data;
        height = 0;
        heightLeft = -1;
        heightRight = -1;
        balanceFactor = 0;

        isLeftChild = isRightChild = false;

        // Set links to be empty
        leftNode = rightNode = parentNode = nullptr;
    }
};

#endif