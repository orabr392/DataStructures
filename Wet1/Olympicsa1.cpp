#include "Olympicsa1.h"

#define ADDCONTESTANT 1
#define REMOVECONTESTANT -1
#define ADDTEAM 1
#define REMOVETEAM -1
#define LEFT_MOST 0
#define MID 1
#define RIGHT_MOST 2

Olympics::Olympics()
{
}

Olympics::~Olympics()
{
}

/*
 *	add_country: Adds a new country to the countriesTree with a given countryId and medals
 *				 If the countryId already exists returns accordingly
 */
StatusType Olympics::add_country(int countryId, int medals)
{
    if (countryId <= 0 || medals < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    else if (countriesTree.nodeExists(countryId))
        return StatusType::FAILURE;

    Country country(countryId, medals);
    if (countriesTree.insert(countryId, country))
        return StatusType::SUCCESS;
    return StatusType::ALLOCATION_ERROR;
}

/*
 *	remove_country: Removes a country from the countriesTree with a given countryId
 *				 If the countryId does not exist returns accordingly
 */
StatusType Olympics::remove_country(int countryId)
{
    if (countryId <= 0)
        return StatusType::INVALID_INPUT;

    else if (!countriesTree.nodeExists(countryId) || !countriesTree.search(countryId)->data.isCountryEmpty())
        return StatusType::FAILURE;

    else if (countriesTree.remove(countryId))
        return StatusType::SUCCESS;
    return StatusType::ALLOCATION_ERROR;
}

/*
 *	add_team: Adds a team to the teamsTree, links it with the given countryId
 *				 If the teamId already exists or countryId does not exist returns accordingly
 */
StatusType Olympics::add_team(int teamId, int countryId, Sport sport)
{
    if (countryId <= 0 || teamId <= 0)
        return StatusType::INVALID_INPUT;
    else if (teamsTree.nodeExists(teamId) || !countriesTree.nodeExists(countryId))
        return StatusType::FAILURE;
    Country *country = &countriesTree.search(countryId)->data;
    Team team(teamId, countryId, sport, country);
    if (!teamsTree.insert(teamId, team))
        return StatusType::ALLOCATION_ERROR;
    country->changeTeamsCounter(ADDTEAM);
    return StatusType::SUCCESS;
}

/*
 *	remove_team: Removes a team from the teamsTree
 *				 If the teamId does not exist returns accordingly
 */
StatusType Olympics::remove_team(int teamId)
{
    if (teamId <= 0)
        return StatusType::INVALID_INPUT;
    if (!teamsTree.nodeExists(teamId))
        return StatusType::FAILURE;
    Country *country = teamsTree.search(teamId)->data.getCountry();
    if (!teamsTree.remove(teamId))
        return StatusType::ALLOCATION_ERROR;
    country->changeTeamsCounter(REMOVETEAM);
    return StatusType::SUCCESS;
}

/*
 *	add_contestant: Adds a contestant to the contestantsTree, links it with the given countryId
 *				 If the contestantId already exists or countryId does not exist returns accordingly
 */
StatusType Olympics::add_contestant(int contestantId, int countryId, Sport sport, int strength)
{
    if (contestantId <= 0 || countryId <= 0 || strength < 0)
        return StatusType::INVALID_INPUT;
    if (contestantsTree.nodeExists(contestantId) || !countriesTree.nodeExists(countryId))
        return StatusType::FAILURE;
    Country *country = &countriesTree.search(countryId)->data;
    Contestant contestant(contestantId, countryId, sport, strength, country);
    if (!contestantsTree.insert(contestantId, contestant))
        return StatusType::ALLOCATION_ERROR;
    country->changeContestantsCounter(ADDCONTESTANT);
    return StatusType::SUCCESS;
}

/*
 *	remove_contestant: Remove a contestant from the contestantsTree,
 *				 If the contestantId does not exist or the contestant is active return accordingly
 */
StatusType Olympics::remove_contestant(int contestantId)
{
    if (contestantId <= 0)
        return StatusType::INVALID_INPUT;
    if (!contestantsTree.nodeExists(contestantId) || contestantsTree.search(contestantId)->data.isContestantActive())
        return StatusType::FAILURE;
    Country *country = contestantsTree.search(contestantId)->data.getCountry();
    if (!contestantsTree.remove(contestantId))
        return StatusType::ALLOCATION_ERROR;
    country->changeContestantsCounter(REMOVECONTESTANT);
    return StatusType::SUCCESS;
}

/*
 *	add_contestant_to_team: Adds a contestant to a team, links it with the given teamId
 *				 Handles problems that may arise accordingly
 */
StatusType Olympics::add_contestant_to_team(int teamId, int contestantId)
{
    if (contestantId <= 0 || teamId <= 0)
        return StatusType::INVALID_INPUT;
    if (!teamsTree.nodeExists(teamId) || !contestantsTree.nodeExists(contestantId))
        return StatusType::FAILURE;
    Team *team = &teamsTree.search(teamId)->data;
    Contestant *contestant = &contestantsTree.search(contestantId)->data;
    if (team->getCountryId() != contestant->getCountryId() ||
        team->getSport() != contestant->getSport() ||
        contestant->isContestantOnTeam(teamId) ||
        !contestant->isContestantAvailable())
        return StatusType::FAILURE;
    if (contestant->joinTeam(teamId))
    {
        if (team->insertContestantToTeam(contestant))
        {
            team->calcMaxPossibleStrength();
            return StatusType::SUCCESS;
        }
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::ALLOCATION_ERROR;
}

/*
 *	remove_contestant_from_team: Removes a contestant from the contestantsTree
 *				 Handles problems that may arise accordingly
 */
StatusType Olympics::remove_contestant_from_team(int teamId, int contestantId)
{
    if (contestantId <= 0 || teamId <= 0)
        return StatusType::INVALID_INPUT;
    if (!teamsTree.nodeExists(teamId) ||
        !contestantsTree.nodeExists(contestantId) ||
        !contestantsTree.search(contestantId)->data.isContestantOnTeam(teamId))
        return StatusType::FAILURE;
    Contestant *contestant = &contestantsTree.search(contestantId)->data;
    Team *team = &teamsTree.search(teamId)->data;
    if (!team->removeContestantFromTeam(contestant) ||
        !contestant->leaveTeam(teamId))
        return StatusType::ALLOCATION_ERROR;
    team->calcMaxPossibleStrength();
    return StatusType::SUCCESS;
}

/*
 *	update_contestant_strength: Increases the contestant strength, checks if it is valid
 *								And updates the teams he is a member of, of the changes
 */
StatusType Olympics::update_contestant_strength(int contestantId, int change)
{
    if (contestantId <= 0)
        return StatusType::INVALID_INPUT;
    if (!contestantsTree.nodeExists(contestantId) ||
        contestantsTree.search(contestantId)->data.getContestantStrength() + change < 0)
        return StatusType::FAILURE;

    int copiedTeamsId[3];
    contestantsTree.search(contestantId)->data.copyTeamsId(copiedTeamsId);
    for (int i = 0; i < PARTITIONS; i++)
    {
        remove_contestant_from_team(copiedTeamsId[i], contestantId);
    }

    contestantsTree.search(contestantId)->data.changeContestantStrength(change);

    for (int i = 0; i < PARTITIONS; i++)
    {
        add_contestant_to_team(copiedTeamsId[i], contestantId);
    }

    return StatusType::SUCCESS;
}

output_t<int> Olympics::get_strength(int contestantId)
{
    if (contestantId <= 0)
        return {StatusType::INVALID_INPUT};
    if (!contestantsTree.nodeExists(contestantId))
        return {StatusType::FAILURE};
    return {contestantsTree.search(contestantId)->data.getContestantStrength()};
}

output_t<int> Olympics::get_medals(int countryId)
{
    if (countryId <= 0)
        return {StatusType::INVALID_INPUT};
    if (!countriesTree.nodeExists(countryId))
        return {StatusType::FAILURE};
    return {countriesTree.search(countryId)->data.getMedals()};
}

output_t<int> Olympics::get_team_strength(int teamId)
{
    if (teamId <= 0)
        return {StatusType::INVALID_INPUT};
    if (!teamsTree.nodeExists(teamId))
        return {StatusType::FAILURE};
    return {teamsTree.search(teamId)->data.getTeamStrength()};
}

/**
 * Unite_teams stuff starts here
 */

/*
 * Merge arrTree1 and arrTree2 into newArr in O(n1+n2)
 * @param arrTree1 - array to merge
 * @param arrTree2 - array to merge
 * @param size - the size of the array
 * @return
 *          AVLNode<dataType1,dataType2>*
 */
template <typename dataType1, typename dataType2>
AVLNode<dataType1, dataType2> *merge(AVLNode<dataType1, dataType2> *arrTree1, AVLNode<dataType1, dataType2> *arrTree2, int size1, int size2)
{
    AVLNode<dataType1, dataType2> *newArr = new AVLNode<dataType1, dataType2>[size1 + size2];

    int i = 0, j = 0, k = 0;
    for (i = 0, j = 0, k = 0; i < size1 && j < size2 && k < size1 + size2;)
    {
        if (arrTree1[i].key <= arrTree2[j].key)
            newArr[k++] = arrTree1[i++];
        else if (arrTree1[i].key > arrTree2[j].key)
            newArr[k++] = arrTree2[j++];
    }
    if (i < size1)
    {
        for (; i < size1; i++, k++)
            newArr[k] = arrTree1[i];
    }
    else if (j < size2)
    {
        for (; j < size2; j++, k++)
            newArr[k] = arrTree2[j];
    }
    return newArr;
}

/*
 * Merge AVL trees into AVLNode array
 * @param tree1
 * @param tree2
 * @param tree3
 * @return
 *          AVLNode<dataType1,dataType2>*
 */
template <typename dataType1, typename dataType2>
AVLNode<dataType1, dataType2> *mergeTeamAVLTrees(AVLTree<dataType1, dataType2> *tree1, AVLTree<dataType1, dataType2> *tree2,
                                                 AVLTree<dataType1, dataType2> *tree3)
{
    int subTreeSize1 = tree1->getTreeSize();
    AVLNode<dataType1, dataType2> *subTreeConts1 = new AVLNode<dataType1, dataType2>[subTreeSize1];

    AVLTree<dataType1, dataType2>::inorderToArray(tree1->getRoot(), subTreeConts1, subTreeSize1, 0);

    int subTreeSize2 = tree2->getTreeSize();
    AVLNode<dataType1, dataType2> *subTreeConts2 = new AVLNode<dataType1, dataType2>[subTreeSize2];

    AVLTree<dataType1, dataType2>::inorderToArray(tree2->getRoot(), subTreeConts2, subTreeSize2, 0);

    int subTreeSize3 = tree3->getTreeSize();
    AVLNode<dataType1, dataType2> *subTreeConts3 = new AVLNode<dataType1, dataType2>[subTreeSize3];

    AVLTree<dataType1, dataType2>::inorderToArray(tree3->getRoot(), subTreeConts3, subTreeSize1, 0);

    AVLNode<dataType1, dataType2> *tempArr = merge(subTreeConts1, subTreeConts2, subTreeSize1, subTreeSize2);
    AVLNode<dataType1, dataType2> *newArr = merge(tempArr, subTreeConts3, subTreeSize1 + subTreeSize2, subTreeSize3);
    delete[] subTreeConts1;

    delete[] subTreeConts2;

    delete[] subTreeConts3;

    delete[] tempArr;

    return newArr;
}

/*
 * Count how many unique elements in arr
 * @param arr - the array we want to count its unique elements
 * @param size - the size of the array
 * @return
 *          int - the desired amount
 */
template <typename dataType1, typename dataType2>
int countSizeNoReps(AVLNode<dataType1, dataType2> *arr, int size)
{
    if (size == 0)
        return 0;
    int counter = 1;
    for (int i = 0; i < size - 1; i++)
    {
        if (arr[i].key != arr[i + 1].key)
            counter++;
    }
    return counter;
}

/*
 * Copy the array of unique elements to the team's AVL indices trees
 * @param team - the target team
 * @param arr - the array to be copied
 * @return
 *          void
 */
void arrayToIndicesTrees(Team *team, AVLNode<int, Contestant *> *arr, int arrSize)
{
    if (arrSize % 3 == 0)
    {
        int treeSize = arrSize / 3;
        int height = std::max((int)(std::ceil(std::log(treeSize + 1)) - 1), -1);
        for (int i = 0; i < PARTITIONS; i++)
        {
            AVLTree<int, Contestant *> newTree(height);
            int toDelete = (newTree.getTreeSize() - treeSize);
            newTree.adjustTreeSize(newTree.getRoot(), &toDelete);
            newTree.inorderToTree(newTree.getRoot(), arr, (i + 1) * treeSize, i * treeSize);
            team->setIndicesTrees(newTree.getRoot(), newTree.getTreeSize(), i);

            // So the destructor of the tree doesnt erase the new tree
            newTree.disconnectRoot();
        }
    }
    else if (arrSize % 3 == 1)
    {
        int sizeLeftMost = (arrSize / 3) + 1;
        int sizeMidRight = sizeLeftMost - 1;
        int heightLeftMost = std::max((int)(std::ceil(std::log(sizeLeftMost + 1)) - 1), -1);
        int heightMidRight = std::max((int)(std::ceil(std::log(sizeMidRight + 1)) - 1), -1);

        AVLTree<int, Contestant *> newLeftTree(heightLeftMost);
        int toDelete = (newLeftTree.getTreeSize() - sizeLeftMost);
        newLeftTree.adjustTreeSize(newLeftTree.getRoot(), &toDelete);
        newLeftTree.inorderToTree(newLeftTree.getRoot(), arr, sizeLeftMost, 0);
        team->setIndicesTrees(newLeftTree.getRoot(), newLeftTree.getTreeSize(), LEFT_MOST);

        // So the destructor of the tree doesnt erase the new tree
        newLeftTree.disconnectRoot();

        for (int i = 1; i < PARTITIONS; i++)
        {
            AVLTree<int, Contestant *> newMidRightTree(heightMidRight);
            toDelete = (newMidRightTree.getTreeSize() - sizeMidRight);
            newMidRightTree.adjustTreeSize(newMidRightTree.getRoot(), &toDelete);
            newMidRightTree.inorderToTree(newMidRightTree.getRoot(), arr, sizeMidRight, sizeLeftMost + (i - 1) * sizeMidRight);
            team->setIndicesTrees(newMidRightTree.getRoot(), newMidRightTree.getTreeSize(), i);

            // So the destructor of the tree doesnt erase the new tree
            newMidRightTree.disconnectRoot();
        }
    }
    else if (arrSize % 3 == 2)
    {
        int sizeLeftMid = (arrSize / 3) + 1;
        int sizeRightMost = sizeLeftMid - 1;
        int heightLeftMid = std::max((int)(std::ceil(std::log(sizeLeftMid + 1)) - 1), -1);
        int heightRightMost = std::max((int)(std::ceil(std::log(sizeRightMost + 1)) - 1), -1);
        int toDelete;

        for (int i = 0; i < PARTITIONS - 1; i++)
        {
            AVLTree<int, Contestant *> newLeftMidTree(heightLeftMid);
            toDelete = (newLeftMidTree.getTreeSize() - sizeLeftMid);
            newLeftMidTree.adjustTreeSize(newLeftMidTree.getRoot(), &toDelete);
            newLeftMidTree.inorderToTree(newLeftMidTree.getRoot(), arr, (i + 1) * sizeLeftMid, i * sizeLeftMid);
            team->setIndicesTrees(newLeftMidTree.getRoot(), newLeftMidTree.getTreeSize(), i);

            // So the destructor of the tree doesnt erase the new tree
            newLeftMidTree.disconnectRoot();
        }

        AVLTree<int, Contestant *> newLeftMidTree(heightRightMost);
        toDelete = (newLeftMidTree.getTreeSize() - sizeRightMost);
        newLeftMidTree.adjustTreeSize(newLeftMidTree.getRoot(), &toDelete);
        newLeftMidTree.inorderToTree(newLeftMidTree.getRoot(), arr, sizeRightMost, 2 * sizeLeftMid);
        team->setIndicesTrees(newLeftMidTree.getRoot(), newLeftMidTree.getTreeSize(), RIGHT_MOST);

        // So the destructor of the tree doesnt erase the new tree
        newLeftMidTree.disconnectRoot();
    }
}

void fillNodesArray(AVLNode<TwoKeysInt, Contestant *> **nodes, AVLNode<TwoKeysInt, Contestant *> *node, int *index)
{
    if (node == nullptr)
        return;

    fillNodesArray(nodes, node->leftNode, index);
    *(nodes + (*index)) = node;
    (*index)++;
    fillNodesArray(nodes, node->rightNode, index);
}

AVLNode<TwoKeysInt, Contestant *> **getNodesFromStrengthsTrees(Team *team)
{
    AVLNode<TwoKeysInt, Contestant *> **nodes = new AVLNode<TwoKeysInt, Contestant *> *[team->getCurrentCapacity()];

    int index = 0;
    for (int i = 0; i < PARTITIONS; i++)
    {
        fillNodesArray(nodes, team->getStrengthsTrees()[i].getRoot(), &index);
    }

    return nodes;
}
/*
 * Copy the array of unique elements to the team's AVL strength trees
 * @param team - the target team
 * @param arr - the array to be copied
 * @return
 *          void
 */

void arrayToStrengthTrees(Team *team, AVLNode<TwoKeysInt, Contestant *> *arr, int arrSize)
{

    // Create the new empty trees

    if (arrSize % 3 == 0)
    {
        int treeSize = arrSize / 3;
        int height = std::max((int)(std::ceil(std::log(treeSize + 1)) - 1), -1);
        for (int i = 0; i < PARTITIONS; i++)
        {
            AVLTree<TwoKeysInt, Contestant *> newTree(height);
            int toDelete = (newTree.getTreeSize() - treeSize);
            newTree.adjustTreeSize(newTree.getRoot(), &toDelete);
            team->setStrengthTrees(newTree.getRoot(), newTree.getTreeSize(), i);

            // So the destructor of the tree doesnt erase the new tree
            newTree.disconnectRoot();
        }
    }
    else if (arrSize % 3 == 1)
    {
        int sizeLeftMost = (arrSize / 3) + 1;
        int sizeMidRight = sizeLeftMost - 1;
        int heightLeftMost = std::max((int)(std::ceil(std::log(sizeLeftMost + 1)) - 1), -1);
        int heightMidRight = std::max((int)(std::ceil(std::log(sizeMidRight + 1)) - 1), -1);

        AVLTree<TwoKeysInt, Contestant *> newLeftTree(heightLeftMost);
        int toDelete = (newLeftTree.getTreeSize() - sizeLeftMost);
        newLeftTree.adjustTreeSize(newLeftTree.getRoot(), &toDelete);
        team->setStrengthTrees(newLeftTree.getRoot(), newLeftTree.getTreeSize(), LEFT_MOST);

        // So the destructor of the tree doesnt erase the new tree
        newLeftTree.disconnectRoot();

        for (int i = 1; i < PARTITIONS; i++)
        {
            AVLTree<TwoKeysInt, Contestant *> newMidRightTree(heightMidRight);
            toDelete = (newMidRightTree.getTreeSize() - sizeMidRight);
            newMidRightTree.adjustTreeSize(newMidRightTree.getRoot(), &toDelete);
            team->setStrengthTrees(newMidRightTree.getRoot(), newMidRightTree.getTreeSize(), -1);

            // So the destructor of the tree doesnt erase the new tree
            newMidRightTree.disconnectRoot();
        }
    }

    else if (arrSize % 3 == 2)
    {
        int sizeLeftMid = (arrSize / 3) + 1;
        int sizeRightMost = sizeLeftMid - 1;
        int heightLeftMid = std::max((int)(std::ceil(std::log(sizeLeftMid + 1)) - 1), -1);
        int heightRightMost = std::max((int)(std::ceil(std::log(sizeRightMost + 1)) - 1), -1);
        int toDelete;

        for (int i = 0; i < PARTITIONS - 1; i++)
        {
            AVLTree<TwoKeysInt, Contestant *> newLeftMidTree(heightLeftMid);
            toDelete = (newLeftMidTree.getTreeSize() - sizeLeftMid);
            newLeftMidTree.adjustTreeSize(newLeftMidTree.getRoot(), &toDelete);
            team->setStrengthTrees(newLeftMidTree.getRoot(), newLeftMidTree.getTreeSize(), i);

            // So the destructor of the tree doesnt erase the new tree
            newLeftMidTree.disconnectRoot();
        }

        AVLTree<TwoKeysInt, Contestant *> newLeftMidTree(heightRightMost);

        toDelete = (newLeftMidTree.getTreeSize() - sizeRightMost);
        newLeftMidTree.adjustTreeSize(newLeftMidTree.getRoot(), &toDelete);
        team->setStrengthTrees(newLeftMidTree.getRoot(), newLeftMidTree.getTreeSize(), RIGHT_MOST);

        // So the destructor of the tree doesnt erase the new tree
        newLeftMidTree.disconnectRoot();
    }

    // Supose we have an 3 arrays of pointers to the each node in each tree in inOrder - size of each array is arrSize/3 (maybe - 1)
    // 1. Foreach node in arr we check what tree he belongs to and set the node to be the current node
    // 2. Increase the appropriate array index by one
    // 3. If the we ran over the arr - finish
    // 4. Else go back to step 1

    AVLNode<TwoKeysInt, Contestant *> **nodes = getNodesFromStrengthsTrees(team);
    int idxs[3] = {0,
                   team->getCapacityInSubTree(0),
                   team->getCapacityInSubTree(0) + team->getCapacityInSubTree(1)};
    int partition;

    for (int i = 0; i < arrSize; i++)
    {
        partition = team->whichPartitionContestantBelongs(arr[i].data->getContestantId());
        (*(nodes + idxs[partition]))->key = arr[i].key;
        (*(nodes + idxs[partition]))->data = arr[i].data;
        idxs[partition]++;
    }

    delete[] nodes;
}

/*
 * Copy arr2 into arr1 without repetition
 * @param arr1 - target array
 * @param arr2 - array yo be copied
 * @param size1 - size of arr1
 * @param size2 - size of arr2
 * @return
 *          void
 */
template <typename dataType1, typename dataType2>
void removeRepetition(AVLNode<dataType1, dataType2> *arr1, AVLNode<dataType1, dataType2> *arr2, int size1, int size2)
{
    if (size1 == 0 || size2 == 0)
        return;
    arr1[0] = arr2[0];
    int j = 0;
    for (int i = 1; i < size2 && j < size1; i++)
    {
        if (arr2[i].key != arr1[j].key)
        {
            arr1[++j] = arr2[i];
        }
    }
}

///*
// * Going through the contestants array and remove contestant from the team
// * (also actually remove the team from contestant)
// * @param contArr - the array of the tree currently working on
// * @param teamID - the team to be removed
// * @return
// *          void
// */
// void removeAllContestantFromTeam(AVLNode<int ,Contestant>* contArr, int arrSize, int teamID)
//{
//    for(int i = 0; i < arrSize; i++)
//    {
//        if(contArr[i].data.isContestantOnTeam(teamID))
//        {
//            contArr[i].data.leaveTeam(teamID);
//        }
//    }
//}

StatusType Olympics::unite_teams(int teamId1, int teamId2)
{
    if (teamId1 == teamId2 || teamId1 <= 0 || teamId2 <= 0)
        return StatusType::INVALID_INPUT;
    if (!teamsTree.nodeExists(teamId1) || !teamsTree.nodeExists(teamId2))
        return StatusType::FAILURE;
    Team *team1 = &(teamsTree.search(teamId1)->data);
    Team *team2 = &(teamsTree.search(teamId2)->data);
    int team1_size = team1->getCurrentCapacity();
    int team2_size = team2->getCurrentCapacity();
    if ((*team1).getSport() != (*team2).getSport() || (*team1).getCountryId() != (*team2).getCountryId())
        return StatusType::FAILURE;

    AVLNode<int, Contestant *> *arrContTeam1 = mergeTeamAVLTrees<int, Contestant *>(&(team1->getIndicesTrees()[0]), &(team1->getIndicesTrees()[1]),
                                                                                    &(team1->getIndicesTrees()[2]));
    AVLNode<int, Contestant *> *arrContTeam2 = mergeTeamAVLTrees<int, Contestant *>(&(team2->getIndicesTrees()[0]), &(team2->getIndicesTrees()[1]),
                                                                                    &(team2->getIndicesTrees()[2]));
    AVLNode<int, Contestant *> *arrCombineContRep = merge(arrContTeam1, arrContTeam2, team1_size, team2_size);
    int noRepSize1 = countSizeNoReps(arrCombineContRep, team1_size + team2_size);
    AVLNode<int, Contestant *> *arrCombineCont = new AVLNode<int, Contestant *>[noRepSize1];
    removeRepetition(arrCombineCont, arrCombineContRep, noRepSize1, team1_size + team2_size);

    AVLNode<TwoKeysInt, Contestant *> *arrStrenTeam1 = mergeTeamAVLTrees<TwoKeysInt, Contestant *>(&(team1->getStrengthsTrees()[0]), &(team1->getStrengthsTrees()[1]),
                                                                                                   &(team1->getStrengthsTrees()[2]));
    AVLNode<TwoKeysInt, Contestant *> *arrStrenTeam2 = mergeTeamAVLTrees<TwoKeysInt, Contestant *>(&(team2->getStrengthsTrees()[0]), &(team2->getStrengthsTrees()[1]),
                                                                                                   &(team2->getStrengthsTrees()[2]));
    AVLNode<TwoKeysInt, Contestant *> *arrCombineStrenRep = merge(arrStrenTeam1, arrStrenTeam2, team1_size, team2_size);
    int noRepSize2 = countSizeNoReps(arrCombineStrenRep, team1_size + team2_size);
    AVLNode<TwoKeysInt, Contestant *> *arrCombineStren = new AVLNode<TwoKeysInt, Contestant *>[noRepSize2];
    removeRepetition(arrCombineStren, arrCombineStrenRep, noRepSize2, team1_size + team2_size);

    // EMPTY TREE 1 AND TREE 2
    for (int i = 0; i < noRepSize1; i++)
    {
        if (arrCombineCont[i].data->isContestantOnTeam(teamId1))
        {
            remove_contestant_from_team(teamId1, arrCombineCont[i].data->getContestantId());
            arrCombineCont[i].data->joinTeam(teamId1);
        }
        if (arrCombineCont[i].data->isContestantOnTeam(teamId2))
        {
            remove_contestant_from_team(teamId2, arrCombineCont[i].data->getContestantId());
            arrCombineCont[i].data->joinTeam(teamId1);
        }
    }

    arrayToIndicesTrees(team1, arrCombineCont, noRepSize1);
    team1->updateIndicesRanges();
    team1->setCurrentCapacity(noRepSize1);
    arrayToStrengthTrees(team1, arrCombineStren, noRepSize2);

    team1->updateStrength();
    team1->calcMaxPossibleStrength();

    remove_team(teamId2);
    delete[] arrContTeam1;

    delete[] arrContTeam2;

    delete[] arrCombineCont;

    delete[] arrCombineContRep;

    delete[] arrStrenTeam1;

    delete[] arrStrenTeam2;

    delete[] arrCombineStren;

    delete[] arrCombineStrenRep;

    return StatusType::SUCCESS;
}

/**
 * Unite_teams stuff ends here
 */

StatusType Olympics::play_match(int teamId1, int teamId2)
{
    if (teamId1 == teamId2 || teamId1 <= 0 || teamId2 <= 0)
        return StatusType::INVALID_INPUT;
    if (!teamsTree.nodeExists(teamId1) || !teamsTree.nodeExists(teamId2))
        return StatusType::FAILURE;
    Team *team1 = &teamsTree.search(teamId1)->data;
    Team *team2 = &teamsTree.search(teamId2)->data;
    if (team1->getSport() != team2->getSport())
        return StatusType::FAILURE;
    Country *country1 = &countriesTree.search(team1->getCountryId())->data;
    Country *country2 = &countriesTree.search(team2->getCountryId())->data;
    int score1 = country1->getMedals() + team1->getTeamStrength(), score2 = country2->getMedals() + team2->getTeamStrength();
    if (score1 > score2)
        country1->addMedal();
    else if (score1 < score2)
        country2->addMedal();
    return StatusType::SUCCESS;
}

output_t<int> Olympics::austerity_measures(int teamId)
{
    if (teamId <= 0)
        return output_t<int>(StatusType::INVALID_INPUT);
    if (!teamsTree.nodeExists(teamId))
        return output_t<int>(StatusType::FAILURE);
    Team *team = &teamsTree.search(teamId)->data;
    if (team->getCurrentCapacity() < 3)
    {
        return output_t<int>(StatusType::FAILURE);
    }
    return output_t<int>(team->getMaxStrength());
}

Team *Olympics::get_team(int teamId)
{
    return &teamsTree.search(teamId)->data;
}
