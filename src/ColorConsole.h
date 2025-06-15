#ifndef COLORCONSOLE_H
#define COLORCONSOLE_H

#include <iostream>
#include <string>
#include <windows.h>

class ColorConsole {
public:
    enum class Color {
        BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, RED = 4, MAGENTA = 5,
        BROWN = 6, LIGHTGRAY = 7, DARKGRAY = 8, LIGHTBLUE = 9,
        LIGHTGREEN = 10, LIGHTCYAN = 11, LIGHTRED = 12, 
        LIGHTMAGENTA = 13, YELLOW = 14, WHITE = 15
    };

    // Core color functions
    static void setTextColor(Color color);
    static void resetColor();
    static void printColored(const std::string& text, Color color);
    
    // Alignment and styling functions
    static void centerColoredText(const std::string& text, Color color);
    static void coloredDivider(char symbol, Color color);
    static void coloredDivider(char symbol, Color color, int width);
    
    // Utility functions
    static int getConsoleWidth();
    static void clearScreen();
    static void pauseScreen();
    
    // Menu styling helpers
    static void printHeader(const std::string& title);
    static void printMenuOption(const std::string& option, Color color = Color::LIGHTGREEN);
    static void printSeparator();
    static void printSuccess(const std::string& message);
    static void printError(const std::string& message);
    static void printWarning(const std::string& message);
};

#endif // COLORCONSOLE_H
