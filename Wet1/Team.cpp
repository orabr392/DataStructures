#include "Team.h"

Team::Team()
{
}

Team::Team(int teamId, int countryId, Sport sport, Country *country) : teamId(teamId), countryId(countryId), sport(sport), country(country)
{
    strength = maxPossibleStrength = currentCapacity = 0;
    for (int i = 0; i < PARTITIONS; i++)
    {
        capacityInSubTree[i] = 0;
    }
}

bool Team::isTeamEmpty()
{
    return (currentCapacity == 0);
}

Country *Team::getCountry()
{
    return country;
}

int Team::getCountryId()
{
    return countryId;
}

Sport Team::getSport()
{
    return sport;
}

void Team::insertIntoPartition(Contestant contestant, int partition)
{
    int contestantId = contestant.getContestantId();
    int contestantStrength = contestant.getContestantStrength();
    TwoKeysInt twoKeys(contestantStrength, contestantId);
    indicesTrees[partition].insert(contestantId, contestant);
    strengthsTrees[partition].insert(twoKeys, contestant);
    capacityInSubTree[partition]++;
}

Contestant Team::removeFromPartition(int partition, bool rightMostNode)
{
    Contestant *contestant;
    if (rightMostNode)
        contestant = indicesTrees[partition].getRightMostNode();
    else
        contestant = indicesTrees[partition].getLeftMostNode();
    TwoKeysInt twoKeys(contestant->getContestantStrength(), contestant->getContestantId());
    if (!strengthsTrees[partition].remove(twoKeys))
    {
        // Throw an exception
    }
    if (!indicesTrees[partition].remove(contestant->getContestantId()))
    {
        // Throw an exception
    }
    capacityInSubTree[partition]--;
    return *contestant;
}

bool Team::insertContestantToTeam(Contestant contestant)
{

    // Insert into bucket 0
    insertIntoPartition(contestant, 0);

    // Remove from bucket 0 if needed
    if (capacityInSubTree[0] > (currentCapacity / 3) + 1)
    {
        contestant = removeFromPartition(0, RIGHTMOSTNODE);

        // Insert into bucket 1
        insertIntoPartition(contestant, 1);

        // Remove from bucket 2 if needed
        if (capacityInSubTree[1] > (currentCapacity / 3) + 1)
        {
            contestant = removeFromPartition(1, RIGHTMOSTNODE);

            // Insert into bucket 2
            insertIntoPartition(contestant, 2);
        }
    }

    currentCapacity++;
    updateStrength();
    return true;
}

bool Team::removeContestantFromTeam(Contestant contestant)
{
    // Find the right bucket to remove contestant from
    int partition;
    for (int i = 0; i < PARTITIONS; i++)
    {
        if (indicesTrees->nodeExists(contestant.getContestantId()))
        {
            partition = i;
            break;
        }
    }

    removeContestantFromPartition(contestant, partition);

    if (currentCapacity % 3 == 0)
    {
        // Left shift twice
        if (partition == 0)
        {
            leftShift(1);
            leftShift(2);
        }

        // Left shift once
        else if (partition == 1)
        {
            leftShift(2);
        }

        // Dont shift
        else
        {
        }
    }
    else if (currentCapacity % 3 == 1)
    {
        // Left shift once
        if (partition == 0)
        {
            leftShift(1);
        }

        // Dont shift
        else if (partition == 1)
        {
        }

        // Right shift once
        else
        {
            rightShift(1);
        }
    }
    else
    {
        // Dont shift
        if (partition == 0)
        {
        }
        // Right shift once
        else if (partition == 1)
        {
            rightShift(0);
        }
        // Right shift twice
        else
        {
            rightShift(1);
            rightShift(0);
        }
    }
    currentCapacity--;
    updateStrength();
    return true;
}

void Team::leftShift(int partition)
{
    int nextPartition = partition - 1;
    Contestant contestant = removeFromPartition(partition, LEFTMOSTNODE);
    insertIntoPartition(contestant, nextPartition);
}

void Team::rightShift(int partition)
{
    int nextPartition = partition + 1;
    Contestant contestant = removeFromPartition(partition, RIGHTMOSTNODE);
    insertIntoPartition(contestant, nextPartition);
}

void Team::removeContestantFromPartition(Contestant contestant, int partition)
{
    TwoKeysInt twoKey(contestant.getContestantStrength(), contestant.getContestantId());
    indicesTrees[partition].remove(contestant.getContestantId());
    strengthsTrees[partition].remove(twoKey);
}

void Team::updateStrength()
{
    Contestant *contestant;
    if (currentCapacity % PARTITIONS != 0)
    {
        strength = 0;
    }
    else
    {
        int sum = 0;
        for (int i = 0; i < PARTITIONS; i++)
        {
            contestant = strengthsTrees[i].getRightMostNode();
            if (contestant != nullptr)
                sum += contestant->getContestantStrength();
            else
            {
                // Throw an exception
            }
        }
        strength = sum;
    }
}

int Team::getTeamStrength()
{
    return strength;
}

void Team::printCurrentContestants()
{
    std::cout << "\n First tree: \n";
    indicesTrees[0].printInOrder();
    std::cout << "\n Second tree: \n";
    indicesTrees[1].printInOrder();
    std::cout << "\n Third tree: \n";
    indicesTrees[2].printInOrder();
}
