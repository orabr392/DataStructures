#ifndef STACKNODE_H_
#define STACKNODE_H_

template <class T>
class StackNode {
   private:
   public:
    StackNode* nextNode;
    StackNode* prevNode;
    T data;
    StackNode();
    StackNode(T data);
};

template <class T>
StackNode<T>::StackNode() {
    this->nextNode = nullptr;
    this->prevNode = nullptr;
}

template <class T>
StackNode<T>::StackNode(T data) {
    this->nextNode = nullptr;
    this->prevNode = nullptr;
    this->data = data;
}
#endif