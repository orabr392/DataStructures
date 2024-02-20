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
	if (contestant->insertTeam(teamId))
	{
		if (team->insertContestantToTeam(*contestant))
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
	if (!team->removeContestantFromTeam(*contestant) ||
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
AVLNode<dataType1,dataType2>* merge(AVLNode<dataType1,dataType2>* arrTree1, AVLNode<dataType1,dataType2>* arrTree2, int size1, int size2)
{
    AVLNode<dataType1,dataType2>* newArr = new AVLNode<dataType1,dataType2>[size1 + size2];
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
AVLNode<dataType1,dataType2>* mergeTeamAVLTrees(AVLTree<dataType1,dataType2>* tree1, AVLTree<dataType1,dataType2>* tree2,
                                                AVLTree<dataType1,dataType2>* tree3)
{
    int subTreeSize1 = tree1->getTreeSize();
    AVLNode<dataType1, dataType2>* subTreeConts1 = new AVLNode<dataType1, dataType2>[subTreeSize1];
    AVLTree<dataType1, dataType2>::inorderToArray(tree1->getRoot(), subTreeConts1, subTreeSize1, 0);

    int subTreeSize2 = tree2->getTreeSize();
    AVLNode<dataType1, dataType2>* subTreeConts2 = new AVLNode<dataType1, dataType2>[subTreeSize2];
    AVLTree<dataType1, dataType2>::inorderToArray(tree2->getRoot(), subTreeConts2, subTreeSize2, 0);

    int subTreeSize3 = tree3->getTreeSize();
    AVLNode<dataType1, dataType2>* subTreeConts3 = new AVLNode<dataType1, dataType2>[subTreeSize3];
    AVLTree<dataType1, dataType2>::inorderToArray(tree3->getRoot(), subTreeConts3, subTreeSize1, 0);

    AVLNode<dataType1, dataType2>* tempArr = merge(subTreeConts1,subTreeConts2,subTreeSize1,subTreeSize2);
    AVLNode<dataType1, dataType2>* newArr = merge(tempArr,subTreeConts3,subTreeSize1+subTreeSize2,subTreeSize3);
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
int countSizeNoReps(AVLNode<dataType1,dataType2>* arr, int size)
{
    if(size == 0)
        return 0;
    int counter = 1;
    for(int i = 0; i < size-1; i++)
    {
        if(arr[i].key != arr[i+1].key)
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
template <typename dataType1, typename dataType2>
void arrayToIndicesTrees(Team* team, AVLNode<dataType1, dataType2>* arr, int arrSize)
{
    if(arrSize % 3 == 0)
    {
        int treeSize = arrSize / 3;
        int height = std::max((int)(std::ceil(std::log(treeSize + 1)) - 1), 0);
        for(int i = 0; i < PARTITIONS; i++)
        {
            AVLNode<dataType1,dataType2>* newRoot = AVLTree<dataType1,dataType2>::createEmptyTree(height);
            int toDelete = (std::pow(2, height + 1) - 1 - treeSize);
            AVLTree<dataType1,dataType2>::adjustTreeSize(newRoot,&toDelete);
            AVLTree<dataType1,dataType2>::inorderToTree(newRoot,arr,treeSize,i*treeSize);
            team->setIndicesTrees(AVLTree<dataType1,dataType2>(newRoot, treeSize), i);
        }
    }
    else if(arrSize % 3 == 1)
    {
        int sizeLeftMost = std::ceil(arrSize / 3), sizeMidRight = sizeLeftMost - 1;
        int heightLeftMost = std::max((int)(std::ceil(std::log(sizeLeftMost + 1)) - 1), 0), heightMidRight = std::max((int)(std::ceil(std::log(sizeMidRight + 1)) - 1), 0);

        AVLNode<dataType1,dataType2>* newRootLeftMost = AVLTree<dataType1,dataType2>::createEmptyTree(heightLeftMost);
        int toDelete = (std::pow(2, heightLeftMost + 1) - 1 - sizeLeftMost);
        AVLTree<dataType1,dataType2>::adjustTreeSize(newRootLeftMost,&toDelete);
        AVLTree<dataType1,dataType2>::inorderToTree(newRootLeftMost,arr,sizeLeftMost,0);
        team->setIndicesTrees(AVLTree<dataType1,dataType2>(newRootLeftMost, sizeLeftMost), LEFT_MOST);

        for(int i = 1; i < PARTITIONS; i++)
        {
            AVLNode<dataType1,dataType2>* newRootMidRight = AVLTree<dataType1,dataType2>::createEmptyTree(heightMidRight);
            toDelete = (std::pow(2, heightMidRight + 1) - 1 - sizeMidRight);
            AVLTree<dataType1,dataType2>::adjustTreeSize(newRootMidRight,&toDelete);
            AVLTree<dataType1,dataType2>::inorderToTree(newRootMidRight,arr,sizeMidRight,sizeLeftMost+(i-1)*sizeMidRight);
            team->setIndicesTrees(AVLTree<dataType1,dataType2>(newRootMidRight, sizeMidRight), i);
        }
    }
    else if(arrSize % 3 == 2)
    {
        int sizeLeftMid = std::ceil(arrSize / 3), sizeRightMost = sizeLeftMid - 1;
        int heightLeftMid = std::max((int)(std::ceil(std::log(sizeLeftMid + 1)) - 1), 0), heightRightMost = std::max((int)(std::ceil(std::log(sizeRightMost + 1)) - 1), 0);
        int toDelete;

        for(int i = 0; i < PARTITIONS - 1; i++)
        {
            AVLNode<dataType1,dataType2>* newRootLeftMid = AVLTree<dataType1,dataType2>::createEmptyTree(heightLeftMid);
            toDelete = (std::pow(2, heightLeftMid + 1) - 1 - sizeLeftMid);
            AVLTree<dataType1,dataType2>::adjustTreeSize(newRootLeftMid, &toDelete);
            AVLTree<dataType1,dataType2>::inorderToTree(newRootLeftMid,arr,sizeLeftMid,i*sizeLeftMid);
            team->setIndicesTrees(AVLTree<dataType1,dataType2>(newRootLeftMid,sizeLeftMid),i);
        }

        AVLNode<dataType1,dataType2>* newRootRightMost = AVLTree<dataType1,dataType2>::createEmptyTree(heightRightMost);
        toDelete = (std::pow(2, heightRightMost + 1) - 1 - sizeRightMost);
        AVLTree<dataType1,dataType2>::adjustTreeSize(newRootRightMost,&toDelete);
        AVLTree<dataType1,dataType2>::inorderToTree(newRootRightMost,arr,sizeRightMost,2*sizeLeftMid);
        team->setIndicesTrees(AVLTree<dataType1,dataType2>(newRootRightMost,sizeLeftMid),RIGHT_MOST);
    }
}

/*
 * Copy the array of unique elements to the team's AVL strength trees
 * @param team - the target team
 * @param arr - the array to be copied
 * @return
 *          void
 */
template <typename dataType1, typename dataType2>
void arrayToStrengthTrees(Team* team, AVLNode<dataType1, dataType2>* arr, int arrSize)
{
    if(arrSize % 3 == 0)
    {
        int treeSize = arrSize / 3;
        int height = std::max((int)(std::ceil(std::log(treeSize + 1)) - 1), 0);
        for(int i = 0; i < PARTITIONS; i++)
        {
            AVLNode<dataType1,dataType2>* newRoot = AVLTree<dataType1,dataType2>::createEmptyTree(height);
            int toDelete = (std::pow(2, height + 1) - 1 - treeSize);
            AVLTree<dataType1,dataType2>::adjustTreeSize(newRoot,&toDelete);
            AVLTree<dataType1,dataType2>::inorderToTree(newRoot,arr,treeSize,i*treeSize);
            team->setStrengthTrees(AVLTree<dataType1,dataType2>(newRoot, treeSize), i);
        }
    }
    else if(arrSize % 3 == 1)
    {
        int sizeLeftMost = std::ceil(arrSize / 3), sizeMidRight = sizeLeftMost - 1;
        int heightLeftMost = std::max((int)(std::ceil(std::log(sizeLeftMost + 1)) - 1), 0), heightMidRight = std::max((int)(std::ceil(std::log(sizeMidRight + 1)) - 1), 0);

        AVLNode<dataType1,dataType2>* newRootLeftMost = AVLTree<dataType1,dataType2>::createEmptyTree(heightLeftMost);
        int toDelete = (std::pow(2, heightLeftMost + 1) - 1 - sizeLeftMost);
        AVLTree<dataType1,dataType2>::adjustTreeSize(newRootLeftMost,&toDelete);
        AVLTree<dataType1,dataType2>::inorderToTree(newRootLeftMost,arr,sizeLeftMost,0);
        team->setStrengthTrees(AVLTree<dataType1,dataType2>(newRootLeftMost, sizeLeftMost), LEFT_MOST);

        for(int i = 1; i < PARTITIONS; i++)
        {
            AVLNode<dataType1,dataType2>* newRootMidRight = AVLTree<dataType1,dataType2>::createEmptyTree(heightMidRight);
            toDelete = (std::pow(2, heightMidRight + 1) - 1 - sizeMidRight);
            AVLTree<dataType1,dataType2>::adjustTreeSize(newRootMidRight,&toDelete);
            AVLTree<dataType1,dataType2>::inorderToTree(newRootMidRight,arr,sizeMidRight,sizeLeftMost+(i-1)*sizeMidRight);
            team->setStrengthTrees(AVLTree<dataType1,dataType2>(newRootMidRight, sizeMidRight), i);
        }
    }
    else if(arrSize % 3 == 2)
    {
        int sizeLeftMid = std::ceil(arrSize / 3), sizeRightMost = sizeLeftMid - 1;
        int heightLeftMid = std::max((int)(std::ceil(std::log(sizeLeftMid + 1)) - 1), 0), heightRightMost = std::max((int)(std::ceil(std::log(sizeRightMost + 1)) - 1), 0);
        int toDelete;

        for(int i = 0; i < PARTITIONS - 1; i++)
        {
            AVLNode<dataType1,dataType2>* newRootLeftMid = AVLTree<dataType1,dataType2>::createEmptyTree(heightLeftMid);
            toDelete = (std::pow(2, heightLeftMid + 1) - 1 - sizeLeftMid);
            AVLTree<dataType1,dataType2>::adjustTreeSize(newRootLeftMid, &toDelete);
            AVLTree<dataType1,dataType2>::inorderToTree(newRootLeftMid,arr,sizeLeftMid,i*sizeLeftMid);
            team->setStrengthTrees(AVLTree<dataType1,dataType2>(newRootLeftMid,sizeLeftMid),i);
        }

        AVLNode<dataType1,dataType2>* newRootRightMost = AVLTree<dataType1,dataType2>::createEmptyTree(heightRightMost);
        toDelete = (std::pow(2, heightRightMost + 1) - 1 - sizeRightMost);
        AVLTree<dataType1,dataType2>::adjustTreeSize(newRootRightMost,&toDelete);
        AVLTree<dataType1,dataType2>::inorderToTree(newRootRightMost,arr,sizeRightMost,2*sizeLeftMid);
        team->setStrengthTrees(AVLTree<dataType1,dataType2>(newRootRightMost,sizeLeftMid),RIGHT_MOST);
    }
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
void removeRepetition(AVLNode<dataType1,dataType2>* arr1, AVLNode<dataType1,dataType2>* arr2 , int size1, int size2)
{
    if(size1 == 0 || size2 == 0)
        return;
    arr1[0] = arr2[0];
    int j = 0;
    for(int i = 1; i < size2 && j < size1; i++)
    {
        if(arr2[i].key != arr1[j].key)
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
//void removeAllContestantFromTeam(AVLNode<int ,Contestant>* contArr, int arrSize, int teamID)
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
	Team* team1 = &(teamsTree.search(teamId1)->data);
	Team* team2 = &(teamsTree.search(teamId2)->data);
	if ((*team1).getSport() != (*team2).getSport() || (*team1).getCountryId() != (*team2).getCountryId())
		return StatusType::FAILURE;


    AVLNode<int, Contestant>* arrContTeam1 = mergeTeamAVLTrees<int, Contestant>(&(team1->getIndicesTrees()[0]), &(team1->getIndicesTrees()[1]),
                                                                                &(team1->getIndicesTrees()[2]));
    AVLNode<int, Contestant>* arrContTeam2 = mergeTeamAVLTrees<int, Contestant>(&(team2->getIndicesTrees()[0]), &(team2->getIndicesTrees()[1]),
                                                                                &(team2->getIndicesTrees()[2]));
    AVLNode<int, Contestant>* arrCombineContRep = merge(arrContTeam1,arrContTeam2,team1->getCurrentCapacity(),team2->getCurrentCapacity());
    int noRepSize1 = countSizeNoReps(arrCombineContRep, team1->getCurrentCapacity()+team2->getCurrentCapacity());
    AVLNode<int ,Contestant>* arrCombineCont = new AVLNode<int, Contestant>[noRepSize1];
    removeRepetition(arrCombineCont, arrCombineContRep,noRepSize1,team1->getCurrentCapacity()+team2->getCurrentCapacity());
    arrayToIndicesTrees(team1,arrCombineCont, noRepSize1);


    AVLNode<TwoKeysInt, Contestant>* arrStrenTeam1 = mergeTeamAVLTrees<TwoKeysInt, Contestant>(&(team1->getStrengthsTrees()[0]),&(team1->getStrengthsTrees()[1]),
                                                                                               &(team1->getStrengthsTrees()[2]));
    AVLNode<TwoKeysInt, Contestant>* arrStrenTeam2 = mergeTeamAVLTrees<TwoKeysInt, Contestant>(&(team2->getStrengthsTrees()[0]),&(team2->getStrengthsTrees()[1]),
                                                                                               &(team2->getStrengthsTrees()[2]));
    AVLNode<TwoKeysInt, Contestant>* arrCombineStrenRep = merge(arrStrenTeam1, arrStrenTeam2,team1->getCurrentCapacity(),team2->getCurrentCapacity());
    int noRepSize2 = countSizeNoReps(arrCombineStrenRep, team1->getCurrentCapacity()+team2->getCurrentCapacity());
    AVLNode<TwoKeysInt, Contestant>* arrCombineStren = new AVLNode<TwoKeysInt, Contestant>[noRepSize2];
    removeRepetition(arrCombineStren,arrCombineStrenRep,noRepSize2,team1->getCurrentCapacity()+team2->getCurrentCapacity());
    arrayToStrengthTrees(team1,arrCombineStren,noRepSize2);

    for(int i = 0; i < noRepSize1; i++)
    {
        if(arrCombineCont[i].data.isContestantOnTeam(teamId2))
        {
            arrCombineCont[i].data.leaveTeam(teamId2);
            remove_contestant_from_team(teamId2,arrCombineCont[i].data.getContestantId());
        }
    }
    remove_team(teamId2);
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

Team* Olympics::get_team(int teamId)
{
	return &teamsTree.search(teamId)->data;
}
