#include "olympics24a2.h"

olympics_t::olympics_t() { this->teamsCounter = 0; }

olympics_t::~olympics_t() {}

StatusType olympics_t::add_team(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (this->hashTable.teamExists(teamId)) {
        return StatusType::FAILURE;
    }
    this->hashTable.insert(teamId);
    this->teamsCounter += 1;
    return StatusType::SUCCESS;
}

StatusType olympics_t::remove_team(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!this->hashTable.teamExists(teamId)) {
        return StatusType::FAILURE;
    }
    Team* team = this->hashTable.getTeam(teamId);
    TwoKeysIntStr teamKey(team->getTeamStr(), teamId);
    strTree.remove(teamKey);
    this->hashTable.remove(teamId);
    this->teamsCounter -= 1;
    return StatusType::SUCCESS;
}

StatusType olympics_t::add_player(int teamId, int playerStrength) {
    if (teamId <= 0 || playerStrength <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!this->hashTable.teamExists(teamId)) {
        return StatusType::FAILURE;
    }
    Team* team = this->hashTable.getTeam(teamId);
    TwoKeysIntStr teamKey(team->getTeamStr(), teamId);
    if (!team->isEmpty()) {
        int medals = strTree.getWins(teamKey);
        team->setMedals(medals);
        strTree.remove(teamKey);
    }
    team->insertPlayer(playerStrength);
    teamKey.key1 = team->getTeamStr();
    strTree.insert(teamKey, team->getTeamStr(), team->getMedals());
    return StatusType::SUCCESS;
}

StatusType olympics_t::remove_newest_player(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!this->hashTable.teamExists(teamId) ||
        this->hashTable.getTeam(teamId)->isEmpty()) {
        return StatusType::FAILURE;
    }
    Team* team = this->hashTable.getTeam(teamId);
    TwoKeysIntStr teamKey(team->getTeamStr(), teamId);
    int medals = strTree.getWins(teamKey);
    team->setMedals(medals);
    strTree.remove(teamKey);
    team->removeNewestPlayer();
    if (!team->isEmpty()) {
        teamKey.key1 = team->getTeamStr();
        strTree.insert(teamKey, team->getTeamStr(), team->getMedals());
    }
    return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_match(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return {StatusType::INVALID_INPUT};
    } else if (!this->hashTable.teamExists(teamId1) ||
               this->hashTable.getTeam(teamId1)->isEmpty() ||
               !this->hashTable.teamExists(teamId2) ||
               this->hashTable.getTeam(teamId2)->isEmpty()) {
        return {StatusType::FAILURE};
    }

    // TODO: Need to update the strengths tree with new medal

    Team* team1 = this->hashTable.getTeam(teamId1);
    Team* team2 = this->hashTable.getTeam(teamId2);
    TwoKeysIntStr team1Key(team1->getTeamStr(), teamId1);
    TwoKeysIntStr team2Key(team2->getTeamStr(), teamId2);
    int indexTeam1 = strTree.findIndex(team1Key);
    int indexTeam2 = strTree.findIndex(team2Key);

    if (team1->getTeamStr() > team2->getTeamStr()) {
        strTree.Add(indexTeam1, indexTeam1, 1);
        return {teamId1};
    } else if (team2->getTeamStr() > team1->getTeamStr()) {
        strTree.Add(indexTeam2, indexTeam2, 1);
        return {teamId2};
    }

    // Both teams have the same strength, return the team with the lower ID
    else {
        if (teamId1 < teamId2) {
            strTree.Add(indexTeam1, indexTeam1, 1);
            return {teamId1};
        } else {
            strTree.Add(indexTeam2, indexTeam2, 1);
            return {teamId2};
        }
    }
}

output_t<int> olympics_t::num_wins_for_team(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    } else if (!this->hashTable.teamExists(teamId)) {
        return StatusType::FAILURE;
    }
    Team* team = this->hashTable.getTeam(teamId);
    if (team->isEmpty()) {
        return {team->getMedals()};
    }
    TwoKeysIntStr teamKey(team->getTeamStr(), teamId);
    int wins = this->strTree.getWins(teamKey);
    return {wins};
}

output_t<int> olympics_t::get_highest_ranked_team() {
    int treeMaxRank = strTree.getMaxRank();
    if (treeMaxRank == -1 && this->teamsCounter > 0) {
        return 0;
    }
    return {treeMaxRank};
}

StatusType olympics_t::unite_teams(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    } else if (!this->hashTable.teamExists(teamId1) ||
               !this->hashTable.teamExists(teamId2)) {
        return StatusType::FAILURE;
    }
    Team* team1 = this->hashTable.getTeam(teamId1);
    Team* team2 = this->hashTable.getTeam(teamId2);
    TwoKeysIntStr team1Key(team1->getTeamStr(), teamId1);
    TwoKeysIntStr team2Key(team2->getTeamStr(), teamId2);
    if (!team1->isEmpty()) {
        int medals = strTree.getWins(team1Key);
        team1->setMedals(medals);
        strTree.remove(team1Key);
    }
    strTree.remove(team2Key);
    team1->purchaseTeam(team2);
    this->hashTable.remove(teamId2);
    this->teamsCounter -= 1;
    if (!team1->isEmpty()) {
        team1Key.key1 = team1->getTeamStr();
        strTree.insert(team1Key, team1->getTeamStr(), team1->getMedals());
    }
    return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_tournament(int lowPower, int highPower) {
    if (lowPower <= 0 || highPower <= 0 || highPower <= lowPower) {
        return {StatusType::INVALID_INPUT};
    }
    // Find smallest team with str >= lowPower
    // Find largest team with str <= highPower
    TwoKeysIntStr keyLow = strTree.closestLowPowerAbove(lowPower);
    TwoKeysIntStr keyHigh = strTree.closestHighPowerBelow(highPower);
    // Find their index and calculate how many teams are between them
    int indexLow = strTree.findIndex(keyLow);
    int indexHigh = strTree.findIndex(keyHigh);
    int numOfTeams = (indexHigh - indexLow) + 1;
    if (strTree.isTreeEmpty() || indexLow == 0 || indexHigh == 0) {
        return {StatusType::FAILURE};
    }
    // Determine if numOfTeams is a power of two
    int numOfTeamsTemp = numOfTeams;
    while (numOfTeamsTemp % 2 == 0 && numOfTeamsTemp > 0) {
        numOfTeamsTemp /= 2;
    }
    if (numOfTeamsTemp != 1) {
        return {StatusType::FAILURE};
    }

    while (numOfTeams > 1) {
        // Add between indexHigh and (indexHigh - indexLow + 1)/2 + indexLow

        indexLow = (indexHigh - indexLow + 1) / 2 + indexLow;
        strTree.Add(indexLow, indexHigh, 1);
        numOfTeams /= 2;
    }

    return {keyHigh.key2};
}
