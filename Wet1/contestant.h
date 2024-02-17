#ifndef CONTESTANT_H_
#define CONTESTANT_H_

#define TEAMSCAP 3

#include "Country.h"
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
    Country *country;

public:
    Contestant();
    Contestant(int contestantId, int countryId, Sport sport, int strength, Country *country);
    ~Contestant() = default;
    bool isContestantActive();
    bool insertTeam(int teamId);
    bool leaveTeam(int teamId);
    bool isContestantOnTeam(int teamId);
    bool isContestantAvailable();
    bool doesContestantBelongToTeam(int teamId);
    Country *getCountry();
    int getCountryId();
    int getContestantId();
    int getContestantStrength();
    void changeContestantStrength(int change);
    int *getTeamsId();
    Sport getSport();
};

#endif