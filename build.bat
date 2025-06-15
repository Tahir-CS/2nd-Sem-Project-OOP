@echo off
REM Build script for Voting-management-system using g++ (MinGW)
cd src

g++ -std=c++11 -o ..\main.exe *.cpp sqlite3.dll -static-libgcc -static-libstdc++

cd ..
echo Build complete. main.exe created.
