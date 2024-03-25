#include "olympics24a2.h"

olympics_t::olympics_t() {}

olympics_t::~olympics_t() {}

StatusType olympics_t::add_team(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (this->hashTable.teamExists(teamId)) {
        return StatusType::FAILURE;
    }
    this->hashTable.insert(teamId);
    return StatusType::SUCCESS;
}

StatusType olympics_t::remove_team(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!this->hashTable.teamExists(teamId)) {
        return StatusType::FAILURE;
    }
    this->hashTable.remove(teamId);
    // TODO: Need to remove from strengths tree
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
    team->insertPlayer(playerStrength);
    // TODO: Need to remove and insert the team into the strengths tree
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
    team->removeNewestPlayer();
    // TODO: Need to remove and insert the team into the strengths tree
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
    if (team1->getTeamStr() > team2->getTeamStr()) {
        return {teamId1};
    } else if (team2->getTeamStr() > team1->getTeamStr()) {
        return {teamId2};
    }

    // Both teams have the same strength, return the team with the lower ID
    else {
        if (teamId1 < teamId2) {
            return {teamId1};
        } else {
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
    int wins = this->strTree.getWins(teamId);
    return {wins};
}

output_t<int> olympics_t::get_highest_ranked_team() {
    return {this->strTree.getMaxRank()};
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
    team1->purchaseTeam(team2);
    this->hashTable.remove(teamId2);
    // TODO: Remove team2 from strenghts tree and reinsert team1 to strengths
    // tree
    return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_tournament(int lowPower, int highPower) {
    // TODO: Your code goes here
    static int i = 0;
    return (i++ == 0) ? 11 : 2;
}
