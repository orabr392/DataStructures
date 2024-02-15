#ifndef CONTESTANT_H_
#define CONTESTANT_H_

#define TEAMSCAP 3

#include "wet1util.h"

class contestant
{
private:
    int contestantId;
    int countryId;
    int strength;
    Sport sport;
    int teamsCounter;
    int teamsId[TEAMSCAP];

public:
    contestant(int contestantId, int countryId, int strength, Sport sport);
    ~contestant() = default;
};

contestant::contestant(int contestantId, int countryId, int strength, Sport sport) : contestantId(contestantId), countryId(countryId), strength(strength), sport(sport)
{
    teamsCounter = 0;
    for (int i = 0; i < TEAMSCAP; i++)
    {
        teamsId[i] = 0;
    }
}
#endif