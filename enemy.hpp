#pragma once
#include <iostream>
#include <windows.h>
#include <cmath>
#include <chrono>
#include "global.hpp"
#include "game.hpp"

using namespace std;

/*
             b
             ^ 
        a <     > c
             v
             d
*/

void next_enemy_position() {
    distance_a = 1000;
    distance_b = 1000;
    distance_c = 1000;
    distance_d = 1000;
    switch(enemy_previous_state) {
        case Upward:
            // Move Upward
            if(!check_obstacles(enemy.x, enemy.y - 1)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y - 1);
                distance_b = sqrt(result_x * result_x + result_y * result_y);
            }
            // Move left
            if(!check_obstacles(enemy.x - 1, enemy.y)) {
                result_x = player.x - (enemy.x - 1);
                result_y = player.y - enemy.y;
                distance_a = sqrt(result_x * result_x + result_y * result_y);
            }
            // Move right
            if(!check_obstacles(enemy.x + 1, enemy.y)) {
                result_x = player.x - (enemy.x + 1);
                result_y = player.y - enemy.y;
                distance_c = sqrt(result_x * result_x + result_y * result_y);
            }
            break;
        case Downward:
            // Move Downward
            if(!check_obstacles(enemy.x, enemy.y + 1)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y + 1);
                distance_d = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x - 1, enemy.y)) {
                result_x = player.x - (enemy.x - 1);
                result_y = player.y - enemy.y;
                distance_a = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x + 1, enemy.y)) {
                result_x = player.x - (enemy.x + 1);
                result_y = player.y - enemy.y;
                distance_c = sqrt(result_x * result_x + result_y * result_y);
            }
            break;
        case Left:
            if(!check_obstacles(enemy.x, enemy.y - 1)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y - 1);
                distance_b = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x, enemy.y + 1)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y + 1);
                distance_d = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x - 1, enemy.y)) {
                result_x = player.x - (enemy.x - 1);
                result_y = player.y - enemy.y;
                distance_a = sqrt(result_x * result_x + result_y * result_y);
            }
            break;
        case Right:
            if(!check_obstacles(enemy.x, enemy.y - 1)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y - 1);
                distance_b = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x, enemy.y + 1)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y + 1);
                distance_d = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x + 1, enemy.y)) {
                result_x = player.x - (enemy.x + 1);
                result_y = player.y - enemy.y;
                distance_c = sqrt(result_x * result_x + result_y * result_y);
            }
        case Still:
            if(!check_obstacles(enemy.x, enemy.y - 1)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y - 1);
                distance_b = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x, enemy.y + 1)) {
                result_x = player.x - enemy.x;
                result_y = player.y - (enemy.y + 1);
                distance_d = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x + 1, enemy.y)) {
                result_x = player.x - (enemy.x + 1);
                result_y = player.y - enemy.y;
                distance_c = sqrt(result_x * result_x + result_y * result_y);
            }
            if(!check_obstacles(enemy.x - 1, enemy.y)) {
                result_x = player.x - (enemy.x - 1);
                result_y = player.y - enemy.y;
                distance_a = sqrt(result_x * result_x + result_y * result_y);
            }
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

void check_reset_enemy() {
    int distance_x, distance_y, time;
    distance_x = player.x - enemy.x;
    distance_y = player.y - enemy.y;
    auto start = chrono::system_clock::now();

    auto end = chrono::system_clock::now();

    auto diff = end - start;

    time = chrono::duration_cast <chrono::seconds> (diff).count();
    switch(enemy_state) {
        case(Left):
            if(distance_x > 0 && time == 2) {
                enemy_previous_state = DIRECTION::Still;
            }
            break;
        case(Right):
            if(distance_x < 0 && time == 2) {
                enemy_previous_state = DIRECTION::Still;
            }
            break;
        case(Upward):
            if(distance_y > 0 && time == 2) {
                enemy_previous_state = DIRECTION::Still;
            }
            break;
        case(Downward):
            if(distance_y < 0 && time == 2) {
                enemy_previous_state = DIRECTION::Still;
            }
            break;
            
    }
}

void draw_enemy_position() {
    enemy_previous_state = enemy_state;
    if(map[enemy.y][enemy.x] == '.') {
        CursorPosition(enemy.x, enemy.y); 
        cout << '.';
    }
    else
        delete_old_position(enemy.x, enemy.y);
    switch(enemy_state) {
        case Upward:
            CursorPosition(enemy.x, enemy.y - 1);
            enemy.y--;
            break;
        case Downward:
            CursorPosition(enemy.x, enemy.y + 1);
            enemy.y++;
            break;
        case Left:
            CursorPosition(enemy.x - 1, enemy.y);
            enemy.x--;
            break;
        case Right:
            CursorPosition(enemy.x + 1, enemy.y);
            enemy.x++;
            break;
    }
    cout << "E";
    
}