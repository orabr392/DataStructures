#ifndef COUNTRY_H_
#define COUNTRY_H_

#include "AVLTree.h"
#include "Contestant.h"
#include "Team.h"

class Country
{
private:
    int countryId;
    int medals;
    int teamsCounter;
    int contestantsCounter;

public:
    Country(int countryId, int medals);
    ~Country() = default;
    void addMedal();
    bool isCountryEmpty();
    void changeTeamsCounter(int change);
    void changeContestantsCounter(int change);
};

Country::Country(int countryId, int medals) : countryId(countryId), medals(medals)
{
    teamsCounter = contestantsCounter = 0;
}

void Country::addMedal()
{
    medals++;
}

bool Country::isCountryEmpty()
{
    return (teamsCounter == 0 && contestantsCounter == 0);
}

void Country::changeTeamsCounter(int change)
{
    teamsCounter += change;
}

void Country::changeContestantsCounter(int change)
{
    contestantsCounter += change;
}

#endif