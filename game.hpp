#pragma once
#include <iostream>
#include <windows.h>
#include <cmath>
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
    return map[y][x] == '#' || map[y][x] == '|';
}

void InitializeTotalDots() {
    // Count the initial number of dots
    for (int row = 0; row < 20; row++) {
        for (int column = 0; column < 80; column++) {
            if (map[row][column] == '.') 
                totalDots++;
        }
    }
}

void check_game_condition() { 
    int distance, distance_x, distance_y;
    distance_x = player.x - enemy.x;
    distance_y = player.y - enemy.y;
    distance = sqrt(distance_x * distance_x + distance_y * distance_y);
    
    if(distance == 0) 
        pacman.status = GAMESTATE::lose;
    else if(score >= totalDots)
        pacman.status = GAMESTATE::win;
}

void cover() {
    int x, y;
    x = 40;
    y = 8;
    cout <<
    "#######        #           #######     ###     ###          #         #       #\n" <<
    "#      #      # #         #            #  #   #  #         # #        # #     #\n" <<
    "#      #     #   #       #             #   # #   #        #   #       #  #    #\n" <<
    "######      #     #     #              #    #    #       #     #      #   #   #\n" <<
    "#          # ##### #     #             #         #      # ##### #     #    #  #\n" <<
    "#         #         #     #            #         #     #         #    #     # #\n" <<
    "#        #           #     #######     #         #    #           #   #       #\n";
    CursorPosition(x, y);
    cout << "start\n";
    cout << endl;
    CursorPosition(x, y + 2);
    cout << "quit";
    CursorPosition(x - 1, y);
    cout << "<";
    while(1) {

        if(GetAsyncKeyState(VK_DOWN) && y + 2 < 11) {
            delete_old_position(x - 1, y);
            y = y + 2;
        }
        if(GetAsyncKeyState(VK_UP) && y - 2 > 7) {
            delete_old_position(x - 1, y);
            y = y - 2;
        }
        CursorPosition(x - 1, y);
        cout << ">";
        if(GetAsyncKeyState(VK_RETURN)) {
            switch(y) {
                case 8:
                    system("CLS");
                    break;
                case 10:
                    system("Pause");
                    exit(0);
                    break;
            }
            break;
        }
    }
}