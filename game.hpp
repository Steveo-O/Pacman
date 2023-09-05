#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <conio.h>
#include <windows.h>

using namespace std;

int totalDots = 0;
string name;
vector<Player> players;
vector<Player> sorted_players;

void check_game_condition();
bool check_obstacles(short x, short y);
void choose_map(string message);
void count_time();
void cursor_position(short x, short y);
void delete_old_position(short x, short y);
void init_total_dots();
void main_menu();
void map_screen();
void print_ranking_list();
void ranking();
void read_player_file();
void record_player_rank();
void title(int &y);

#include "global.hpp"
#include "map.hpp"

/*
 * Move console cursor to the position (x, y)
 */
void
cursor_position(short x, short y)
{
    COORD position = {(short)x, (short)y};
    SetConsoleCursorPosition(pacman.hConsole, position);
}

/*
 * Remove current component in the grid of position (x, y)
 *      (x,y)
 *       v
 *      |x|x|
 *      ----
 *      |x|x|
 */
void
delete_old_position(short x, short y)
{
    cursor_position(x, y);
    cout << ' ';
    cursor_position(x + 1, y);
    cout << ' ';
    cursor_position(x, y + 1);
    cout << ' ';
    cursor_position(x + 1, y + 1);
    cout << ' ';
}

/*
 * Check if the position (x, y) is a wall
 * Note that it should receive
 * the coordinate of the top left corner grid
 */
bool
check_obstacles(short x, short y)
{
    return map[y][x] == (char)Map::Wall;
}

/*
 * Count the initial number of dots
 */
void
init_total_dots()
{
    for (int row = 0; row < 40; row += 2) {
        for (int column = 0; column < 80; column += 2) {
            if (map[row][column] == (char)Map::Dot)
                totalDots++;
        }
    }
}

void
check_game_condition()
{ 
    int distance_x = player.x - enemy.x;
    int distance_y = player.y - enemy.y;

    if(distance_x == 0 && distance_y == 0)
        pacman.status = Gamestate::Lose;
    else if(pacman.score >= totalDots)
        pacman.status = Gamestate::Win;
}

void
read_player_file()
{
    string line, name;
    int score, time;
    int i = 0;
    int member = 0;
    fstream player_file(pacman_folder + "\\" + "playerfile.txt", fstream::in);
    while (getline(player_file, line)) {
        i++;
        if(i == 1)
            name = line;
        if(i == 2) 
            score = stoi(line);  
        if(i == 3) {
            time = stoi(line);

            // By this time, we have collected all information
            // of the particular player.
            // Save them and reset `i`
            players.push_back(Player());
            players[member].player_name = name;
            players[member].highscore = score;
            players[member].duration = time;

            member++;
            i = 0;
        }
    }
    player_file.close();
}

void
ranking()
{
    int size = players.size();
    int arr[size];
    for(int i = 0; i < size; i++) {
        arr[i] = players[i].highscore;
    }
    cout << endl;
    sort(arr, arr + size, greater<int>());

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(players[j].highscore == arr[i]) {
                sorted_players.push_back(Player());
                sorted_players[i].player_name = players[j].player_name;
                sorted_players[i].highscore = arr[i];
                sorted_players[i].duration = players[j].duration;
            }  
        }
    }
}

void
print_ranking_list()
{
    int max_name_len = 0;
    int max_score_len = 0;
    int max_duration_len = 0;
    int size = sorted_players.size();

    // Scan for the longest length of data for each field
    for (auto player: sorted_players) {
        if ((int)player.player_name.length() > max_name_len) {
            max_name_len = player.player_name.length();
        }
        if ((int)to_string(player.highscore).length() > max_score_len) {
            max_score_len = to_string(player.highscore).length();
        }
        if ((int)to_string(player.duration).length() > max_duration_len) {
            max_duration_len = to_string(player.duration).length();
        }
    }
    // Add one more length for whitespace
    max_name_len++;
    max_score_len = max_score_len < 6 ? 6 : max_score_len + 1;
    max_duration_len = max_score_len < 9 ? 9 : max_score_len;

    /* Calculate the beginning position to print.
     *
     *   x        x
     *   V        V
     *  ------------
     *  |          | < y
     *  |  xxxxxx  |
     *  |  xxxxxx  |
     *  |  xxxxxx  |
     *  |  xxxxxx  |
     *  |          | < y
     *  ------------
     *
     * For x axis, the coordinate is determined by finding the remaining spaces
     * after occupying the longest data in a row.
     * Similarly, y coordinate is obtained by subtracting the total length of list
     */
    int x = (SCREEN_WIDTH - max_name_len - max_score_len - max_duration_len) / 2;
    int y = (SCREEN_HEIGHT - size - 1) / 2;

    /*
     * To compute how much space to insert between table header,
     * we use the longest the data available minus the length of
     * the data word
     */
    cursor_position(x, y++);
    cout << "Name" << string(max_name_len - 4, ' ');
    cout << "Score" << string(max_score_len - 5, ' ');
    cout << "Duration" << endl;
    for(int i = 0; i < size; i++) {
        cursor_position(x, y++);
        cout << sorted_players[i].player_name;
        cout << string(max_name_len - sorted_players[i].player_name.length(), ' ');

        cout << sorted_players[i].highscore;
        cout << string(max_score_len - to_string(sorted_players[i].highscore).length(), ' ');

        cout << sorted_players[i].duration;
        cout << endl;
    }
    cout << endl << endl;
    cursor_position(x, y + 1);
    cout << ">return to main menu";

    while(1) {
        while(!kbhit()) {}
        int const ch = getch();
        if(ch == key_enter) {
            system("CLS");
            return main_menu();
        }
    }
}


void
record_player_rank()
{
    int playtime;
    fstream player_file(pacman_folder + "\\" + "playerfile.txt", fstream::app);

    playtime = pacman.hours * 3600 + pacman.minutes * 60 + pacman.seconds;

    player_file << name << endl;
    player_file << pacman.score << endl;
    player_file << playtime << endl;

    player_file.close();
}

/*
 * The magic number 81 is actually the length of `PACMAN` banner.
 * To understand why moving cursor position, refer to `print_ranking_list` function,
 * it is to put the title in the middle of the screen
 *
 * We do not compute the y position of the banner, it is up to the caller so that the caller
 * can adjust the title to accommodate the menu items
 */
void
title(int &y)
{
    cursor_position((SCREEN_WIDTH - 81) / 2, y++);
    cout << "#######        #           #######     ###     ###          #         ##      #";
    cursor_position((SCREEN_WIDTH - 81) / 2, y++);
    cout << "#      #      # #         #            #  #   #  #         # #        # #     #";
    cursor_position((SCREEN_WIDTH - 81) / 2, y++);
    cout << "#      #     #   #      #              #   # #   #        #   #       #  #    #";
    cursor_position((SCREEN_WIDTH - 81) / 2, y++);
    cout << "######      #     #     #              #    #    #       #     #      #   #   #";
    cursor_position((SCREEN_WIDTH - 81) / 2, y++);
    cout << "#          # ##### #    #              #         #      # ##### #     #    #  #";
    cursor_position((SCREEN_WIDTH - 81) / 2, y++);
    cout << "#         #         #     #            #         #     #         #    #     # #";
    cursor_position((SCREEN_WIDTH - 81) / 2, y++);
    cout << "#        #           #     #######     #         #    #           #   #      ##";
    cursor_position((SCREEN_WIDTH - 81) / 2, y++);
    cout << endl;
}

void
main_menu()
{
    system("CLS");
    /*
     * XXX: I know magic number should be banned but.. who cares :P
     * 11 is refer to the length of `leaderboard` word
     * 14 is the total height of title banner + total menu items
     */
    int x = (SCREEN_WIDTH - 11) / 2;
    int y = (SCREEN_HEIGHT - 14) / 2;
    title(y);

    cursor_position(x, y);
    cout << "start" << endl << endl;
    cursor_position(x, y + 2);
    cout << "map editor" << endl << endl;
    cursor_position(x, y + 4);
    cout << "leaderboard" << endl << endl;
    cursor_position(x, y + 6);
    cout << "quit";

    /*
     * Here we have to store the cursor position, which is `cy`
     * We take y as the origin that is the top of the menu,
     * and compute the current selection
     */
    int cy = y;
    while(1) {
        cursor_position(x - 1, cy);
        cout << ">";
        while(!kbhit()) {}
        int const ch = getch();
        switch(ch) {
            case key_up:
                if (cy - 2 >= y) {
                    cursor_position(x - 1, cy);
                    cout << ' ';
                    cy -= 2;
                }
                break;
            case key_down:
                if (cy + 2 <= y + 6) {
                    cursor_position(x - 1, cy);
                    cout << ' ';
                    cy += 2;
                }
                break;
            case key_enter:
                if (cy == y) {
                    system("CLS");
                    // NOTE: we can't use back `y` variable
                    // as it is currently pointing at the first menu
                    int ty = (SCREEN_HEIGHT - 14) / 2;
                    title(ty);
                    cursor_position(x, ty);
                    cout << "Name: ";
                    // store name with whitespaces
                    // and avoid empty input
                    while(getline(cin, name)) {
                        if (name != "") {
                            break;
                        }
                        cursor_position(x + 6, ty);
                    }
                    choose_map("Please select a map: ");
                } else if (cy == y + 2) {
                    system("CLS");
                    map_screen();
                } else if (cy == y + 4) {
                    system("CLS");
                    read_player_file();
                    ranking();
                    print_ranking_list();
                } else if (cy == y + 6) {
                    exit(0);
                }
                return;
        }
    }
}

void
choose_map(string message)
{
    system("CLS");
    vector<string> maps = find_all_maps();

    /*
     * Magic numbers:
     *  - 40 -> length of map preview
     *  - 20 -> height of map preview
     *
     *  Here, the message is aligned with the left side of
     *  map preview. It's not the best design, but it is
     *  still neat.
     *
     *  For vertical direction, we compute the amount of map list
     *  multiply by 2 (as each map inserts a blank line) and include
     *  the height of map preview.
     */
    int x = ((SCREEN_WIDTH - 40) / 2);
    int y = (SCREEN_HEIGHT - (maps.size() * 2) - 20) / 2;

    cursor_position(x, y++);
    cout << message;
    for (int i = 0; i < (int)maps.size(); i++) {
        cursor_position(x, y + (i * 2));
        cout << " " << maps[i];
    }

    /*
     * Same thing as `main_menu` function
     */
    int cy = y;
    int max_list = maps.size() * 2;
    while(1) {
        cursor_position(x, cy);
        cout << ">";
        pacman.map_choice = maps[(cy - y) / 2];

        cursor_position(x, y + max_list);
        cout << endl;
        preview_map(x, y + max_list);

        while(!kbhit()){}
        int const ch = getch();
        switch(ch) {
            case key_up:
                if (cy - 2 >= y) {
                    cursor_position(x, cy);
                    cout << ' ';
                    cy -= 2;
                }
                break;
            case key_down:
                if (cy + 2 < y + max_list) {
                    cursor_position(x, cy);
                    cout << ' ';
                    cy += 2;
                }
                break;
            case key_enter:
                system("CLS");
                return;
        }
    }
}

/*
 * Refer to `main_menu` function to understand
 * how the display is centered at screen
 */
void
map_screen()
{
    string command;

    int x = (SCREEN_WIDTH / 2) - (19 / 2);
    int y = (SCREEN_HEIGHT - 14) / 2;
    title(y);

    cursor_position(x, y);
    cout << "new map" << endl << endl;
    cursor_position(x, y + 2);
    cout << "edit map" << endl << endl;
    cursor_position(x, y + 4);
    cout << "return to main menu";

    int cy = y;
    while(1) {
        cursor_position(x - 1, cy);
        cout << ">";
        while(!kbhit()) {}
        int const ch = getch();
        switch(ch) {
            case key_up:
                if (cy - 2 >= y) {
                    cursor_position(x - 1, cy);
                    cout << ' ';
                    cy -= 2;
                }
                break;
            case key_down:
                if (cy + 2 <= y + 4) {
                    cursor_position(x - 1, cy);
                    cout << ' ';
                    cy += 2;
                }
                break;
            case key_enter:
                if (cy == y) {
                    /*
                     * Create new map and let user to edit, also allow users to
                     * copy existing map as template.
                     */
                    char use_template;
                    string res;

                    string q1 = "Use existing maps as template? [y/n]";
                    string q2 = "Enter the new map name: ";
                    // Compute the position to prompt Q1
                    // Magic number 14 is really magic :tada:
                    cursor_position((SCREEN_WIDTH - q1.length()) / 2, cy + 14);
                    cout << q1;
                    while (1) {
                        use_template = _getch();
                        if (tolower(use_template) == 'y' ||
                            tolower(use_template) == 'n') {
                            break;
                        }
                    }
                    // Magic number 15 is really magic :tada:
                    cursor_position((SCREEN_WIDTH - q1.length()) / 2, cy + 15);
                    cout << q2;
                    cin >> res;

                    // Copy existing map to new file (input)
                    if (tolower(use_template) == 'y') {
                        choose_map("Choose a map as template: ");
                        CopyFile((pacman_folder + "\\" + pacman.map_choice).c_str(), (pacman_folder + "\\" + res + ".map").c_str(), true);
                    }

                    // Start notepad and wait for user to close it
                    command = editor_command(res + ".map");
                    system(command.c_str());

                    return main_menu();
                } else if (cy == y + 2) {
                    choose_map("Choose a map to be edited: ");

                    // Start notepad and wait for user to close it
                    command = editor_command(pacman.map_choice);
                    system(command.c_str());
                    return main_menu();
                } else if (cy == y + 4) {
                    return main_menu();
                }
                break;
        }
    }
}

void
count_time()
{
    auto finish = std::chrono::steady_clock::now();
    auto diff = finish - pacman.time_start;
    auto playtime = std::chrono::duration_cast<std::chrono::seconds>(diff).count();

    pacman.hours = playtime / 3600;
    pacman.minutes = (playtime % 3600) / 60;
    pacman.seconds = playtime % 60;
    cout << "Time: " << pacman.hours << ":" << pacman.minutes << ":" << pacman.seconds << endl;
}
