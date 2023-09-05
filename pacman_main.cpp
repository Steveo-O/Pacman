#include <iostream>

#include <conio.h>
#include <windows.h>

#include "global.hpp"
#include "enemy.hpp"
#include "game.hpp"
#include "map.hpp"
#include "player.hpp"

using namespace std;

void
setup()
{
    pacman.hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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
    cfi.dwFontSize.X = 16;
    cfi.dwFontSize.Y = 16;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Lucida Console");
    SetCurrentConsoleFontEx(pacman.hConsole, false, &cfi);

    // Set the window size
    _SMALL_RECT rectWindow = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SetConsoleWindowInfo(pacman.hConsole, true, &rectWindow);

    /*
     * Blinking cursor is annoying!!!
     * Make the cursor invisible
     */
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(pacman.hConsole, &cursorInfo);

    main_menu();

    string message = "[Press any key to start]";
    cursor_position((SCREEN_WIDTH - message.length()) / 2, SCREEN_HEIGHT / 2);
    cout << message;
    while(!kbhit()) {}

    cursor_position(0, 0);
    init_map();
    init_total_dots();
    // Initialize enemy direction upward
    enemy_previous_state = Direction::Upward;

}

void
run()
{
    pacman.status = Gamestate::Running;
    pacman.time_start = std::chrono:: steady_clock::now();

    int i = 0;
    while(pacman.status == Gamestate::Running) {
        cursor_position(81, 6);
        count_time();
        check_direction();
        if(next_position()) {
            draw_player_position();
            cursor_position(81, 5);
            cout << "score: " << pacman.score;
        }

        /*
         * NOTE: we check the game condition at here too because
         * we need to check whether player bump into enemy after they
         * move.
         * Who knows the player is a member of suicide squad
         */
        check_game_condition();
        if (pacman.status != Gamestate::Running) {
            break;
        }

        if(i % 2 && pacman.start) {
            next_enemy_position();
            check_enemy_state();
            check_game_condition();
            draw_enemy_position();
            check_reset_enemy();
        }

        check_game_condition();
        Sleep(100);
        i++;
    } 
}

void
cleanup()
{
    record_player_rank();
    switch(pacman.status) {
        case Lose:
            system("CLS");
            cout << "You lose!\n";
            cout << "Total score: " << pacman.score << "\n";
            cout << "Total used time: " << pacman.hours << ":" << pacman.minutes << ":" << pacman.seconds << endl;
            system("pause");
            break;
        case Win:
            system("CLS");
            cout << "You lose!\n";
            cout << "Total score: " << pacman.score << "\n";
            cout << "Total used time: " << pacman.hours << ":" << pacman.minutes << ":" << pacman.seconds << endl;
            system("pause");
            break;
        default:
            // Unreachable
            break;
    }
}

int
main(void)
{
    setup();
    run();
    cleanup();
    return 0;
}
