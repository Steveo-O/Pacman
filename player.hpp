#pragma once

#include <iostream>

#include <windows.h>

using namespace std;

void check_direction();
void check_consume_dot(short x, short y);
void draw_player_position();
bool next_position();

#include "global.hpp"
#include "game.hpp"

/*
 * Check which key is pressed
 * Set the player direction state
*/
void
check_direction()
{
    if (kbhit()) {
        int const ch = getch();
        switch(ch) {
            case key_up:
                player_state = Direction::Upward;
                pacman.start = true;
                break;
            case key_down:
                player_state = Direction::Downward;
                pacman.start = true;
                break;
            case key_left:
                player_state = Direction::Left;
                pacman.start = true;
                break;
            case key_right:
                player_state = Direction::Right;
                pacman.start = true;
                break;
        }

    }
}

void
check_consume_dot(short x, short y)
{
    if(map[y][x] == (char)Map::Dot) {
        map[y][x] = ' ';
        pacman.score++;  // Increment the score
    }
}

/*
 * Draw player in 4 grid
 *
 *      (player.x, player.y)
 *       v
 *      |x|x|
 *      ----
 *      |x|x|
 *
 */
void
draw_player_position()
{
    switch(player_state) {
        case Upward:
            /*
             *
             * |/|\|
             * ----
             * | | |
             */
            cursor_position(player.x, player.y);
            cout << '/';
            cursor_position(player.x + 1, player.y);
            cout << '\\';
            cursor_position(player.x, player.y + 1);
            cout << ' ';
            cursor_position(player.x + 1, player.y + 1);
            cout << ' ';
            break;
        case Downward:
            /*
             *
             * | | |
             * ----
             * |\|/|
             */
            cursor_position(player.x, player.y);
            cout << ' ';
            cursor_position(player.x + 1, player.y);
            cout << ' ';
            cursor_position(player.x, player.y + 1);
            cout << '\\';
            cursor_position(player.x + 1, player.y + 1);
            cout << '/';
            break;
        case Left:
            /*
             *
             * |/| |
             * ----
             * |\| |
             */
            cursor_position(player.x, player.y);
            cout << '/';
            cursor_position(player.x + 1, player.y);
            cout << ' ';
            cursor_position(player.x, player.y + 1);
            cout << '\\';
            cursor_position(player.x + 1, player.y + 1);
            cout << ' ';
            break;
        case Right:
            /*
             *
             * | |\|
             * ----
             * | |/|
             */
            cursor_position(player.x, player.y);
            cout << ' ';
            cursor_position(player.x + 1, player.y);
            cout << '\\';
            cursor_position(player.x, player.y + 1);
            cout << ' ';
            cursor_position(player.x + 1, player.y + 1);
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
bool
next_position()
{
    switch(player_state) {
        case Upward:
            delete_old_position(player.x, player.y);
            if (!check_obstacles(player.x, player.y - 2)){
                player.y -= 2;
            }
            break;
        case Downward:
            delete_old_position(player.x, player.y);
            if (!check_obstacles(player.x, player.y + 2)){
                player.y += 2;
            }
            break;
        case Left:
            delete_old_position(player.x, player.y);
            if (!check_obstacles(player.x - 2, player.y)){
                player.x -= 2;
            }
            break;
        case Right:
            delete_old_position(player.x, player.y);
            if (!check_obstacles(player.x + 2, player.y)){
                player.x += 2;
            }
            break;
        case Still:
            return false;
    }
    return true;
}
