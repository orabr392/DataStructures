#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "AVLTree.h"
#include "Team.h"
#include <memory>
#include <stdexcept>

#define BASE_SIZE 10

using namespace std;

class HashTable{
    int arrSize;
    int numOfTeams;
    AVLTree<int, shared_ptr<Team>>* arr;
    const double loadFactor = 0.5;
    int hashKey(int key, int mod) const;
    void rehash(int newSize);
    void insertTreeToTable(AVLNode<int,shared_ptr<Team>>* root, AVLTree<int,shared_ptr<Team>>* table, int newSize);
public:
    HashTable();
    bool teamExists(int teamId);
    bool insert(int teamId);
    bool remove(int teamId);
    int getArrSize() const;
    int getNumOfTeams() const;
    AVLTree<int, shared_ptr<Team>> *getArr() const;
    double getLoadFactor() const;
    /*
     * For debugging and other fun stuff
     */
    void printHash();
};

HashTable::HashTable() : arrSize(BASE_SIZE), numOfTeams(0), arr(new AVLTree<int,shared_ptr<Team>>[BASE_SIZE]){
}

int HashTable::hashKey(int key, int mod) const {
    return key % mod;
}

bool HashTable::insert(int teamId) {
    if(teamExists(teamId)){
        return false;
    }
    try{
        if(((double)(numOfTeams+1) / (double)arrSize) >= loadFactor){
            rehash(2*arrSize);
        }
        int indexToInsert = hashKey(teamId, arrSize);
        arr[indexToInsert].insert(teamId,std::make_shared<Team>(Team(teamId)));
        numOfTeams++;
        return true;
    }catch (exception& e){

    }
    return false;
}

bool HashTable::remove(int teamId) {
    if(!teamExists(teamId)){
        return false;
    }
    if((double)(numOfTeams-1)/(double)arrSize <= 0.5*loadFactor && arrSize > BASE_SIZE){
        rehash((double)arrSize/2);
    }
    int indexToInsert = hashKey(teamId,arrSize);
    arr[indexToInsert].remove(teamId);
    numOfTeams--;
    return true;
}

void HashTable::insertTreeToTable(AVLNode<int,shared_ptr<Team>>* root, AVLTree<int,shared_ptr<Team>>* table, int newSize){
    if(root == nullptr)
        return;
    int indexToInsert = hashKey(root->key, newSize);
    table[indexToInsert].insert(root->key, root->data);
    insertTreeToTable(root->leftNode,table, newSize);
    insertTreeToTable(root->rightNode,table, newSize);
}

void HashTable::rehash(int newSize) {
    AVLTree<int,shared_ptr<Team>>* newArr = new AVLTree<int,shared_ptr<Team>>[newSize];
    AVLTree<int,shared_ptr<Team>>* temp = arr;
    for(int i = 0; i < arrSize; i++){
        insertTreeToTable(arr[i].root,newArr, newSize);
    }
    arr = newArr;
    arrSize = newSize;
    delete[] temp;
}



bool HashTable::teamExists(int teamId) {
    for(int i = 0; i < arrSize; i++){
        if(arr[i].nodeExists(teamId)){
            return true;
        }
    }
    return false;
}


int HashTable::getArrSize() const {
    return arrSize;
}

int HashTable::getNumOfTeams() const {
    return numOfTeams;
}

AVLTree<int, shared_ptr<Team>> *HashTable::getArr() const {
    return arr;
}

double HashTable::getLoadFactor() const {
    return loadFactor;
}


// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(AVLNode<int,shared_ptr<Team>>* root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += 10;

    // Process right child first
    print2DUtil(root->rightNode, space);

    // Print current node after space
    // count
    cout << endl;
    for (int i = 10; i < space; i++)
        cout << " ";
    cout << root->key << "\n";

    // Process left child
    print2DUtil(root->leftNode, space);
}

// Wrapper over print2DUtil()
void print2D(AVLNode<int,shared_ptr<Team>>* root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}

void HashTable::printHash() {
    cout << "Table size: " << arrSize << "\n";
    for(int i = 0; i < arrSize; i++){
       // arr[i].printInOrder();
       if(arr[i].treeSize > 0) {
           cout << "arr[" << i << "]:\n";
           print2D(arr[i].root);
           cout << "--------\n";
       }
    }
    cout << "-------------------------------------\n";
}

#endif