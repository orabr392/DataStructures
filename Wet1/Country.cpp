#include "Country.h"

Country::Country()
{
}

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

int Country::getCountryId()
{
    return countryId;
}

int Country::getMedals()
{
    return medals;
}