/*
    '$' && '|' are wall
    'X' is player
*/
#pragma once
enum GAMESTATE {
    running = 0,
    win, lose
};

enum DIRECTION {
    Still = 0,
    Upward, Downward, Left, Right
}state, enemy_state, enemy_previous_state;

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
int map_num, hours, minutes, seconds;