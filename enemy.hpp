#pragma once
#include <iostream>
#include <windows.h>
#include <cmath>
#include <chrono>

#include "global.hpp"
#include "game.hpp"

using namespace std;

void check_enemy_state();
void draw_enemy_position();
void next_enemy_position();

/*
             b
             ^
        a <     > c
             v
             d
*/

float distance_a, distance_b, distance_c, distance_d;
float result_x, result_y;

/*
 * NOTE: move direction in 2 unit as expanded grid is 2x2
*/

void next_enemy_position() {
    distance_a = 1000;
    distance_b = 1000;
    distance_c = 1000;
    distance_d = 1000;
    switch(enemy_previous_state) {
        case Upward:
            // Move Upward
            if(!check_obstacles(enemy.x, enemy.y - 2)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y - 2);
                distance_b = sqrt(result_x * result_x + result_y * result_y);
            }
            // Move left
            if(!check_obstacles(enemy.x - 2, enemy.y)) {
                result_x = player.x - (enemy.x - 2);
                result_y = player.y - enemy.y;
                distance_a = sqrt(result_x * result_x + result_y * result_y);
            }
            // Move right
            if(!check_obstacles(enemy.x + 2, enemy.y)) {
                result_x = player.x - (enemy.x + 2);
                result_y = player.y - enemy.y;
                distance_c = sqrt(result_x * result_x + result_y * result_y);
            }
            break;
        case Downward:
            // Move Downward
            if(!check_obstacles(enemy.x, enemy.y + 2)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y + 2);
                distance_d = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x - 2, enemy.y)) {
                result_x = player.x - (enemy.x - 2);
                result_y = player.y - enemy.y;
                distance_a = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x + 2, enemy.y)) {
                result_x = player.x - (enemy.x + 2);
                result_y = player.y - enemy.y;
                distance_c = sqrt(result_x * result_x + result_y * result_y);
            }
            break;
        case Left:
            if(!check_obstacles(enemy.x, enemy.y - 2)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y - 2);
                distance_b = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x, enemy.y + 2)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y + 2);
                distance_d = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x - 2, enemy.y)) {
                result_x = player.x - (enemy.x - 2);
                result_y = player.y - enemy.y;
                distance_a = sqrt(result_x * result_x + result_y * result_y);
            }
            break;
        case Right:
            if(!check_obstacles(enemy.x, enemy.y - 2)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y - 2);
                distance_b = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x, enemy.y + 2)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y + 2);
                distance_d = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x + 2, enemy.y)) {
                result_x = player.x - (enemy.x + 2);
                result_y = player.y - enemy.y;
                distance_c = sqrt(result_x * result_x + result_y * result_y);
            }
            break;
        case Still:
            break;
    }
}   

void check_enemy_state() {
    float smallest_num[] = {distance_a, distance_b, distance_c, distance_d};   
    for(int i = 1; i < 4; i++) {
        if(smallest_num[0] > smallest_num[i])
            smallest_num[0] = smallest_num[i];
    }

    if(smallest_num[0] == distance_a) {
        enemy_state = DIRECTION::Left;
    }
    if(smallest_num[0] == distance_b) {
        enemy_state = DIRECTION::Upward;
    }
    if(smallest_num[0] == distance_c) {
        enemy_state = DIRECTION::Right;
    }
    if(smallest_num[0] == distance_d) {
        enemy_state = DIRECTION::Downward;
    }
}

void draw_enemy_position() {
    enemy_previous_state = enemy_state;
    if(map[enemy.y][enemy.x] == (char)MAP::Dot) {
        CursorPosition(enemy.x, enemy.y);
        cout << (char)MAP::Dot;
        CursorPosition(enemy.x + 1, enemy.y);
        cout << (char)MAP::Dot;
        CursorPosition(enemy.x, enemy.y + 1);
        cout << (char)MAP::Dot;
        CursorPosition(enemy.x + 1, enemy.y + 1);
        cout << (char)MAP::Dot;
    }
    else
        delete_old_position(enemy.x, enemy.y);
    switch(enemy_state) {
        case Upward:
            CursorPosition(enemy.x, enemy.y - 2);
            enemy.y -= 2;
            break;
        case Downward:
            CursorPosition(enemy.x, enemy.y + 2);
            enemy.y += 2;
            break;
        case Left:
            CursorPosition(enemy.x - 2, enemy.y);
            enemy.x -= 2;
            break;
        case Right:
            CursorPosition(enemy.x + 2, enemy.y);
            enemy.x += 2;
            break;
        case Still:
            break;
    }
    /*
     * This is the enemy appearance
     *        ··
     *        ══
     */
    CursorPosition(enemy.x, enemy.y);
    cout << '^';
    CursorPosition(enemy.x + 1, enemy.y);
    cout << '^';
    CursorPosition(enemy.x, enemy.y + 1);
    cout << '=';
    CursorPosition(enemy.x + 1, enemy.y + 1);
    cout << '=';
}
