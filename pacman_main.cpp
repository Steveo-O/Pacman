#include <iostream>
#include <windows.h>
#include <conio.h>
#include "global.hpp"
#include "game.hpp"
#include "player.hpp"
#include "map.hpp"
#include "enemy.hpp"

using namespace std;

void reset_font() {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 10;
    cfi.dwFontSize.Y = 16;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Lucida Console");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &cfi);
}

int main() {
    int i = 0;

    /*
     * To maximize the window
     */
    system("mode 650");
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

    while(1) {
        main_menu();
        /*
        * Because we are rendering the player cursor (<,>,^,v) in 4 grids,
        * to make it looks same as the symbol in one grid we have to:
        *  - set the cell size (the character size) to a square, in this case it's 16x16
        *  - find a font that it's character / and \ are close to each other, and
        *    we have found that `Lucida Console` played nicely
        */
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 12;
        cfi.dwFontSize.Y = 12;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcscpy_s(cfi.FaceName, L"Lucida Console");
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &cfi);
        CursorPosition(20, 20);
        
        cout << "Press any key to start"<< endl;
        while(!kbhit()) {}

        CursorPosition(0, 0);
        InitMap();
       
        pacman.max_score = 0;
        InitializeTotalDots();
        
        player_state = DIRECTION::Still;
        enemy_previous_state = DIRECTION::Upward; // Initiliaze enemy direction 
        pacman.status = GAMESTATE::running;
        pacman.start = false;
        pacman.score = 0;
        time_start = std::chrono:: steady_clock::now();

        while(pacman.status == GAMESTATE::running) {
            CursorPosition(81, 6);
            if(pacman.start)
                count_time();
            check_direction();
            if(next_position()) {
                draw_player_position();
                CursorPosition(81, 5);
                cout << "score: " << pacman.score;
            }
            check_game_condition();

            if(pacman.status != GAMESTATE::running)
                break;

            if(i % 2 && pacman.start) {
                next_enemy_position();
                check_enemy_state();
                draw_enemy_position();
            }
            check_game_condition();
            Sleep(100);
            i++;
        } 
        record_player_rank(); 
        switch(pacman.status) {
            case lose:
                system("CLS");
                reset_font();
                cout << "You lose!\n";
                cout << "Total score: " << pacman.score << "\n";
                cout << "Total used time: " << hours << ":" << minutes << ":" << seconds << endl;
                system("pause");
                break;
            case win:
                system("CLS");
                reset_font();
                cout << "You Win!\n";
                cout << "Total score: " << pacman.score << "\n";
                cout << "Total used time: " << hours << ":" << minutes << ":" << seconds << endl;
                system("pause");
                break;
            default:
                break;
        }
    }  
}
