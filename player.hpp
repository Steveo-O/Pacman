#pragma once
#include <iostream>
#include <windows.h>
#include "global.hpp"
#include "game.hpp"

using namespace std;

/*
    Check which key is pressed
    Set the player direction state 
*/ 
void check_direction() {
    const int key_up = 72;
    const int key_down = 80;
    const int key_left = 75;
    const int key_right = 77;

    if (kbhit()) {
        int const ch = getch();
        switch(ch) {
            case key_up:
                state = DIRECTION::Upward;
                start = true;
                break;
            case key_down:
                state = DIRECTION::Downward;
                start = true;
                break;
            case key_left:
                state = DIRECTION::Left;
                start = true;
                break;
            case key_right:
                state = DIRECTION::Right;
                start = true;
                break;
        }

    }
}

void check_consume_dot(short x, short y) {
    if(map[y][x] == (char)MAP::Dot) {
        map[y][x] = ' ';
        score++;  // Increment the score
    }
}

void draw_player_position() {
    switch(state) {
        case Upward:
            CursorPosition(player.x, player.y);
            cout << '/';
            CursorPosition(player.x + 1, player.y);
            cout << '\\';
            CursorPosition(player.x, player.y + 1);
            cout << ' ';
            CursorPosition(player.x + 1, player.y + 1);
            cout << ' ';
            break;
        case Downward:
            CursorPosition(player.x, player.y);
            cout << ' ';
            CursorPosition(player.x + 1, player.y);
            cout << ' ';
            CursorPosition(player.x, player.y + 1);
            cout << '\\';
            CursorPosition(player.x + 1, player.y + 1);
            cout << '/';
            break;
        case Left:
            CursorPosition(player.x, player.y);
            cout << '/';
            CursorPosition(player.x + 1, player.y);
            cout << ' ';
            CursorPosition(player.x, player.y + 1);
            cout << '\\';
            CursorPosition(player.x + 1, player.y + 1);
            cout << ' ';
            break;
        case Right:
            CursorPosition(player.x, player.y);
            cout << ' ';
            CursorPosition(player.x + 1, player.y);
            cout << '\\';
            CursorPosition(player.x, player.y + 1);
            cout << ' ';
            CursorPosition(player.x + 1, player.y + 1);
            cout << '/';
            break;
        case Still:
            break;
    }
    check_consume_dot(player.x, player.y);
}

// From the given direction state, change the player position according to the direction state
bool next_position() {
    switch(state) {
        case Upward:
            if (!check_obstacles(player.x, player.y - 2)){
                delete_old_position(player.x, player.y);
                player.y -= 2;
                return true;
            }
            break;
        case Downward:
            if (!check_obstacles(player.x, player.y + 2)){
                delete_old_position(player.x, player.y);
                player.y += 2;
                return true;
            }
            break;
        case Left:
            if (!check_obstacles(player.x - 2, player.y)){
                delete_old_position(player.x, player.y);
                player.x -= 2;
                return true;
            }
            break;
        case Right:
            if (!check_obstacles(player.x + 2, player.y)){
                delete_old_position(player.x, player.y);
                player.x += 2;
                return true;
            }
            break;
    }
    return false;
}
