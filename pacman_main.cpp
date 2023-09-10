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
    // HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // _SMALL_RECT rectWindow = {0, 0, 90, 20};
    // SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);
    // SetConsoleScreenBufferSize(hConsole, {(short)100,(short)40});
    // SetConsoleActiveScreenBuffer(hConsole);
    main_menu();

    // This is to mitigate GetAsyncKeyState scanned
    // previous input key
    CursorPosition(20, 20);
    cout << "Press any key to start"<< endl;
    while(!kbhit()) {}

    CursorPosition(0, 0);
    InitMap();

    InitializeTotalDots();
    enemy_previous_state = DIRECTION::Upward; // Initiliaze enemy direction upward
    pacman.status = GAMESTATE::running;
    time_start = std::chrono:: steady_clock::now();
    while(pacman.status == GAMESTATE::running) {
        CursorPosition(81, 6);
        count_time();
        check_direction();
        if(next_position()) {
            draw_player_position();
            CursorPosition(81, 5);
            cout << "score: " << score;
        }
        if(i % 2 && start) {
            next_enemy_position();
            check_enemy_state();
            draw_enemy_position();
            check_reset_enemy();
        }
        check_game_condition();
        Sleep(100);
        i++;
    } 
    record_player_rank(); 

    switch(pacman.status) {
        case lose:
            system("CLS");
            cout << "You lose!\n";
            cout << "Total score: " << score << "\n";
            cout << "Total used time: " << hours << ":" << minutes << ":" << seconds << endl;
            system("pause");
            break;
        case win:
            system("CLS");
            cout << "You lose!\n";
            cout << "Total score: " << score << "\n";
            cout << "Total used time: " << hours << ":" << minutes << ":" << seconds << endl;
            system("pause");
    }
}
