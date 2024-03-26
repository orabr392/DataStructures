#ifndef TEAM_H_
#define TEAM_H_

#include "AVLNode.h"
#include "AVLTree.h"
#include "Stack.h"
#include "TwoKeysInt.h"

class TwoKeysInt;
class Team {
   private:
    int teamId;
    int playersCounter;
    int teamStr;
    int nextPlayerId;
    int medals;
    Stack<TwoKeysInt> playersStack;
    AVLTree<TwoKeysInt, int> playersTree;

    void calculateTeamStr();
    void combineStacks(Team* otherTeam);
    void mergeTeamsTrees(AVLTree<TwoKeysInt, int>* otherTree,
                         int playersCounterBefore);
    AVLNode<TwoKeysInt, int>* mergeTeamAVLTrees(
        AVLTree<TwoKeysInt, int>* tree1, AVLTree<TwoKeysInt, int>* tree2,
        int playersCounterBefore);
    AVLNode<TwoKeysInt, int>* merge(AVLNode<TwoKeysInt, int>* arrTree1,
                                    AVLNode<TwoKeysInt, int>* arrTree2,
                                    int size1, int size2);

   public:
    Team();
    Team(int teamId);
    ~Team() = default;
    int getTeamStr();
    void insertPlayer(int str);
    void removeNewestPlayer();
    bool isEmpty();
    void purchaseTeam(Team* otherTeam);
    void setMedals(int medals);
    int getMedals();
};

#endif