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
	if (countryId <= 0 || medals <= 0)
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
	Team team(teamId, countryId, sport);
	if (!teamsTree.insert(teamId, team))
		return StatusType::ALLOCATION_ERROR;
	countriesTree.search(countryId)->data.changeTeamsCounter(ADDTEAM);
	return StatusType::SUCCESS;
}

/*
/  Currently running in O(logn + logk) need to change
/  countries method for checking if its empty
/  possibly a linked list might work
/  and checking the head node only
*/
StatusType Olympics::remove_team(int teamId)
{
	if (teamId <= 0)
		return StatusType::INVALID_INPUT;
	if (!teamsTree.nodeExists(teamId))
		return StatusType::FAILURE;
	int countryId = teamsTree.search(teamId)->data.getCountryId();
	if (!teamsTree.remove(teamId))
		return StatusType::ALLOCATION_ERROR;
	countriesTree.search(countryId)->data.changeTeamsCounter(REMOVETEAM);
	return StatusType::SUCCESS;
}

StatusType Olympics::add_contestant(int contestantId, int countryId, Sport sport, int strength)
{
	if (contestantId <= 0 || countryId <= 0 || strength < 0)
		return StatusType::INVALID_INPUT;
	if (contestantsTree.nodeExists(contestantId) || !countriesTree.nodeExists(countryId))
		return StatusType::FAILURE;
	Contestant contestant(contestantId, countryId, sport, strength);
	if (!contestantsTree.insert(contestantId, contestant))
		return StatusType::ALLOCATION_ERROR;
	countriesTree.search(countryId)->data.changeContestantsCounter(ADDCONTESTANT);
	return StatusType::SUCCESS;
}

/*
/  Currently running in O(logn + logk) need to change
/  countries method for checking if its empty
/  possibly a linked list might work
/  and checking the head node only
*/
StatusType Olympics::remove_contestant(int contestantId)
{
	if (contestantId <= 0)
		return StatusType::INVALID_INPUT;
	if (!contestantsTree.nodeExists(contestantId) || contestantsTree.search(contestantId)->data.isContestantActive())
		return StatusType::FAILURE;
	int countryId = contestantsTree.search(contestantId)->data.getCountryId();
	if (!contestantsTree.remove(contestantId))
		return StatusType::ALLOCATION_ERROR;
	countriesTree.search(countryId)->data.changeContestantsCounter(REMOVECONTESTANT);
	return StatusType::SUCCESS;
}

StatusType Olympics::add_contestant_to_team(int teamId, int contestantId)
{
	return StatusType::FAILURE;
}

StatusType Olympics::remove_contestant_from_team(int teamId, int contestantId)
{
	return StatusType::FAILURE;
}

StatusType Olympics::update_contestant_strength(int contestantId, int change)
{
	return StatusType::FAILURE;
}

output_t<int> Olympics::get_strength(int contestantId)
{
	return 0;
}

output_t<int> Olympics::get_medals(int countryId)
{
	return 0;
}

output_t<int> Olympics::get_team_strength(int teamId)
{
	return 0;
}

StatusType Olympics::unite_teams(int teamId1, int teamId2)
{
	return StatusType::FAILURE;
}

StatusType Olympics::play_match(int teamId1, int teamId2)
{
	return StatusType::FAILURE;
}

output_t<int> Olympics::austerity_measures(int teamId)
{
	return 0;
}
