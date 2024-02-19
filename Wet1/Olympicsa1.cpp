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
	if (!teamsTree.search(teamId)->data.removeContestantFromTeam(*contestant) ||
		!contestant->leaveTeam(teamId))
		return StatusType::ALLOCATION_ERROR;
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

/*
 * Going through the contestants tree and remove contestant from the team
 * (actually remove the team from contestant)
 * @param root - the root of the tree currently working on
 * @param teamID - the team to be removed
 * @return
 *          void
 */
void removeAllContestantFromTeam(AVLNode<int, Contestant> *root, int teamID)
{
	if (root == nullptr)
		return;
	removeAllContestantFromTeam(root->leftNode, teamID);
	for (int i = 0; i < 3; i++)
	{
		if (root->data.getTeamsId()[i] == teamID)
		{
			root->data.getTeamsId()[i] = 0;
		}
	}
	removeAllContestantFromTeam(root->rightNode, teamID);
}

StatusType Olympics::unite_teams(int teamId1, int teamId2)
{
	if (teamId1 == teamId2 || teamId1 <= 0 || teamId2 <= 0)
		return StatusType::INVALID_INPUT;
	if (!teamsTree.nodeExists(teamId1) || !teamsTree.nodeExists(teamId2))
		return StatusType::FAILURE;
	Team team1 = teamsTree.search(teamId1)->data;
	Team team2 = teamsTree.search(teamId2)->data;
	if (team1.getSport() != team2.getSport() || team1.getCountryId() != team2.getCountryId())
		return StatusType::FAILURE;

	// IndicesTrees
	for (int i = 0; i < 3; i++)
	{
		int size1 = team1.getIndicesTrees()[i].getTreeSize(), size2 = team2.getIndicesTrees()[i].getTreeSize();
		int newSize = size1 + size2;
		int height = std::ceil(std::log(newSize + 1)) - 1;

		AVLNode<int, Contestant> *newRoot = AVLTree<int, Contestant>::createEmptyTree(height);
		AVLTree<int, Contestant> emptyTree;
		emptyTree.setRoot(newRoot);
		int toDelete = (std::pow(2, height + 1) - 1 - newSize);
		AVLTree<int, Contestant>::adjustTreeSize(emptyTree, emptyTree.getRoot(), &toDelete);
		AVLTree<int, Contestant>::combineAVLTrees(emptyTree, team1.getIndicesTrees()[i], team2.getIndicesTrees()[i]);

		team1.getIndicesTrees()[i] = emptyTree; // Not so empty anymore
		removeAllContestantFromTeam(emptyTree.getRoot(), teamId2);
	}
	// StrengthsTrees
	for (int i = 0; i < 3; i++)
	{
		int size1 = team1.getStrengthsTrees()[i].getTreeSize(), size2 = team2.getStrengthsTrees()[i].getTreeSize();
		int newSize = size1 + size2;
		int height = std::ceil(std::log(newSize + 1)) - 1;

		AVLNode<TwoKeysInt, Contestant> *newRoot = AVLTree<TwoKeysInt, Contestant>::createEmptyTree(height);
		AVLTree<TwoKeysInt, Contestant> emptyTree;
		emptyTree.setRoot(newRoot);
		int toDelete = (std::pow(2, height + 1) - 1 - newSize);
		AVLTree<TwoKeysInt, Contestant>::adjustTreeSize(emptyTree, emptyTree.getRoot(), &toDelete);
		AVLTree<TwoKeysInt, Contestant>::combineAVLTrees(emptyTree, team1.getStrengthsTrees()[i], team2.getStrengthsTrees()[i]);

		team1.getStrengthsTrees()[i] = emptyTree; // Not so empty anymore
	}

	return StatusType::FAILURE;
}

StatusType Olympics::play_match(int teamId1, int teamId2)
{
	if (teamId1 == teamId2 || teamId1 <= 0 || teamId2 <= 0)
		return StatusType::INVALID_INPUT;
	if (!teamsTree.nodeExists(teamId1) || !teamsTree.nodeExists(teamId2))
		return StatusType::FAILURE;
	Team team1 = teamsTree.search(teamId1)->data;
	Team team2 = teamsTree.search(teamId2)->data;
	if (team1.getSport() != team2.getSport())
		return StatusType::FAILURE;
	Country country1 = countriesTree.search(team1.getCountryId())->data;
	Country country2 = countriesTree.search(team2.getCountryId())->data;
	int score1 = country1.getMedals() + team1.getTeamStrength(), score2 = country2.getMedals() + team2.getTeamStrength();
	if (score1 > score2)
		country1.addMedal();
	else if (score1 < score2)
		country2.getMedals();
	return StatusType::SUCCESS;
}

output_t<int> Olympics::austerity_measures(int teamId)
{
	if (teamId <= 0)
		return output_t<int>(StatusType::INVALID_INPUT);
	if (!teamsTree.nodeExists(teamId))
		return output_t<int>(StatusType::FAILURE);
	Team *team = &teamsTree.search(teamId)->data;
	// team->calcMaxPossibleStrength();
	if (team->getCurrentCapacity() < 3)
		return output_t<int>(StatusType::FAILURE);
	return output_t<int>(team->getMaxStrength());
}

Team Olympics::get_team(int teamId)
{
	return teamsTree.search(teamId)->data;
}
