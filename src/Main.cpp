#include "db_utils.h"
#include "ColorConsole.h"
#include <iostream>
#include <direct.h> // For _mkdir on Windows
#include <cstdlib>
#include "sqlite3.h"
#define MAX_USERS 10
#define MAX_ELECTIONS 10
#include "Voter.h"
#include "Administrator.h"
#include "LocalElection.h"
#include "NationalElection.h"
using namespace std;

int main()
{
    sqlite3 *db;
    if (initializeDatabase(&db) != SQLITE_OK) {
        return 1;
    }
    _mkdir("data");
    bool running = true;    while (running)
    {
        ColorConsole::clearScreen();
        ColorConsole::printHeader("ONLINE VOTING SYSTEM");
        ColorConsole::printMenuOption("1. Login as Voter", ColorConsole::Color::LIGHTGREEN);
        ColorConsole::printMenuOption("2. Login as Admin", ColorConsole::Color::LIGHTMAGENTA);
        ColorConsole::printMenuOption("3. Register Voter", ColorConsole::Color::WHITE);
        ColorConsole::printMenuOption("4. Exit", ColorConsole::Color::LIGHTRED);
        ColorConsole::printSeparator();
        ColorConsole::centerColoredText("Select option: ", ColorConsole::Color::YELLOW);
        int mainChoice;
        cin >> mainChoice;        if (mainChoice == 1 || mainChoice == 2)
        {
            ColorConsole::clearScreen();
            string uname, pwd, role, name, region;
            int userId, age;
            ColorConsole::printHeader("USER LOGIN");
            ColorConsole::centerColoredText("Enter username: ", ColorConsole::Color::LIGHTCYAN);
            cin >> uname;
            ColorConsole::centerColoredText("Enter password: ", ColorConsole::Color::LIGHTCYAN);
            cin >> pwd;            if (loginUser(db, uname, pwd, role, userId, name, age, region)) {
                if ((mainChoice == 1 && role == "voter") || (mainChoice == 2 && role == "admin")) {
                    ColorConsole::printSuccess("Login successful!");
                    if (role == "voter") {
                        bool voterMenu = true;
                        while (voterMenu) {
                            ColorConsole::clearScreen();
                            ColorConsole::printHeader("VOTER MENU");
                            ColorConsole::printMenuOption("1. View Elections", ColorConsole::Color::LIGHTGREEN);
                            ColorConsole::printMenuOption("2. Vote", ColorConsole::Color::LIGHTMAGENTA);
                            ColorConsole::printMenuOption("3. View Results", ColorConsole::Color::WHITE);
                            ColorConsole::printMenuOption("4. Logout", ColorConsole::Color::LIGHTRED);
                            ColorConsole::printSeparator();
                            ColorConsole::centerColoredText("Select option: ", ColorConsole::Color::YELLOW);
                            int vOpt;
                            cin >> vOpt;                            if (vOpt == 1) {
                                showElections(db);
                                ColorConsole::pauseScreen();
                            } else if (vOpt == 2) {
                                castVote(db, userId);
                                ColorConsole::pauseScreen();
                            } else if (vOpt == 3) {
                                viewResults(db);
                                ColorConsole::pauseScreen();
                            } else if (vOpt == 4) {
                                voterMenu = false;
                            } else {
                                ColorConsole::printError("Invalid option!");
                                ColorConsole::pauseScreen();
                            }
                        }                    } else if (role == "admin") {
                        bool adminMenu = true;
                        while (adminMenu) {
                            ColorConsole::clearScreen();
                            ColorConsole::printHeader("ADMIN MENU");
                            ColorConsole::printMenuOption("1. Create Election", ColorConsole::Color::LIGHTGREEN);
                            ColorConsole::printMenuOption("2. Add Candidate", ColorConsole::Color::LIGHTMAGENTA);
                            ColorConsole::printMenuOption("3. Start Election", ColorConsole::Color::WHITE);
                            ColorConsole::printMenuOption("4. End Election", ColorConsole::Color::YELLOW);
                            ColorConsole::printMenuOption("5. View Results", ColorConsole::Color::LIGHTCYAN);
                            ColorConsole::printMenuOption("6. Logout", ColorConsole::Color::LIGHTRED);
                            ColorConsole::printSeparator();
                            ColorConsole::centerColoredText("Select option: ", ColorConsole::Color::YELLOW);
                            int aOpt;
                            cin >> aOpt;                            if (aOpt == 1) {
                                addElection(db);
                                ColorConsole::pauseScreen();
                            } else if (aOpt == 2) {
                                addCandidate(db);
                                ColorConsole::pauseScreen();
                            } else if (aOpt == 3) {
                                setElectionStatus(db, true);
                                ColorConsole::pauseScreen();
                            } else if (aOpt == 4) {
                                setElectionStatus(db, false);
                                ColorConsole::pauseScreen();
                            } else if (aOpt == 5) {
                                viewResults(db);
                                ColorConsole::pauseScreen();
                            } else if (aOpt == 6) {
                                adminMenu = false;
                            } else {
                                ColorConsole::printError("Invalid option!");
                                ColorConsole::pauseScreen();
                            }
                        }
                    }                } else {
                    ColorConsole::printError("Role mismatch!");
                    ColorConsole::pauseScreen();
                }
            } else {
                ColorConsole::printError("Login failed!");
                ColorConsole::pauseScreen();
            }
        }
        else if (mainChoice == 3)
        {
            ColorConsole::clearScreen();
            registerVoter(db);
            ColorConsole::pauseScreen();
        }
        else if (mainChoice == 4)
        {
            ColorConsole::printWarning("Thank you for using the Voting System!");
            running = false;
        }
        else
        {
            ColorConsole::printError("Invalid option!");
            ColorConsole::pauseScreen();
        }
    }
    sqlite3_close(db);
    return 0;
}