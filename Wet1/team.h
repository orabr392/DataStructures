#ifndef TEAM_H_
#define TEAM_H_

#include "AVLTree.h"
#include "Contestant.h"
#include "Country.h"
#include "TwoKeysInt.h"
#include "wet1util.h"

#define PARTITIONS 3
#define IDXRANGE 2
#define RIGHTMOSTNODE 1
#define LEFTMOSTNODE 0

class Team
{
private:
    int teamId;
    int countryId;
    Sport sport;
    int strength;
    int maxPossibleStrength;
    int currentCapacity;
    Country *country;
    int capacityInSubTree[PARTITIONS];
    AVLTree<int, Contestant> indicesTrees[PARTITIONS];
    AVLTree<TwoKeysInt, Contestant> strengthsTrees[PARTITIONS];
    void insertIntoPartition(Contestant contestant, int partition);
    void removeContestantFromPartition(Contestant contestant, int partition);
    Contestant removeFromPartition(int partition, bool leftMostEnd);
    void leftShift(int partition);
    void rightShift(int partition);
    void updateStrength();

public:
    Team();
    Team(int teamId, int countryId, Sport sport, Country *country);
    ~Team() = default;
    bool isTeamEmpty();
    Country *getCountry();
    int getTeamStrength();
    int getCountryId();
    bool insertContestantToTeam(Contestant contestant);
    bool removeContestantFromTeam(Contestant contestant);
    Sport getSport();
    AVLTree<int, Contestant> *getIndicesTrees();
    AVLTree<TwoKeysInt, Contestant> *getStrengthsTrees();
    int getCurrentCapacity();
    int getMaxStrength();
    // TEMPORARY FOR TESTING:

    void printCurrentContestants();
};

#endif