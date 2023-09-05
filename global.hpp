#pragma once
#include <chrono>
#include <string>
#include <vector>

#include <windows.h>

#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 40

enum Gamestate {
    Running = 0,
    Win,
    Lose,
};

enum Direction {
    Still = 0,
    Upward,
    Downward,
    Left,
    Right,
};

/*
 * Reference: https://www.ascii-code.com/CP437
 */
enum Map {
    Dot = 176u, // middle dot
    Wall = 219u, // full block
};

struct Position {
    short x;
    short y;
    short old_x;
    short old_y;
};

struct Pacman {
    // Console related data
    HANDLE hConsole;

    Gamestate status;
    bool start = false;
    std::string map_choice;

    // Track score
    int max_score;
    int score;

    // Track game time
    std::chrono::time_point<std::chrono::steady_clock> time_start;
    int hours, minutes, seconds;
};

struct Player {
    std::string player_name;
    int highscore;
    int duration;
};

/*
 * Global game state
 */
Pacman pacman;

/*
 * Player's state
 */
Position player;
Direction player_state;

/*
 * Enemy's state
 */
Position enemy;
Direction enemy_state, enemy_previous_state;

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
