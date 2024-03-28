#ifndef STACK_H_
#define STACK_H_

#include "StackNode.h"

template <class T>
class Stack {
   private:
    int counter;
    StackNode<T>* startNode;
    StackNode<T>* topNode;

   public:
    Stack();
    ~Stack();
    void push(T element);
    void pop();
    T& top();
    void joinStack(Stack<T>& otherStack);
    StackNode<T>* getStartNode();
    StackNode<T>* getTopNode();
};

template <class T>
Stack<T>::Stack() {
    this->counter = 0;
    this->startNode = nullptr;
    this->topNode = nullptr;
}

template <class T>
Stack<T>::~Stack() {
    StackNode<T>* tmpNode = topNode;
    for (int i = 0; i < this->counter - 1; i++) {
        tmpNode = tmpNode->prevNode;
        delete tmpNode->nextNode;
    }
    if (tmpNode != nullptr) {
        delete tmpNode;
    }
}

template <class T>
void Stack<T>::push(T element) {
    StackNode<T>* newNode = new StackNode<T>;
    newNode->data = element;
    newNode->prevNode = topNode;
    if (topNode != nullptr) {
        topNode->nextNode = newNode;
    }
    topNode = newNode;
    if (counter == 0) {
        startNode = newNode;
    }
    counter += 1;
}

template <class T>
void Stack<T>::pop() {
    if (counter < 1) {
        return;
    }
    StackNode<T>* tmpNode = nullptr;
    if (counter == 1) {
        startNode = nullptr;
    }
    if (counter > 1) {
        tmpNode = topNode->prevNode;
    }

    delete topNode;
    topNode = tmpNode;
    counter -= 1;
}

template <class T>
T& Stack<T>::top() {
    return topNode->data;
}

template <class T>
void Stack<T>::joinStack(Stack<T>& otherStack) {
    // Link this stack's last node with otherStack first node
    topNode->nextNode = otherStack.startNode;
    otherStack.startNode->prevNode = topNode;
    topNode = otherStack.topNode;
    counter += otherStack.counter;
}

template <class T>
StackNode<T>* Stack<T>::getStartNode() {
    return this->startNode;
}

template <class T>
StackNode<T>* Stack<T>::getTopNode() {
    return this->topNode;
}

#endif