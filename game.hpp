#pragma once
#include <iostream>
#include <windows.h>
#include "global.hpp"
#include "map.hpp"

using namespace std;

// Move the Cursor to the given position
void CursorPosition(short x, short y) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);  
    COORD position = {(short)x, (short)y};  

    // Blinking cursor is annoying!!!
    // Make the cursor invisible
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hStdout, &cursorInfo);

    SetConsoleCursorPosition(hStdout, position); 
} 

void delete_old_position(short x, short y) {
        CursorPosition(x, y);
        cout << " ";
}

bool check_obstacles(short x, short y) {
    return map[y][x] == '$' || map[y][x] == '|';
}
