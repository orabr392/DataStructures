#include "Contestant.h"
#include "Country.h"

/**
 *   Contestant: Empty Constructor
 */
Contestant::Contestant()
{
}

/**
 *   Contestant: A non empty Constructor
 *
 */
Contestant::Contestant(int contestantId, int countryId, Sport sport, int strength, Country *country)
{
    this->contestantId = contestantId;
    this->countryId = countryId;
    this->sport = sport;
    this->strength = strength;
    this->country = country;
    this->teamsCounter = 0;
    for (int i = 0; i < TEAMSCAP; i++)
    {
        this->teamsId[i] = 0;
    }
}

Contestant::Contestant(Contestant &contestant)
{
    this->contestantId = contestant.contestantId;
    this->countryId = contestant.countryId;
    this->sport = contestant.sport;
    this->strength = contestant.strength;
    this->country = contestant.country;
    this->teamsCounter = 0;
    for (int i = 0; i < TEAMSCAP; i++)
    {
        this->teamsId[i] = 0;
    }
}

/*
 *   joinTeam: Join the given teamId to the teamsId array
 */
bool Contestant::joinTeam(int teamId)
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

/*
 *   isContestantAvailable - Checks if the contestant can still join a new team
 */
bool Contestant::isContestantAvailable()
{
    return (!teamsId[0] || !teamsId[1] || !teamsId[2]);
}

/*
 *   isContestantOnTeam - Checks if the contestant is on the given teamId
 */
bool Contestant::isContestantOnTeam(int teamId)
{
    for (int i = 0; i < TEAMSCAP; i++)
    {
        if (teamId == teamsId[i])
            return true;
    }
    return false;
}

/*
 *   isContestantActive - Checks if the contestant is part of a team
 */
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

/*
 *   leaveTeam: Set the array[i] such that array[i] equals teamId be 0
 */
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

/*
 *   copyTeamsId: Copy the teamsId array to a given array
 */
void Contestant::copyTeamsId(int *copiedTeamsId)
{
    for (int i = 0; i < TEAMSCAP; i++)
    {
        copiedTeamsId[i] = teamsId[i];
    }
}

int *Contestant::getTeamsId()
{
    return teamsId;
}

/*
 *   changeContestantStrength: Increase the contestant strength by change (could be negative)
 */
void Contestant::changeContestantStrength(int change)
{
    strength += change;
}