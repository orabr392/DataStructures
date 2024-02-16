#ifndef CONTESTANT_H_
#define CONTESTANT_H_

#define TEAMSCAP 3

#include "wet1util.h"

class Contestant
{
private:
    int contestantId;
    int countryId;
    int strength;
    Sport sport;
    int teamsCounter;
    int teamsId[TEAMSCAP];

public:
    Contestant(int contestantId, int countryId, Sport sport, int strength);
    ~Contestant() = default;
    bool isContestantActive();
    int getCountryId();
};

Contestant::Contestant(int contestantId, int countryId, Sport sport, int strength) : contestantId(contestantId), countryId(countryId), sport(sport), strength(strength)
{
    teamsCounter = 0;
    for (int i = 0; i < TEAMSCAP; i++)
    {
        teamsId[i] = 0;
    }
}

bool Contestant::isContestantActive()
{
    if (teamsId[0] || teamsId[1] || teamsId[2])
        return true;
    return false;
}

int Contestant::getCountryId()
{
    return countryId;
}

#endif