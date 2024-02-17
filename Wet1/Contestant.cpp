#include "Contestant.h"

Contestant::Contestant()
{
}

Contestant::Contestant(int contestantId, int countryId, Sport sport, int strength, Country *country) : contestantId(contestantId), countryId(countryId), strength(strength), country(country)
{
    sport = sport;
    teamsCounter = 0;
    for (int i = 0; i < TEAMSCAP; i++)
    {
        teamsId[i] = 0;
    }
}

bool Contestant::insertTeam(int teamId)
{
    for (int i = 0; i < TEAMSCAP; i++)
    {
        if (teamsId[i] == 0)
        {
            teamsId[i] = teamId;
            return true;
        }
    }
    return false;
}

bool Contestant::isContestantAvailable()
{
    return (!teamsId[0] || !teamsId[1] || !teamsId[2]);
}

bool Contestant::isContestantOnTeam(int teamId)
{
    for (int i = 0; i < TEAMSCAP; i++)
    {
        if (teamId == teamsId[i])
            return true;
    }
    return false;
}

bool Contestant::isContestantActive()
{
    if (teamsId[0] || teamsId[1] || teamsId[2])
        return true;
    return false;
}

Country *Contestant::getCountry()
{
    return country;
}

int Contestant::getCountryId()
{
    return countryId;
}

Sport Contestant::getSport()
{
    return sport;
}

int Contestant::getContestantId()
{
    return contestantId;
}

int Contestant::getContestantStrength()
{
    return strength;
}

bool Contestant::doesContestantBelongToTeam(int teamId)
{
    for (int i = 0; i < TEAMSCAP; i++)
    {
        if (teamId == teamsId[i])
            return true;
    }
    return false;
}

bool Contestant::leaveTeam(int teamId)
{
    for (int i = 0; i < TEAMSCAP; i++)
    {
        if (teamId == teamsId[i])
        {
            teamsId[i] = 0;
            return true;
        }
    }
    return false;
}

int *Contestant::getTeamsId()
{

    int *teamsIdCopy = new int[3];
    for (int i = 0; i < TEAMSCAP; i++)
    {
        teamsIdCopy[i] = teamsId[i];
    }

    return teamsIdCopy;
}

void Contestant::changeContestantStrength(int change)
{
    strength += change;
}