#pragma once
#include <string>
#include "sqlite3.h"

// Database initialization
int initializeDatabase(sqlite3 **db);

// User management
void registerVoter(sqlite3 *db);
bool loginUser(sqlite3 *db, const std::string &uname, const std::string &pwd, std::string &role, int &userId, std::string &name, int &age, std::string &region);

// Election management
void showElections(sqlite3 *db);
void addElection(sqlite3 *db);
void setElectionStatus(sqlite3 *db, bool start);

// Candidate management
void addCandidate(sqlite3 *db);
void showCandidates(sqlite3 *db, int eid);

// Voting
void castVote(sqlite3 *db, int userId);
void viewResults(sqlite3 *db);
