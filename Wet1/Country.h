#ifndef COUNTRY_H_
#define COUNTRY_H_

#include "AVLTree.h"

class Country
{
private:
    int countryId;
    int medals;
    int teamsCounter;
    int contestantsCounter;

public:
    Country();
    Country(int countryId, int medals);
    ~Country() = default;
    void addMedal();
    bool isCountryEmpty();
    void changeTeamsCounter(int change);
    void changeContestantsCounter(int change);
    int getCountryId();
    int getMedals();
};

#endif