#ifndef TEAM_H_
#define TEAM_H_

#include "AVLTree.h"
#include "Contestant.h"
#include "wet1util.h"

class Team
{
private:
    int teamId;
    int countryId;
    Sport sport;
    int strength;
    int maxPossibleStrength;
    int countContestant;
    AVLTree<Contestant> indicesTrees[3];
    AVLTree<Contestant> strengthsTrees[3];

public:
    Team() = default;
    Team(int teamId, int countryId, Sport sport);
    ~Team() = default;
    bool isTeamEmpty();
    int getCountryId();
    int getStrength();
    Sport getSport();
    int getCountContestant();
    int getMaxStrength();
    AVLTree<Contestant> *getIndicesTrees();
    AVLTree<Contestant> *getStrengthsTrees();
};



#endif