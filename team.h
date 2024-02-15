#ifndef TEAM_H_
#define TEAM_H_

#include "TwoThreeTree.h"
#include "wet1util.h"

class team
{
private:
    int teamId;
    int countryId;
    Sport sport;
    int strength;
    int maxPossibleStrength;
    TwoThreeTree indices[3];
    TwoThreeTree strengths[3];

public:
    team(int teamId, int countryId, Sport sport);
    ~team() = default;
};

team::team(int teamId, int countryId, Sport sport) : teamId(teamId), countryId(countryId), sport(sport)
{
    strength = maxPossibleStrength = 0;
}

#endif