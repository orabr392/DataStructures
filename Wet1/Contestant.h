#ifndef CONTESTANT_H_
#define CONTESTANT_H_

#define TEAMSCAP 3

#include "wet1util.h"

// Forward declaration
class Country;
class Contestant
{
private:
    int contestantId;
    int countryId;
    int strength;
    Sport sport;
    int teamsCounter;
    int teamsId[TEAMSCAP];
    Country *country;

public:
    Contestant();
    Contestant(int contestantId, int countryId, Sport sport, int strength, Country *country);
    Contestant(Contestant &contestant);
    ~Contestant() = default;
    bool isContestantActive();
    bool insertTeam(int teamId);
    bool leaveTeam(int teamId);
    bool isContestantOnTeam(int teamId);
    bool isContestantAvailable();
    Country *getCountry();
    int getCountryId();
    int getContestantId();
    int getContestantStrength();
    int *getTeamsId();
    void changeContestantStrength(int change);
    void copyTeamsId(int *copiedTeamsId);
    Sport getSport();
};

#endif