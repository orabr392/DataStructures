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

	int *teamsId = contestantsTree.search(contestantId)->data.getTeamsId();
	for (int i = 0; i < PARTITIONS; i++)
	{
		remove_contestant_from_team(*(teamsId + i), contestantId);
	}
	contestantsTree.search(contestantId)->data.changeContestantStrength(change);
	for (int i = 0; i < PARTITIONS; i++)
	{
		add_contestant_to_team(*(teamsId + i), contestantId);
	}
	return StatusType::SUCCESS;
}

output_t<int> Olympics::get_strength(int contestantId)
{
	if (contestantId <= 0)
		return output_t<int>(StatusType::INVALID_INPUT);
	else if (!contestantsTree.nodeExists(contestantId))
		return output_t<int>(StatusType::FAILURE);
	else
	{
		int strength = contestantsTree.search(contestantId)->data.getContestantStrength();
		return output_t<int>(strength);
	}
}

output_t<int> Olympics::get_medals(int countryId)
{
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
}

output_t<int> Olympics::get_team_strength(int teamId)
{
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
