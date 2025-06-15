#include "ColorConsole.h"
#include <iostream>
#include <windows.h>

void ColorConsole::setTextColor(Color color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<int>(color));
}

void ColorConsole::resetColor() {
    setTextColor(Color::WHITE);
}

void ColorConsole::printColored(const std::string& text, Color color) {
    setTextColor(color);
    std::cout << text;
    resetColor();
}

void ColorConsole::centerColoredText(const std::string& text, Color color) {
    int consoleWidth = getConsoleWidth();
    int padding = (consoleWidth - text.length()) / 2;
    if (padding > 0) {
        std::cout << std::string(padding, ' ');
    }
    printColored(text, color);
    std::cout << std::endl;
}

void ColorConsole::coloredDivider(char symbol, Color color) {
    int width = getConsoleWidth();
    coloredDivider(symbol, color, width);
}

void ColorConsole::coloredDivider(char symbol, Color color, int width) {
    setTextColor(color);
    std::cout << std::string(width, symbol) << std::endl;
    resetColor();
}

int ColorConsole::getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 80; // Default width if unable to get console info
}

void ColorConsole::clearScreen() {
    system("cls");
}

void ColorConsole::pauseScreen() {
    std::cout << "\nPress any key to continue...";
    std::cin.ignore();
    std::cin.get();
}

void ColorConsole::printHeader(const std::string& title) {
    std::cout << std::endl;
    coloredDivider('=', Color::LIGHTCYAN);
    centerColoredText(title, Color::YELLOW);
    coloredDivider('=', Color::LIGHTCYAN);
    std::cout << std::endl;
}

void ColorConsole::printMenuOption(const std::string& option, Color color) {
    centerColoredText(option, color);
}

void ColorConsole::printSeparator() {
    coloredDivider('-', Color::LIGHTCYAN);
}

void ColorConsole::printSuccess(const std::string& message) {
    std::cout << std::endl;
    centerColoredText(message, Color::LIGHTGREEN);
    std::cout << std::endl;
}

void ColorConsole::printError(const std::string& message) {
    std::cout << std::endl;
    centerColoredText(message, Color::LIGHTRED);
    std::cout << std::endl;
}

void ColorConsole::printWarning(const std::string& message) {
    std::cout << std::endl;
    centerColoredText(message, Color::YELLOW);
    std::cout << std::endl;
}
