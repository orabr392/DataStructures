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
    Contestant() = default;
    Contestant(int contestantId, int countryId, Sport sport, int strength);
    ~Contestant() = default;
    bool isContestantActive();
    int getCountryId();
    int getStrength();
    int getContestantId();
    //friend std::ostream& operator<<(std::ostream& os, const Contestant& c);
};



//std::ostream &operator<<(std::ostream &os, const Contestant &c) {
//    os << c.getContestantId();
//    return os;
//}



#endif