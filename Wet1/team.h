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
    AVLTree<Contestant> indices[3];
    AVLTree<Contestant> strengths[3];

public:
    Team(int teamId, int countryId, Sport sport);
    ~Team() = default;
    bool isTeamEmpty();
    int getCountryId();
};

Team::Team(int teamId, int countryId, Sport sport) : teamId(teamId), countryId(countryId), sport(sport)
{
    strength = maxPossibleStrength = 0;
}

bool Team::isTeamEmpty()
{
    return (indices[0].isTreeEmpty() &&
            indices[1].isTreeEmpty() &&
            indices[2].isTreeEmpty());
}

int Team::getCountryId()
{
    return countryId;
}

#endif