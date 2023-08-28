#include <iostream>
#include <windows.h>
#include <conio.h>
#include "global.hpp"
#include "game.hpp"
#include "player.hpp"
#include "map.hpp"
#include "enemy.hpp"

using namespace std;


int main() {
    int i = 0;
    cover();
    ShowMap();
    enemy.x = 19;
    enemy.y = 9;
    CursorPosition(enemy.x, enemy.y);
    cout << "E";
    state = DIRECTION::Still;
    enemy_previous_state = DIRECTION::Upward;
    pacman.status = GAMESTATE::running;
    while(pacman.status == GAMESTATE::running) {
        check_direction();
        if(next_position()) {
            draw_player_position();          
        }
        if(i % 2 && start) {
            next_enemy_position();
            check_enemy_state();
            draw_enemy_position();
        }
        check_game_condition();
        Sleep(100);
        i++;
    }  
    switch(pacman.status) {
        case lose:
            system("CLS");
            cout << "You lose!\n";
            system("pause");
            break;
    }
}