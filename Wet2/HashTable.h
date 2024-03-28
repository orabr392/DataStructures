#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <memory>
#include <stdexcept>

#include "HashTableAVLNode.h"
#include "HashTableAVLTree.h"
#include "Team.h"

#define BASE_SIZE 10

using namespace std;
class Team;
class HashTable {
    int arrSize;
    int numOfTeams;
    HashTableAVLTree* arr;
    const double loadFactor = 0.5;
    int hashKey(int key, int mod) const;
    void rehash(int newSize);
    void insertTreeToTable(HashTableAVLNode* root, HashTableAVLTree* table,
                           int newSize);

   public:
    HashTable();
    ~HashTable();
    bool teamExists(int teamId);
    bool insert(int teamId);
    bool remove(int teamId);
    int getArrSize() const;
    int getNumOfTeams() const;
    HashTableAVLTree* getArr() const;
    Team* getTeam(int teamId);
    double getLoadFactor() const;
    /*
     * For debugging and other fun stuff
     */
    void printHash();
};

#endif