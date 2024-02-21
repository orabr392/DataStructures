//
// 234218 Data Structures 1.
// Semester: 2024A (winter).
// Wet Exercise #1.
//
// Recommended TAB size to view this file: 8.
//
// The following main file is necessary to link and run your code.
// This file is READ ONLY: even if you submit something else, the compiler ..
// .. WILL use our file.
//

#include "Olympicsa1.h"
#include <iostream>
#include <string>
#include "wet1util.h"

using namespace std;

void print(string cmd, StatusType res);
void print(string cmd, output_t<int> res);

int main()
{

    int d1, d2, d3, g1;
    string b1;
    // bool b;

    // Init
    Olympics *obj = new Olympics();

    // Add country
    obj->add_country(1, 10);
    obj->add_team(1, 1, Sport::BOULDERING);
    obj->add_team(2, 1, Sport::BOULDERING);
    obj->add_contestant(1, 1, Sport::BOULDERING, 2);
    obj->add_contestant(4, 1, Sport::BOULDERING, 5);
    obj->add_contestant(6, 1, Sport::BOULDERING, 3);
    obj->add_contestant(10, 1, Sport::BOULDERING, 10);
    obj->add_contestant(16, 1, Sport::BOULDERING, 11);
    obj->add_contestant(20, 1, Sport::BOULDERING, 15);
    obj->add_contestant(100, 1, Sport::BOULDERING, 7);
    obj->add_contestant(101, 1, Sport::BOULDERING, 6);
    obj->add_contestant(102, 1, Sport::BOULDERING, 4);

    obj->add_contestant_to_team(1, 1);
    obj->add_contestant_to_team(1, 4);
    obj->add_contestant_to_team(1, 6);
    // obj->add_contestant_to_team(1, 16);
    // obj->add_contestant_to_team(1, 10);
    // obj->add_contestant_to_team(1, 20);
    // obj->add_contestant_to_team(2, 101);
    // obj->add_contestant_to_team(2, 100);
    // obj->add_contestant_to_team(2, 102);

    // // t->printCurrentStrengths();
    // // // t.printCurrentContestants();
    // print("get_team_strength", obj->get_team_strength(1));
    // print("austerity_measures", obj->austerity_measures(1));

    // print("get_team_strength", obj->get_team_strength(2));
    // print("austerity_measures", obj->austerity_measures(2));

    obj->unite_teams(1, 2);

    // print("get_team_strength", obj->get_team_strength(1));
    // print("austerity_measures", obj->austerity_measures(1));

    Team *t = obj->get_team(1);
    t->printCurrentContestants();
    // std::cout << std::endl;
    t->printCurrentStrengths();

    //// Execute all commands in file
    // string op;
    // while (cin >> op)
    // {
    //     if (!op.compare("add_country"))
    //     {
    //         cin >> d1 >> d2;
    //         print(op, obj->add_country(d1, d2));
    //     }
    //     else if (!op.compare("remove_country"))
    //     {
    //         cin >> d1;
    //         print(op, obj->remove_country(d1));
    //     }
    //     else if (!op.compare("add_team"))
    //     {
    //         cin >> d1 >> d2 >> d3;
    //         print(op, obj->add_team(d1, d2, (Sport)d3));
    //     }
    //     else if (!op.compare("remove_team"))
    //     {
    //         cin >> d1;
    //         print(op, obj->remove_team(d1));
    //     }
    //     else if (!op.compare("add_contestant"))
    //     {
    //         cin >> d1 >> d2 >> d3 >> g1;
    //         print(op, obj->add_contestant(d1, d2, (Sport)d3, g1));
    //     }
    //     else if (!op.compare("remove_contestant"))
    //     {
    //         cin >> d1;
    //         print(op, obj->remove_contestant(d1));
    //     }
    //     else if (!op.compare("add_contestant_to_team"))
    //     {
    //         cin >> d1 >> d2;
    //         print(op, obj->add_contestant_to_team(d1, d2));
    //     }
    //     else if (!op.compare("remove_contestant_from_team"))
    //     {
    //         cin >> d1 >> d2;
    //         print(op, obj->remove_contestant_from_team(d1, d2));
    //     }
    //     else if (!op.compare("update_contestant_strength"))
    //     {
    //         cin >> d1 >> d2;
    //         print(op, obj->update_contestant_strength(d1, d2));
    //     }
    //     else if (!op.compare("get_strength"))
    //     {
    //         cin >> d1;
    //         print(op, obj->get_strength(d1));
    //     }
    //     else if (!op.compare("get_medals"))
    //     {
    //         cin >> d1;
    //         print(op, obj->get_medals(d1));
    //     }
    //     else if (!op.compare("get_team_strength"))
    //     {
    //         cin >> d1;
    //         print(op, obj->get_team_strength(d1));
    //     }
    //     else if (!op.compare("unite_teams"))
    //     {
    //         cin >> d1 >> d2;
    //         print(op, obj->unite_teams(d1, d2));
    //     }
    //     else if (!op.compare("play_match"))
    //     {
    //         cin >> d1 >> d2;
    //         print(op, obj->play_match(d1, d2));
    //     }
    //     else if (!op.compare("austerity_measures"))
    //     {
    //         cin >> d1;
    //         print(op, obj->austerity_measures(d1));
    //     }
    //     else
    //     {
    //         cout << "Unknown command: " << op << endl;
    //         return -1;
    //     }
    //     // Verify no faults
    //     if (cin.fail())
    //     {
    //         cout << "Invalid input format" << endl;
    //         return -1;
    //     }
    // }

    // Quit
    delete obj;
    return 0;
}

// Helpers
static const char *StatusTypeStr[] =
    {
        "SUCCESS",
        "ALLOCATION_ERROR",
        "INVALID_INPUT",
        "FAILURE"};

void print(string cmd, StatusType res)
{
    cout << cmd << ": " << StatusTypeStr[(int)res] << endl;
}

void print(string cmd, output_t<int> res)
{
    if (res.status() == StatusType::SUCCESS)
    {
        cout << cmd << ": " << StatusTypeStr[(int)res.status()] << ", " << res.ans() << endl;
    }
    else
    {
        cout << cmd << ": " << StatusTypeStr[(int)res.status()] << endl;
    }
}
