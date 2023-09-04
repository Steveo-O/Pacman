/*
    '$' && '|' are wall
    'X' is player
*/
#pragma once
#include <string>
#include <chrono>

enum GAMESTATE {
    running = 0,
    win, lose
};

enum DIRECTION {
    Still = 0,
    Upward, Downward, Left, Right
}state, enemy_state, enemy_previous_state;

// Reference: https://www.ascii-code.com/CP437
enum MAP {
    Dot = 176u, // Middle dot
    Enemy = 239u, // Intersection
    Wall = 219u, // Full block
};

/*
    To access the member of struct
    Use "."
    Example: player.x
*/
struct POSITION{
    short x;
    short y;
    short old_x;
    short old_y;
}player, enemy;
struct PACMAN{
    GAMESTATE status;
    int score, max_score;
}pacman;

float distance_a, distance_b, distance_c, distance_d;
float result_x, result_y;
bool start = false;
int score = 0;
int totalDots = 0;
int hours, minutes, seconds;
std::string map_choice;
std::chrono::time_point<std::chrono::steady_clock> time_start;
