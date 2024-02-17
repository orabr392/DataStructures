#include "Olympicsa1.h"

#define ADDCONTESTANT 1
#define REMOVECONTESTANT -1
#define ADDTEAM 1
#define REMOVETEAM -1

Olympics::Olympics()
{
}

Olympics::~Olympics()
{
}

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
			return StatusType::SUCCESS;
		return StatusType::ALLOCATION_ERROR;
	}
	return StatusType::ALLOCATION_ERROR;
}

StatusType Olympics::remove_contestant_from_team(int teamId, int contestantId)
{
	if (contestantId <= 0 || teamId <= 0)
		return StatusType::INVALID_INPUT;
	if (!teamsTree.nodeExists(teamId) ||
		!contestantsTree.nodeExists(contestantId) ||
		!contestantsTree.search(contestantId)->data.doesContestantBelongToTeam(teamId))
		return StatusType::FAILURE;
	Contestant *contestant = &contestantsTree.search(contestantId)->data;
	if (!teamsTree.search(teamId)->data.removeContestantFromTeam(*contestant) ||
		!contestant->leaveTeam(teamId))
		return StatusType::ALLOCATION_ERROR;
	return StatusType::SUCCESS;
}

StatusType Olympics::update_contestant_strength(int contestantId, int change)
{
	if (contestantId <= 0)
		return StatusType::INVALID_INPUT;
	if (!contestantsTree.nodeExists(contestantId) ||
		contestantsTree.search(contestantId)->data.getContestantStrength() + change < 0)
		return StatusType::FAILURE;

	int *teamsIdCopy = contestantsTree.search(contestantId)->data.getTeamsId();
	for (int i = 0; i < PARTITIONS; i++)
	{
		remove_contestant_from_team(*(teamsIdCopy + i), contestantId);
	}

	contestantsTree.search(contestantId)->data.changeContestantStrength(change);

	for (int i = 0; i < PARTITIONS; i++)
	{
		add_contestant_to_team(*(teamsIdCopy + i), contestantId);
	}

	// Free the copied array
	delete teamsIdCopy;
	return StatusType::SUCCESS;
}

output_t<int> Olympics::get_strength(int contestantId)
{
<<<<<<< HEAD
    if(contestantId <= 0)
        return {StatusType::INVALID_INPUT};
    if(!contestantsTree.nodeExists(contestantId))
        return {StatusType::FAILURE};
	return {contestantsTree.search(contestantId)->data.getStrength()};
=======
	if (contestantId <= 0)
		return output_t<int>(StatusType::INVALID_INPUT);
	else if (!contestantsTree.nodeExists(contestantId))
		return output_t<int>(StatusType::FAILURE);
	else
	{
		int strength = contestantsTree.search(contestantId)->data.getContestantStrength();
		return output_t<int>(strength);
	}
>>>>>>> 34f4e39f2f88b6de94a18d5b2569fbdbcf17e846
}

output_t<int> Olympics::get_medals(int countryId)
{
<<<<<<< HEAD
	if(countryId <= 0)
		return {StatusType::INVALID_INPUT};
	if(!countriesTree.nodeExists(countryId))
		return {StatusType::FAILURE};
	return {countriesTree.search(countryId)->data.getMedals()};
=======
	if (countryId <= 0)
		return output_t<int>(StatusType::INVALID_INPUT);
	else if (!countriesTree.nodeExists(countryId))
		return output_t<int>(StatusType::FAILURE);
	else
	{
		int medals = countriesTree.search(countryId)->data.getMedals();
		return output_t<int>(medals);
	}
	return 0;
>>>>>>> 34f4e39f2f88b6de94a18d5b2569fbdbcf17e846
}

output_t<int> Olympics::get_team_strength(int teamId)
{
<<<<<<< HEAD
	if(teamId <= 0)
		return {StatusType::INVALID_INPUT};
	if(!teamsTree.nodeExists(teamId))
		return {StatusType::FAILURE};
	return {teamsTree.search(teamId)->data.getStrength()};
}


/*
 * Running Inorder traversal and placing nodes into array
 * @param root - tree to be copied into array
 * @param arr - array to store the tree
 *  @return
 *          int
 */
template <typename T>
int inorderToArray(AVLNode<T>* root, AVLNode<T>* arr, int arrSize, int i){
    if(root == nullptr)
        return i;

    i = inorderToArray(root->leftNode,arr,arrSize,i);
    if(i > arrSize-1)
        return i;
    arr[i++] = *root;
    return inorderToArray(root->rightNode,arr, arrSize, i);
}

/*
 * Running inorder traversal and placing arrays' items into the nearly-complete empty tree
 *  @param root - tree to store the array
 *  @param arr - array to be copied into tree
 *  @return
 *          int
 */
template <typename T>
int inorderToTree(AVLNode<T>* root, AVLNode<T>* arr, int arrSize, int i){
    if(root == nullptr)
        return i;

    i = inorderToTree(root->leftNode,arr,arrSize,i);
    if(i > arrSize-1)
        return i;
    *root = arr[i++];
    while (arr[i].key == root->key)
        i++;
    return inorderToTree(root->rightNode,arr,arrSize,i);
}


template <typename T>
void getTreeSize(AVLNode<T>* root, int* counter){
    if(root == nullptr)
        return;

    getTreeSize(root->leftNode,counter);
    (*counter)++;
    getTreeSize(root->rightNode,counter);
}

/*
 * Combining tree1 and tree2 into newTree (which is nearly-complete and empty) using Inorder arrays
 * Might need operator= for Node placement
 * O(n1+n2)
 * @param - newTree, tree1, tree2
 * @return -
 *          void
 */
template <typename T>
void combineAVLTrees(AVLTree<T>& newTree, AVLTree<T>& tree1, AVLTree<T>& tree2){
    int size1, size2;
    getTreeSize(tree1.getRoot(),&size1);
    getTreeSize(tree2.getRoot(),&size2);
    AVLNode<T>* arrTree1 = new AVLNode<T>[size1];
    AVLNode<T>* arrTree2 = new AVLNode<T>[size2];
    inorderToArray(tree1.getRoot(),arrTree1,size1,0);
    inorderToArray(tree2.getRoot(),arrTree2,size2,0);

    AVLNode<T>* newArr = new AVLNode<T>[size1 + size2];
    int i,j,k;
    for(i = 0, j = 0, k = 0; i < size1 && j < size2 && k < size1 + size2;){
        if(arrTree1[i].key <= arrTree2[j].key)
            newArr[k++] = arrTree1[i++];
        else if(arrTree1[i].key > arrTree2[j].key)
            newArr[k++] = arrTree2[j++];
    }
    if(i < size1){
        for(; i < size1; i++, k++)
            newArr[k] = arrTree1[i];
    }
    else if(j < size2){
        for(; j < size2; j++, k++)
            newArr[k] = arrTree2[j];
    }

    inorderToTree(newTree.getRoot(),newArr,size1+size2,0);

    delete[] arrTree1;
    delete[] arrTree2;
    delete[] newArr;
}


/*
 * Create new empty complete tree using Preorder traversal
 * @param height - the height of the complete tree
 * @return
 *          AVLNode<T>*
 */
template <typename T>
AVLNode<T>* createEmptyTree(int height){
    if(height <= 0)
        return nullptr;
    AVLNode<T>* node = new AVLNode<T>;
    node->leftNode = createEmptyTree<T>(height-1);
    node->rightNode = createEmptyTree<T>(height-1);
    return node;
}

/*
 * Remove the leafs from the complete tree, so it will be nearly complete, using Inorder traversal
 * THIS FUNCTION ASSUMES A DEFAULT KEY VALUE FOR EMPTY NODES
 * @param tree - the tree to adjust its size
 * @param root - the current node that is being worked on
 * @param toDelete - the amount of nodes to remove
 * @return
 *          void
 */
template <typename T>
void adjustTreeSize(AVLTree<T>& tree, AVLNode<T>* root, int* toDelete){
    if(root == nullptr || *toDelete == 0)
        return;
    adjustTreeSize(tree,root->rightNode,toDelete);
    if(root->rightNode == nullptr && root->leftNode == nullptr){
        tree.remove(root->key);
        (*toDelete)--;
    }
    else
        adjustTreeSize(tree,root->leftNode,toDelete);
=======
	if (teamId <= 0)
		return output_t<int>(StatusType::INVALID_INPUT);
	else if (!teamsTree.nodeExists(teamId))
		return output_t<int>(StatusType::FAILURE);
	else
	{
		int team_strength = teamsTree.search(teamId)->data.getTeamStrength();
		return output_t<int>(team_strength);
	}
	return 0;
>>>>>>> 34f4e39f2f88b6de94a18d5b2569fbdbcf17e846
}

StatusType Olympics::unite_teams(int teamId1, int teamId2)
{
    if(teamId1 == teamId2 || teamId1 <=0 || teamId2 <=0)
        return StatusType::INVALID_INPUT;
    if(!teamsTree.nodeExists(teamId1) || !teamsTree.nodeExists(teamId2))
        return StatusType::FAILURE;
    Team team1 = teamsTree.search(teamId1)->data;
    Team team2 = teamsTree.search(teamId2)->data;
    if(team1.getSport() != team2.getSport() || team1.getCountryId() != team2.getCountryId())
        return StatusType::FAILURE;

    //IndicesTrees
    for(int i = 0; i < 3; i++){
        int size1, size2;
        getTreeSize(team1.getIndicesTrees()[i].getRoot(),&size1);
        getTreeSize(team2.getIndicesTrees()[i].getRoot(),&size2);
        int newSize = size1 + size2;
        int height = std::ceil(std::log(newSize+1))-1;

        AVLNode<Contestant>* newRoot = createEmptyTree<Contestant>(height);
        AVLTree<Contestant> emptyTree;
        emptyTree.setRoot(newRoot);
        int toDelete = (std::pow(2,height+1)-1-newSize);
        adjustTreeSize(emptyTree,emptyTree.getRoot(),&toDelete);
        combineAVLTrees(emptyTree,team1.getIndicesTrees()[i],team2.getIndicesTrees()[i]);

        team1.getIndicesTrees()[i] = emptyTree;
    }

	return StatusType::FAILURE;
}

StatusType Olympics::play_match(int teamId1, int teamId2)
{
	if(teamId1 == teamId2 || teamId1 <=0 || teamId2 <=0)
		return StatusType::INVALID_INPUT;
	if(!teamsTree.nodeExists(teamId1) || !teamsTree.nodeExists(teamId2))
		return StatusType::FAILURE;
	Team team1 = teamsTree.search(teamId1)->data;
	Team team2 = teamsTree.search(teamId2)->data;
	if(team1.getSport() != team2.getSport())
		return StatusType::FAILURE;
	Country country1 = countriesTree.search(team1.getCountryId())->data;
	Country country2 = countriesTree.search(team2.getCountryId())->data;
	int score1 = country1.getMedals() + team1.getStrength(), score2 = country2.getMedals() + team2.getStrength();
	if(score1 > score2)
		country1.addMedal();
	else if(score1 < score2)
		country2.getMedals();
	return StatusType::SUCCESS;
}

output_t<int> Olympics::austerity_measures(int teamId)
{
	if(teamId <= 0)
		return {StatusType::INVALID_INPUT};
	if(!teamsTree.nodeExists(teamId))
		return {StatusType::FAILURE};
	Team team = teamsTree.search(teamId)->data;
	if(team.getCountContestant() < 3)
		return {StatusType::FAILURE};
	return {team.getMaxStrength()};
}

Team Olympics::get_team(int teamId)
{
	return teamsTree.search(teamId)->data;
}
