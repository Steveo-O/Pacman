#pragma once
#include <string>
#include <chrono>
#include <vector>

enum GAMESTATE {
    running = 0,
    win,
    lose,
};

enum DIRECTION {
    Still = 0,
    Upward, 
    Downward, 
    Left, 
    Right
};

enum MAP {
    Dot = '.',
    Wall = '#'
};

struct POSITION{
    short x;
    short y;
};
struct PACMAN{
    GAMESTATE status;
    int score, max_score;
    bool start = false;
}pacman;

int map_num, hours, minutes, seconds;

struct Player{
    std::string player_name;
    int highscore;
    int duration;
};

std::string name;
std::string map_choice;
std::chrono::time_point<std::chrono::steady_clock> time_start;

/*
 * Player's state
 */
POSITION player;
DIRECTION player_state;

/*
 * Enemy's state
 */
POSITION enemy;
DIRECTION enemy_state, enemy_previous_state;

/*
 * data storage path
 * All data that needs to be stored are
 * located at `%APPDATA%\pacman_game`
 *
 * This location is at:
 * C:\Users\<username>\AppData\Roaming\pacman_game\
 */
std::string appdata = getenv("APPDATA");
std::string pacman_folder = appdata + "\\pacman_game";

/*
 * shorthand for keycodes
 */
const int key_up = 72;
const int key_down = 80;
const int key_left = 75;
const int key_right = 77;
const int key_enter = 13;
