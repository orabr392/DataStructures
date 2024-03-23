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
    Stack<TwoKeysInt> playersStack;
    AVLTree<TwoKeysInt, int> playersTree;

    void calculateTeamStr();

   public:
    Team();
    Team(int teamId);
    ~Team() = default;
    int getTeamStr();
    void insertPlayer(int str);
    void removeNewestPlayer();
};

Team::Team() {
    this->teamId = 0;
    this->teamStr = 0;
    this->playersCounter = 0;
    this->nextPlayerId = 1;
}

Team::Team(int teamId) {
    this->teamId = teamId;
    this->teamStr = 0;
    this->playersCounter = 0;
    this->nextPlayerId = 1;
}

int Team::getTeamStr() { return this->teamStr; }

void Team::calculateTeamStr() {
    if (this->playersCounter == 0) {
        this->teamStr = 0;
        return;
    }
    int median = (this->playersCounter / 2) + 1;
    AVLNode<TwoKeysInt, int>* player =
        this->playersTree.getNodeByRank(median);
    this->teamStr = player->data;
}

void Team::insertPlayer(int str) {
    TwoKeysInt playerKey(str, this->nextPlayerId);
    this->playersStack.push(playerKey);
    this->playersTree.insert(playerKey, str);

    this->nextPlayerId += 1;
    this->playersCounter += 1;

    this->calculateTeamStr();
}

void Team::removeNewestPlayer() {
    if (playersCounter == 0) {
        return;
    }

    TwoKeysInt playerKey = this->playersStack.top();
    this->playersTree.remove(playerKey);
    this->playersStack.pop();

    this->playersCounter -= 1;
    this->nextPlayerId -= 1;

    this->calculateTeamStr();
}

#endif