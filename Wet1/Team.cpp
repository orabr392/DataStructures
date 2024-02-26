#include "Team.h"
#include "Contestant.h"
#include "Country.h"
#include "TwoKeysInt.h"

/*
 *   Team: Empty Constructor
 */
Team::Team()
{
}

/*
 *   Team: A non empty Constructor, initializes variables
 */
Team::Team(int teamId, int countryId, Sport sport, Country *country) : teamId(teamId), countryId(countryId), sport(sport), country(country)
{
    strength = maxPossibleStrength = currentCapacity = 0;
    for (int i = 0; i < PARTITIONS; i++)
    {
        capacityInSubTree[i] = 0;
    }
    for (int i = 0; i < PARTITIONS; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            indicesRanges[i][j] = 0;
        }
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

/*
 *   insertIntoParition: Inserts a given contestant into a given partition
 */
void Team::insertIntoPartition(Contestant *contestant, int partition)
{
    int contestantId = contestant->getContestantId();
    int contestantStrength = contestant->getContestantStrength();
    TwoKeysInt twoKeys(contestantStrength, contestantId);
    indicesTrees[partition].insert(contestantId, contestant);
    strengthsTrees[partition].insert(twoKeys, contestant);
    capacityInSubTree[partition]++;
    currentCapacity++;
}

/*
 *   removeFromPartition: Remove either the left or right most contestant from a given partitions
 */
Contestant *Team::removeFromPartition(int partition, bool rightMostNode)
{
    AVLNode<int, Contestant *> *edgeNode;
    if (rightMostNode)
        edgeNode = indicesTrees[partition].getRightMostNode();
    else
        edgeNode = indicesTrees[partition].getLeftMostNode();
    if (edgeNode == nullptr)
    {
        return nullptr;
    }
    Contestant *contestant = edgeNode->data;
    int contestantId = edgeNode->data->getContestantId();
    int contestantStrength = edgeNode->data->getContestantStrength();
    TwoKeysInt twoKeys(contestantStrength, contestantId);
    if (!strengthsTrees[partition].remove(twoKeys))
    {
        // Throw an exception
    }
    if (!indicesTrees[partition].remove(contestantId))
    {
        // Throw an exception
    }
    capacityInSubTree[partition]--;
    currentCapacity--;
    return contestant;
}

/*
 *   insertContestantToTeam: A wrapper function that inserts the given contestant to the team's trees
 */
bool Team::insertContestantToTeam(Contestant *contestant)
{

    Contestant *c_ptr;

    // Insert into bucket 0
    insertIntoPartition(contestant, 0);

    // Remove from bucket 0 if needed
    if (capacityInSubTree[0] > ((currentCapacity - 1) / 3) + 1)
    {
        c_ptr = removeFromPartition(0, RIGHTMOSTNODE);

        // Insert into bucket 1
        insertIntoPartition(c_ptr, 1);

        // Remove from bucket 2 if needed
        if (capacityInSubTree[1] > ((currentCapacity - 1) / 3) + 1)
        {
            c_ptr = removeFromPartition(1, RIGHTMOSTNODE);

            // Insert into bucket 2
            insertIntoPartition(c_ptr, 2);
        }
    }

    // Check if needs to swap rightMostNode in bucket 0 and leftMostNode in bucket 1
    swapBetweenPartitions(0);
    swapBetweenPartitions(1);
    updateIndicesRanges();
    updateStrength();
    return true;
}

/*
 *   removeContestantFromTeam: Removes given contestant from the team
 */
bool Team::removeContestantFromTeam(Contestant *contestant)
{
    // Find the right bucket to remove contestant from
    int partition;
    for (int i = 0; i < PARTITIONS; i++)
    {
        if (indicesTrees[i].nodeExists(contestant->getContestantId()))
        {
            partition = i;
            break;
        }
    }

    removeContestantFromPartition(contestant, partition);
    if (currentCapacity % 3 == 0)
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
            rightShift(0);
            rightShift(1);
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
    updateIndicesRanges();
    updateStrength();
    return true;
}

/*
 *   leftShift: Removes the left most contestant from the given partition and inserts him to the previous one
 */
void Team::leftShift(int partition)
{
    Contestant *contestant;
    int nextPartition = partition - 1;
    contestant = removeFromPartition(partition, LEFTMOSTNODE);
    if (contestant == nullptr)
    {
        std::cout << "Left shift, Parition: " << partition << std::endl;
        return;
    }
    insertIntoPartition(contestant, nextPartition);
}

/*
 *   leftShift: Removes the right most contestant from the given partition and inserts him to the next one
 */
void Team::rightShift(int partition)
{
    Contestant *contestant;
    int nextPartition = partition + 1;
    contestant = removeFromPartition(partition, RIGHTMOSTNODE);
    if (contestant == nullptr)
    {
        std::cout << "Right shift, Parition: " << partition << std::endl;
        return;
    }
    insertIntoPartition(contestant, nextPartition);
}

void Team::swapBetweenPartitions(int partition)
{
    AVLNode<int, Contestant *> *edgeNodeRight = indicesTrees[partition].getRightMostNode();
    AVLNode<int, Contestant *> *edgeNodeLeft = indicesTrees[partition + 1].getLeftMostNode();
    if (edgeNodeRight == nullptr || edgeNodeLeft == nullptr)
        return;
    Contestant *contestant1 = edgeNodeRight->data;
    Contestant *contestant2 = edgeNodeLeft->data;
    if (contestant1->getContestantId() > contestant2->getContestantId())
    {
        removeContestantFromPartition(contestant1, partition);
        removeContestantFromPartition(contestant2, partition + 1);
        insertIntoPartition(contestant2, partition);
        insertIntoPartition(contestant1, partition + 1);

        if (contestant1 == nullptr || contestant2 == nullptr)
        {
            std::cout << "Hey" << std::endl;
        }
    }
}

/*
 *   removeContestantFromPartition: Removes a given contestant from a given partition
 */
void Team::removeContestantFromPartition(Contestant *contestant, int partition)
{
    TwoKeysInt twoKey(contestant->getContestantStrength(), contestant->getContestantId());
    indicesTrees[partition].remove(contestant->getContestantId());
    strengthsTrees[partition].remove(twoKey);
    capacityInSubTree[partition]--;
    currentCapacity--;
}

/*
 *   updateStrength: Updates the team's strength
 */
void Team::updateStrength()
{
    if (currentCapacity % PARTITIONS != 0)
    {
        strength = 0;
    }
    else
    {
        int sum = 0;
        for (int i = 0; i < PARTITIONS; i++)
        {
            AVLNode<TwoKeysInt, Contestant *> *edgeNodeRight = strengthsTrees[i].getRightMostNode();
            if (edgeNodeRight != nullptr)
            {
                sum += edgeNodeRight->data->getContestantStrength();
            }
            else
            {
                // Throw an exception
            }
        }
        strength = sum;
    }
}

void Team::updateIndicesRanges()
{
    for (int i = 0; i < PARTITIONS; i++)
    {
        AVLNode<int, Contestant *> *edgeNodeLeft = indicesTrees[i].getLeftMostNode();
        AVLNode<int, Contestant *> *edgeNodeRight = indicesTrees[i].getRightMostNode();
        if (edgeNodeLeft != nullptr)
        {
            indicesRanges[i][0] = edgeNodeLeft->data->getContestantId();
        }
        else
        {
            indicesRanges[i][0] = 0;
        }
        if (edgeNodeRight != nullptr)
        {
            indicesRanges[i][1] = edgeNodeRight->data->getContestantId();
        }
        else
        {
            indicesRanges[i][1] = 0;
        }
    }
}

int Team::whichPartitionContestantBelongs(int contestantId)
{
    for (int i = 0; i < PARTITIONS; i++)
    {
        if (contestantId >= indicesRanges[i][0] && contestantId <= indicesRanges[i][1])
            return i;
    }
    return -1;
}

int Team::getTeamStrength()
{
    return strength;
}

int Team::getCapacityInSubTree(int partition)
{
    return capacityInSubTree[partition];
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

void Team::printCurrentStrengths()
{
    std::cout << "\n First tree: \n";
    strengthsTrees[0].printInOrder();
    std::cout << "\n Second tree: \n";
    strengthsTrees[1].printInOrder();
    std::cout << "\n Third tree: \n";
    strengthsTrees[2].printInOrder();
}

AVLTree<int, Contestant *> *Team::getIndicesTrees()
{
    return indicesTrees;
}

AVLTree<TwoKeysInt, Contestant *> *Team::getStrengthsTrees()
{
    return strengthsTrees;
}

int Team::getCurrentCapacity()
{
    return currentCapacity;
}

int Team::getMaxStrength()
{
    return maxPossibleStrength;
}

/*
 *   calcMaxPossibleStrength: Calculate the max possible strength if we were to remove 3 contestants
 */
void Team::calcMaxPossibleStrength()
{
    if (currentCapacity <= 3 || currentCapacity % 3 != 0)
    {
        maxPossibleStrength = 0;
        return;
    }
    int maxValueReached = 0;
    for (int i = 0; i < PARTITIONS; i++)
    {

        AVLNode<TwoKeysInt, Contestant *> *edgeNodeLeft = strengthsTrees[i].getLeftMostNode();
        if (edgeNodeLeft == nullptr)
            continue;
        Contestant *minStrengthCont = edgeNodeLeft->data;
        removeContestantFromTeam(minStrengthCont);
        for (int j = i; j < PARTITIONS; j++)
        {
            edgeNodeLeft = strengthsTrees[j].getLeftMostNode();
            if (edgeNodeLeft == nullptr)
                continue;
            Contestant *secMinStrengthCont = edgeNodeLeft->data;
            removeContestantFromTeam(secMinStrengthCont);
            for (int k = j; k < PARTITIONS; k++)
            {
                edgeNodeLeft = strengthsTrees[k].getLeftMostNode();
                if (edgeNodeLeft == nullptr)
                    continue;
                Contestant *thirdMinStrengthCont = edgeNodeLeft->data;
                removeContestantFromTeam(thirdMinStrengthCont);
                updateStrength();
                if (strength > maxValueReached)
                    maxValueReached = strength;
                insertContestantToTeam(thirdMinStrengthCont);
            }
            insertContestantToTeam(secMinStrengthCont);
        }
        insertContestantToTeam(minStrengthCont);
    }
    updateStrength();
    maxPossibleStrength = maxValueReached;
}

void Team::setIndicesTrees(AVLNode<int, Contestant *> *root, int treeSize, int i)
{
    if (i < 0 || i >= PARTITIONS)
        return;
    indicesTrees[i].root = root;
    indicesTrees[i].treeSize = treeSize;
    capacityInSubTree[i] = treeSize;
}

void Team::setStrengthTrees(AVLNode<TwoKeysInt, Contestant *> *root, int treeSize, int i)
{
    if (i < 0 || i >= PARTITIONS)
        return;
    strengthsTrees[i].root = root;
    strengthsTrees[i].treeSize = treeSize;
}

void Team::setCurrentCapacity(int capacity)
{
    currentCapacity = capacity;
}

bool Team::isTeamEpmty()
{
    return (indicesTrees[0].getTreeSize() == 0) && (indicesTrees[1].getTreeSize() == 0) && (indicesTrees[2].getTreeSize() == 0);
}
