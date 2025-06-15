# 🗳️ Voting Management System

A comprehensive **C++ console-based voting management system** with a colorful and user-friendly interface. This system supports both local and national elections with secure user authentication and real-time vote counting.

## ✨ Features

### 🎨 **Modern Console Interface**
- **Colorful UI** with custom ColorConsole class
- **Centered text alignment** for professional appearance
- **Color-coded menus** and status messages
- **Interactive dividers** and headers

### 👥 **User Management**
- **Dual role system**: Voters and Administrators
- **Secure registration** with CNIC validation
- **Login authentication** with role-based access
- **User data persistence** with SQLite database

### 🗳️ **Election Management**
- **Multiple election types**: Local and National
- **Real-time election status** (Active/Inactive)
- **Candidate management** with party affiliation
- **Vote counting** and results display

### 🔒 **Security Features**
- **Role-based access control**
- **Duplicate voting prevention**
- **CNIC-based voter identification**
- **Secure database operations**

## 🛠️ Technology Stack

- **Language**: C++11
- **Database**: SQLite3
- **UI**: Custom ColorConsole class with Windows Console API
- **Build System**: MinGW/GCC
- **Platform**: Windows

## 📁 Project Structure

```
Voting-management-system/
├── src/                        # Source code files
│   ├── ColorConsole.h          # Color console interface header
│   ├── ColorConsole.cpp        # Color console implementation
│   ├── Main.cpp                # Main application entry point
│   ├── db_utils.h              # Database utilities header
│   ├── db_utils.cpp            # Database operations
│   ├── Administrator.h         # Admin class header
│   ├── Administartor.cpp       # Admin functionality
│   ├── Voter.h                 # Voter class header
│   ├── Voter.cpp               # Voter functionality
│   ├── Election.h              # Election base class
│   ├── Election.cpp            # Election implementation
│   ├── LocalElection.h         # Local election class
│   ├── LocalElection.cpp       # Local election implementation
│   ├── NationalElection.h      # National election class
│   ├── NationalElection.cpp    # National election implementation
│   ├── Candidate.h             # Candidate class header
│   ├── Candidate.cpp           # Candidate implementation
│   ├── User.h                  # User base class
│   ├── User.cpp                # User implementation
│   ├── fileHandler.h           # File handling utilities
│   ├── fileHandler.cpp         # File operations
│   ├── sqlite3.h               # SQLite header
│   ├── sqlite3.dll             # SQLite dynamic library
│   └── sqlite3.def             # SQLite definition file
├── data/                       # Database and data files
│   └── voting.db               # SQLite database (auto-generated)
├── build.bat                   # Build script
├── README.md                   # Project documentation
├── LICENSE                     # License file
└── .gitignore                  # Git ignore file
```

## 🚀 Getting Started

### Prerequisites

- **MinGW/GCC compiler** with C++11 support
- **Windows operating system**
- **Git** (for cloning the repository)

### Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/Voting-management-system.git
   cd Voting-management-system
   ```

2. **Build the project**:
   ```bash
   build.bat
   ```

3. **Run the application**:
   ```bash
   main.exe
   ```

### Manual Build (Alternative)

```bash
cd src
g++ -std=c++11 -o ../main.exe *.cpp sqlite3.dll -static-libgcc -static-libstdc++
cd ..
```

## 📖 Usage Guide

### 🏠 **Main Menu**
The application starts with a colorful main menu offering:
1. **Login as Voter** - Access voting functionality
2. **Login as Admin** - Access administrative features
3. **Register Voter** - Create new voter account
4. **Exit** - Close the application

### 👤 **Voter Functions**
- **View Elections**: See all available elections
- **Vote**: Cast votes in active elections
- **View Results**: Check election results
- **Logout**: Return to main menu

### 👨‍💼 **Administrator Functions**
- **Create Election**: Set up new local or national elections
- **Add Candidate**: Register candidates for elections
- **Start Election**: Activate elections for voting
- **End Election**: Close elections and finalize results
- **View Results**: Monitor election outcomes
- **Logout**: Return to main menu

## 🎨 ColorConsole Features

The project includes a custom **ColorConsole** class that provides:

### Color Definitions
```cpp
enum class Color {
    BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, RED = 4, MAGENTA = 5,
    BROWN = 6, LIGHTGRAY = 7, DARKGRAY = 8, LIGHTBLUE = 9,
    LIGHTGREEN = 10, LIGHTCYAN = 11, LIGHTRED = 12, 
    LIGHTMAGENTA = 13, YELLOW = 14, WHITE = 15
};
```

### Key Functions
- `setTextColor(Color color)` - Set text color
- `centerColoredText(text, color)` - Center text with color
- `coloredDivider(symbol, color)` - Create decorative dividers
- `printHeader(title)` - Display formatted headers
- `printSuccess/Error/Warning(message)` - Status messages

### UI Color Scheme
- **Headers/Titles**: YELLOW
- **Dividers**: LIGHTCYAN
- **Menu Options**: LIGHTGREEN, LIGHTMAGENTA, WHITE
- **Success Messages**: LIGHTGREEN
- **Error Messages**: LIGHTRED
- **Warnings**: YELLOW

## 🗄️ Database Schema

The system uses SQLite with the following tables:

### Users Table
```sql
CREATE TABLE users (
    id INTEGER PRIMARY KEY,
    username TEXT UNIQUE,    -- CNIC number
    password TEXT,
    name TEXT,
    age INTEGER,
    region TEXT,
    role TEXT               -- 'voter' or 'admin'
);
```

### Elections Table
```sql
CREATE TABLE elections (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    type TEXT,              -- 'local' or 'national'
    title TEXT,
    status INTEGER,         -- 0: Inactive, 1: Active
    region TEXT
);
```

### Candidates Table
```sql
CREATE TABLE candidates (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    election_id INTEGER,
    name TEXT,
    party TEXT,
    votes INTEGER DEFAULT 0,
    FOREIGN KEY(election_id) REFERENCES elections(id)
);
```

## 🤝 Contributing

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/new-feature`
3. **Commit changes**: `git commit -am 'Add new feature'`
4. **Push to branch**: `git push origin feature/new-feature`
5. **Submit a Pull Request**

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author
Muhammad TAhir
- GitHub: https://github.com/Tahir-CS
- Email: mtahirbutt1005@gmail.com

## 🙏 Acknowledgments

- SQLite for database functionality
- Windows Console API for color support
- C++ community for best practices

## 📸 Screenshots

*Add screenshots of your colorful console interface here*

## 🔮 Future Enhancements

- [ ] Add vote encryption
- [ ] Implement vote audit trail
- [ ] Add candidate photo support
- [ ] Create web-based interface
- [ ] Add multi-language support
- [ ] Implement advanced reporting

---

⭐ **Star this repository if you found it helpful!**