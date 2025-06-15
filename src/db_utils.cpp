#include "db_utils.h"
#include "ColorConsole.h"
#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

int initializeDatabase(sqlite3 **db) {
    int rc = sqlite3_open("data/voting.db", db);
    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(*db) << endl;
        return rc;
    }
    const char *userTable =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY,"
        "username TEXT UNIQUE,"
        "password TEXT,"
        "name TEXT,"
        "age INTEGER,"
        "region TEXT,"
        "role TEXT);";
    rc = sqlite3_exec(*db, userTable, 0, 0, 0);
    if (rc != SQLITE_OK) {
        cerr << "Failed to create users table." << endl;
        return rc;
    }
    const char *electionTable =
        "CREATE TABLE IF NOT EXISTS elections ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "type TEXT,"
        "title TEXT,"
        "status INTEGER,"
        "region TEXT);";
    rc = sqlite3_exec(*db, electionTable, 0, 0, 0);
    if (rc != SQLITE_OK) {
        cerr << "Failed to create elections table." << endl;
        return rc;
    }
    const char *candidateTable =
        "CREATE TABLE IF NOT EXISTS candidates ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "election_id INTEGER,"
        "name TEXT,"
        "party TEXT,"
        "votes INTEGER DEFAULT 0,"
        "FOREIGN KEY(election_id) REFERENCES elections(id));";
    rc = sqlite3_exec(*db, candidateTable, 0, 0, 0);
    if (rc != SQLITE_OK) {
        cerr << "Failed to create candidates table." << endl;
        return rc;
    }
    const char *votesTable =
        "CREATE TABLE IF NOT EXISTS votes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id INTEGER,"
        "election_id INTEGER,"
        "candidate_id INTEGER,"
        "FOREIGN KEY(user_id) REFERENCES users(id),"
        "FOREIGN KEY(election_id) REFERENCES elections(id),"
        "FOREIGN KEY(candidate_id) REFERENCES candidates(id));";
    rc = sqlite3_exec(*db, votesTable, 0, 0, 0);
    if (rc != SQLITE_OK) {
        cerr << "Failed to create votes table." << endl;
        return rc;
    }
    return SQLITE_OK;
}

void registerVoter(sqlite3 *db) {
    ColorConsole::printHeader("VOTER REGISTRATION");
    string newCNIC, newPwd, newName, newRegion;
    int newAge;
    ColorConsole::centerColoredText("Enter CNIC: ", ColorConsole::Color::LIGHTCYAN);
    cin >> newCNIC;
    ColorConsole::centerColoredText("Enter name: ", ColorConsole::Color::LIGHTCYAN);
    cin.ignore();
    getline(cin, newName);
    ColorConsole::centerColoredText("Enter age: ", ColorConsole::Color::LIGHTCYAN);
    cin >> newAge;
    ColorConsole::centerColoredText("Enter region: ", ColorConsole::Color::LIGHTCYAN);
    cin >> newRegion;
    ColorConsole::centerColoredText("Enter new password: ", ColorConsole::Color::LIGHTCYAN);
    cin >> newPwd;
    sqlite3_stmt *stmt;
    const char *checkSql = "SELECT COUNT(*) FROM users WHERE username = ?;";
    sqlite3_prepare_v2(db, checkSql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, newCNIC.c_str(), -1, SQLITE_STATIC);
    int exists = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    if (exists) {
        ColorConsole::printError("A voter with this CNIC already exists!");
        return;
    }
    const char *insertSql = "INSERT INTO users (username, password, name, age, region, role) VALUES (?, ?, ?, ?, ?, 'voter');";
    sqlite3_prepare_v2(db, insertSql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, newCNIC.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, newPwd.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, newName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, newAge);
    sqlite3_bind_text(stmt, 5, newRegion.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        ColorConsole::printSuccess("Registration successful!");
    } else {
        ColorConsole::printError("Registration failed!");
    }
    sqlite3_finalize(stmt);
}

bool loginUser(sqlite3 *db, const std::string &uname, const std::string &pwd, std::string &role, int &userId, std::string &name, int &age, std::string &region) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, role, name, age, region FROM users WHERE username = ? AND password = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, uname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pwd.c_str(), -1, SQLITE_STATIC);
    bool found = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userId = sqlite3_column_int(stmt, 0);
        role = (const char*)sqlite3_column_text(stmt, 1);
        name = (const char*)sqlite3_column_text(stmt, 2);
        age = sqlite3_column_int(stmt, 3);
        region = (const char*)sqlite3_column_text(stmt, 4);
        found = true;
    }
    sqlite3_finalize(stmt);
    return found;
}

void showElections(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, type, title, status, region FROM elections;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    ColorConsole::printHeader("AVAILABLE ELECTIONS");
    bool hasElections = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hasElections = true;
        int id = sqlite3_column_int(stmt, 0);
        string type = (const char*)sqlite3_column_text(stmt, 1);
        string title = (const char*)sqlite3_column_text(stmt, 2);
        int status = sqlite3_column_int(stmt, 3);
        string region = (const char*)sqlite3_column_text(stmt, 4);
        string statusText = status ? "Active" : "Inactive";
        ColorConsole::Color statusColor = status ? ColorConsole::Color::LIGHTGREEN : ColorConsole::Color::LIGHTRED;
        string electionInfo = to_string(id) + ". [" + type + "] " + title + " (" + statusText + ") Region: " + region;
        ColorConsole::centerColoredText(electionInfo, statusColor);
    }
    if (!hasElections) {
        ColorConsole::printWarning("No elections available.");
    }
    sqlite3_finalize(stmt);
}

void addElection(sqlite3 *db) {
    ColorConsole::printHeader("CREATE NEW ELECTION");
    string type, title, region;
    ColorConsole::centerColoredText("Enter election type (local/national): ", ColorConsole::Color::LIGHTCYAN);
    cin >> type;
    ColorConsole::centerColoredText("Enter election title: ", ColorConsole::Color::LIGHTCYAN);
    cin.ignore();
    getline(cin, title);
    if (type == "local") {
        ColorConsole::centerColoredText("Enter city name: ", ColorConsole::Color::LIGHTCYAN);
        cin >> region;
    } else {
        region = "Pakistan";
    }
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO elections (type, title, status, region) VALUES (?, ?, 0, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, region.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        ColorConsole::printSuccess("Election added successfully!");    } else {
        ColorConsole::printError("Failed to add election!");
    }
    sqlite3_finalize(stmt);
}

void addCandidate(sqlite3 *db) {
    showElections(db);
    ColorConsole::printSeparator();
    ColorConsole::centerColoredText("Enter election id: ", ColorConsole::Color::LIGHTCYAN);
    int eid;
    cin >> eid;
    string cname, cparty;
    ColorConsole::centerColoredText("Enter candidate name: ", ColorConsole::Color::LIGHTCYAN);
    cin >> cname;
    ColorConsole::centerColoredText("Enter party: ", ColorConsole::Color::LIGHTCYAN);
    cin >> cparty;
    sqlite3_stmt *stmt;
    const char *checkSql = "SELECT COUNT(*) FROM candidates WHERE election_id = ? AND party = ?;";
    sqlite3_prepare_v2(db, checkSql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, eid);
    sqlite3_bind_text(stmt, 2, cparty.c_str(), -1, SQLITE_STATIC);
    int exists = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) exists = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    if (exists) {
        ColorConsole::printError("A candidate from this party already exists in this election!");
        return;
    }
    const char *sql = "INSERT INTO candidates (election_id, name, party) VALUES (?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, eid);
    sqlite3_bind_text(stmt, 2, cname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, cparty.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        ColorConsole::printSuccess("Candidate added successfully!");
    } else {
        ColorConsole::printError("Failed to add candidate!");
    }
    sqlite3_finalize(stmt);
}

void setElectionStatus(sqlite3 *db, bool start) {
    showElections(db);
    cout << "Enter election id: ";
    int eid;
    cin >> eid;
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE elections SET status = ? WHERE id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, start ? 1 : 0);
    sqlite3_bind_int(stmt, 2, eid);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        cout << (start ? "Election started." : "Election ended.") << "\n";
    } else {
        cout << "Failed to update election status.\n";
    }
    sqlite3_finalize(stmt);
}

void showCandidates(sqlite3 *db, int eid) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, name, party, votes FROM candidates WHERE election_id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, eid);
    cout << "Candidates:\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int cid = sqlite3_column_int(stmt, 0);
        string name = (const char*)sqlite3_column_text(stmt, 1);
        string party = (const char*)sqlite3_column_text(stmt, 2);
        int votes = sqlite3_column_int(stmt, 3);
        cout << cid << ". " << name << " (" << party << ") Votes: " << votes << endl;
    }
    sqlite3_finalize(stmt);
}

void castVote(sqlite3 *db, int userId) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, title FROM elections WHERE status = 1;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    vector<pair<int, string>> activeElections;
    cout << "Active Elections:\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int eid = sqlite3_column_int(stmt, 0);
        string title = (const char*)sqlite3_column_text(stmt, 1);
        activeElections.push_back({eid, title});
        cout << eid << ". " << title << endl;
    }
    sqlite3_finalize(stmt);
    if (activeElections.empty()) {
        cout << "No active elections.\n";
        return;
    }
    cout << "Enter election id: ";
    int eid;
    cin >> eid;
    const char *checkSql = "SELECT COUNT(*) FROM votes WHERE user_id = ? AND election_id = ?;";
    sqlite3_prepare_v2(db, checkSql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, eid);
    int voted = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) voted = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    if (voted) {
        cout << "You have already voted in this election.\n";
        return;
    }
    showCandidates(db, eid);
    cout << "Enter candidate id: ";
    int cid;
    cin >> cid;
    const char *voteSql = "INSERT INTO votes (user_id, election_id, candidate_id) VALUES (?, ?, ?);";
    sqlite3_prepare_v2(db, voteSql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, eid);
    sqlite3_bind_int(stmt, 3, cid);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        sqlite3_stmt *stmt2;
        const char *incSql = "UPDATE candidates SET votes = votes + 1 WHERE id = ?;";
        sqlite3_prepare_v2(db, incSql, -1, &stmt2, 0);
        sqlite3_bind_int(stmt2, 1, cid);
        sqlite3_step(stmt2);
        sqlite3_finalize(stmt2);
        cout << "Vote cast successfully.\n";
    } else {
        cout << "Failed to cast vote.\n";
    }
    sqlite3_finalize(stmt);
}

void viewResults(sqlite3 *db) {
    showElections(db);
    cout << "Enter election id: ";
    int eid;
    cin >> eid;
    sqlite3_stmt *stmt;
    const char *statusSql = "SELECT status, title FROM elections WHERE id = ?;";
    sqlite3_prepare_v2(db, statusSql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, eid);
    int status = 0;
    std::string title;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        status = sqlite3_column_int(stmt, 0);
        title = (const char*)sqlite3_column_text(stmt, 1);
    }
    sqlite3_finalize(stmt);
    if (status) {
        cout << "Election is still active. Results will be available after it ends.\n";
        return;
    }
    cout << "Results for: " << title << "\n";
    showCandidates(db, eid);
}
