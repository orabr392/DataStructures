#ifndef STACK_H_
#define STACK_H_

template <class T>
class Stack {
   private:
    struct Node {
        Node* nextNode = nullptr;
        Node* prevNode = nullptr;
        T data;
    };
    int counter;
    Node* topNode;

   public:
    Stack();
    ~Stack();
    void push(T element);
    void pop();
    T& top();
};

template <class T>
Stack<T>::Stack() {
    this->counter = 0;
    this->topNode = nullptr;
}

template <class T>
Stack<T>::~Stack() {
    Node* tmpNode;
    for (int i = 0; i < this->counter - 1; i++) {
        tmpNode = topNode->prevNode;
        delete tmpNode->nextNode;
    }
    delete tmpNode;
}

template <class T>
void Stack<T>::push(T element) {
    Node* newNode = new Node;
    newNode->data = element;
    newNode->prevNode = topNode;
    topNode = newNode;
    counter += 1;
}

template <class T>
void Stack<T>::pop() {
    Node* tmpNode = nullptr;
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

#endif