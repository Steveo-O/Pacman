#pragma once
#include <iostream>
#include <windows.h>
#include "global.hpp"
#include "game.hpp"

using namespace std;

void check_direction();
void check_consume_dot(short x, short y);
void draw_player_position();
bool next_position();

/*
    Check which key is pressed
    Set the player direction state 
*/ 
void check_direction()
{
    if (kbhit()) {
        int const ch = getch();
        switch(ch) {
            case key_up:
                player_state = DIRECTION::Upward;
                pacman.start = true;
                break;
            case key_down:
                player_state = DIRECTION::Downward;
                pacman.start = true;
                break;
            case key_left:
                player_state = DIRECTION::Left;
                pacman.start = true;
                break;
            case key_right:
                player_state = DIRECTION::Right;
                pacman.start = true;
                break;
        }

    }
}

void check_consume_dot(short x, short y) {
    if(map[y][x] == (char)MAP::Dot) {
        map[y][x] = ' ';
        pacman.score++;  // Increment the score
    }
}

void draw_player_position() {
    switch(player_state) {
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

/* From the given direction state,
 * change the player position according to the direction state
 *
 * NOTE: the reason for moving 2 unit instead of 1 is the map grid is 2x2
 */
bool next_position() {
    switch(player_state) {
        case Upward:
            if (!check_obstacles(player.x, player.y - 2)){
                delete_old_position(player.x, player.y);
                player.y -= 2;
            }
            break;
        case Downward:
            if (!check_obstacles(player.x, player.y + 2)){
                delete_old_position(player.x, player.y);
                player.y += 2;
            }
            break;
        case Left:
            if (!check_obstacles(player.x - 2, player.y)){
                delete_old_position(player.x, player.y);
                player.x -= 2;
            }
            break;
        case Right:
            if (!check_obstacles(player.x + 2, player.y)){
                delete_old_position(player.x, player.y);
                player.x += 2;
            }
            break;
        case Still:
            return false;
    }
    return true;
}
