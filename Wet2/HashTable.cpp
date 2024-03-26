#include "HashTable.h"

HashTable::HashTable()
    : arrSize(BASE_SIZE),
      numOfTeams(0),
      arr(new AVLTree<int, Team*>[BASE_SIZE]) {}

int HashTable::hashKey(int key, int mod) const { return key % mod; }

bool HashTable::insert(int teamId) {
    if (teamExists(teamId)) {
        return false;
    }
    try {
        if (((double)(numOfTeams + 1) / (double)arrSize) >= loadFactor) {
            rehash(2 * arrSize);
        }
        int indexToInsert = hashKey(teamId, arrSize);
        Team* newTeam = new Team(teamId);
        arr[indexToInsert].insert(teamId, newTeam);
        numOfTeams++;
        return true;
    } catch (exception& e) {
    }
    return false;
}

bool HashTable::remove(int teamId) {
    if (!teamExists(teamId)) {
        return false;
    }
    if ((double)(numOfTeams - 1) / (double)arrSize <= 0.5 * loadFactor &&
        arrSize > BASE_SIZE) {
        rehash((double)arrSize / 2);
    }
    int indexToInsert = hashKey(teamId, arrSize);
    arr[indexToInsert].remove(teamId);
    numOfTeams--;
    return true;
}

void HashTable::insertTreeToTable(AVLNode<int, Team*>* root,
                                  AVLTree<int, Team*>* table, int newSize) {
    if (root == nullptr) return;
    int indexToInsert = hashKey(root->key, newSize);
    table[indexToInsert].insert(root->key, root->data);
    insertTreeToTable(root->leftNode, table, newSize);
    insertTreeToTable(root->rightNode, table, newSize);
}

void HashTable::rehash(int newSize) {
    AVLTree<int, Team*>* newArr = new AVLTree<int, Team*>[newSize];
    AVLTree<int, Team*>* temp = arr;
    for (int i = 0; i < arrSize; i++) {
        insertTreeToTable(arr[i].root, newArr, newSize);
    }
    arr = newArr;
    arrSize = newSize;
    delete[] temp;
}

bool HashTable::teamExists(int teamId) {
    int index = hashKey(teamId,arrSize);
    if(arr[index].nodeExists(teamId)){
        return true;
    }
//    for (int i = 0; i < arrSize; i++) {
//        if (arr[i].nodeExists(teamId)) {
//            return true;
//        }
//    }
    return false;
}

Team* HashTable::getTeam(int teamId) {
    int index = hashKey(teamId, arrSize);
    if(arr[index].nodeExists(teamId)){
        return arr[index].search(teamId)->data;
    }
//    if (teamExists(teamId)) {
//        for (int i = 0; i < arrSize; i++) {
//            if (arr[i].nodeExists(teamId)) {
//                return arr[i].search(teamId)->data;
//            }
//        }
//    }
    return nullptr;
}

int HashTable::getArrSize() const { return arrSize; }

int HashTable::getNumOfTeams() const { return numOfTeams; }

AVLTree<int, Team*>* HashTable::getArr() const { return arr; }

double HashTable::getLoadFactor() const { return loadFactor; }

// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(AVLNode<int, Team*>* root, int space) {
    // Base case
    if (root == NULL) return;

    // Increase distance between levels
    space += 10;

    // Process right child first
    print2DUtil(root->rightNode, space);

    // Print current node after space
    // count
    cout << endl;
    for (int i = 10; i < space; i++) cout << " ";
    cout << root->key << "\n";

    // Process left child
    print2DUtil(root->leftNode, space);
}

// Wrapper over print2DUtil()
void print2D(AVLNode<int, Team*>* root) {
    // Pass initial space count as 0
    print2DUtil(root, 0);
}

void HashTable::printHash() {
    cout << "Table size: " << arrSize << "\n";
    for (int i = 0; i < arrSize; i++) {
        // arr[i].printInOrder();
        if (arr[i].treeSize > 0) {
            cout << "arr[" << i << "]:\n";
            print2D(arr[i].root);
            cout << "--------\n";
        }
    }
    cout << "-------------------------------------\n";
}