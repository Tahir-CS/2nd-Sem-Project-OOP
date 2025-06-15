#include "Administrator.h"
#include "LocalElection.h"
#include "NationalElection.h"
#define MAX_ELECTIONS 10
#include <cstring>
#include <iostream>
using namespace std;

Administrator::Administrator(const string &uname, const string &pwd) : User(uname, pwd) {}

void Administrator::createElection(Election *elections[], int &count, const string &type, const string &title)
{
    if (count < MAX_ELECTIONS)
    {
        if (type == "local")
        {
            string city;
            cout << "Enter city name: ";
            cin >> city;
            elections[count++] = new LocalElection(title, city);
        }
        else if (type == "national")
        {
            elections[count++] = new NationalElection(title);
        }
    }
}
void Administrator::addCandidate(Election *election, const string &name, const string &party)
{
    if (!election->isPartyAllowed(party))
    {
        cout << "Party already exists in this election!\n";
        return;
    }
    election->addCandidate(name, party);
}
void Administrator::editCandidate(Election *election, int index, const string &newName, const string &newParty)
{
    election->editCandidate(index, newName, newParty);
}
void Administrator::removeCandidate(Election *election, int index)
{
    election->removeCandidate(index);
}
void Administrator::startElection(Election *election, Election **elections, int electionCount)
{
    // shows active national election
    if (dynamic_cast<NationalElection *>(election))
    {
        for (int i = 0; i < electionCount; ++i)
        {
            if (dynamic_cast<NationalElection *>(elections[i]) && elections[i]->getStatus())
            {
                cout << "Another national election is already active!\n";
                return;
            }
        }
    }
    int minutes, seconds;
    cout << "Enter election duration (minutes): ";
    cin >> minutes;
    cout << "Enter election duration (seconds): ";
    cin >> seconds;
    election->startElection(minutes, seconds);
}
void Administrator::endElection(Election *election) { election->endElection(); }
void Administrator::viewResults(Election *election) { election->showResults(); }