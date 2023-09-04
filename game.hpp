#pragma once
#include <iostream>
#include <windows.h>
#include <cmath>
#include <chrono>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "global.hpp"
#include "map.hpp"

using namespace std;

void choose_map(string);
void main_menu();
void map_screen();

const int key_up = 72;
const int key_down = 80;
const int key_left = 75;
const int key_right = 77;
const int key_enter = 13;

// Move the Cursor to the given position
void CursorPosition(short x, short y) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);  
    COORD position = {(short)x, (short)y};  

    // Blinking cursor is annoying!!!
    // Make the cursor invisible
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hStdout, &cursorInfo);

    SetConsoleCursorPosition(hStdout, position); 
} 

void delete_old_position(short x, short y) {
    CursorPosition(x, y);
    cout << ' ';
    CursorPosition(x + 1, y);
    cout << ' ';
    CursorPosition(x, y + 1);
    cout << ' ';
    CursorPosition(x + 1, y + 1);
    cout << ' ';
}

bool check_obstacles(short x, short y) {
    return map[y][x] == (char)MAP::Wall;
}

void InitializeTotalDots() {
    // Count the initial number of dots
    for (int row = 0; row < 40; row += 2) {
        for (int column = 0; column < 80; column += 2) {
            if (map[row][column] == (char)MAP::Dot)
                totalDots++;
        }
    }
}

void check_game_condition() { 
    int distance, distance_x, distance_y;
    distance_x = player.x - enemy.x;
    distance_y = player.y - enemy.y;
    distance = sqrt(distance_x * distance_x + distance_y * distance_y);

    if(distance == 0) 
        pacman.status = GAMESTATE::lose;
    else if(score >= totalDots)
        pacman.status = GAMESTATE::win;
}

void read_player_file() {
    string line, name;
    int score, time;
    int i = 0;
    int member = 0;
    fstream player_file("playerfile.txt", fstream::in);
    while (getline(player_file, line)) {
        
        i++;
        if(i == 1)
            name = line;
        if(i == 2) 
            score = stoi(line);  
        if(i == 3)
            time = stoi(line);
        
        if(i > 3) { 
            i = 1;
            players.push_back(Player());
            players[member].player_name = name;
            players[member].highscore = score;
            players[member].duration = time;
            member++;
            name = line;
        }
    }
    player_file.close();
}

void ranking() {
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

void print_ranking_list() {
    int size = sorted_players.size();
    cout << "Name\t" << "Score\t" << "Duration\n";
    for(int i = 0; i < size; i++) {
        cout << sorted_players[i].player_name << "\t" << sorted_players[i].highscore << 
        "\t" << sorted_players[i].duration << endl;
    }
    cout << endl;
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


void record_player_rank() {
    int playtime;
    fstream player_file("playerfile.txt", fstream::app);

    playtime = hours * 3600 + minutes * 60 + seconds;

    player_file << name << endl;
    player_file << score << endl;
    player_file << playtime << endl;
    
    player_file.close();
}

void title() {
    cout <<
    "#######        #           #######     ###     ###          #         ##      #\n" <<
    "#      #      # #         #            #  #   #  #         # #        # #     #\n" <<
    "#      #     #   #      #              #   # #   #        #   #       #  #    #\n" <<
    "######      #     #     #              #    #    #       #     #      #   #   #\n" <<
    "#          # ##### #    #              #         #      # ##### #     #    #  #\n" <<
    "#         #         #     #            #         #     #         #    #     # #\n" <<
    "#        #           #     #######     #         #    #           #   #      ##\n";
}

void main_menu() {
    int x, y;
    x = 40;
    y = 8;
    CursorPosition(0, 0);
    title();
    CursorPosition(x, y);
    cout << "start" << endl;
    cout << endl;
    CursorPosition(x, y + 2);
    cout << "map editor" << endl;
    cout << endl;
    CursorPosition(x, y + 4);
    cout << "leaderboard" << endl;
    cout << endl;
    CursorPosition(x, y + 6);
    cout << "quit";
    while(1) {
        CursorPosition(x - 1, y);
        cout << ">";
        while(!kbhit()) {}
        int const ch = getch();
        switch(ch) {
            case key_up:
                if (y - 2 > 7) {
                    CursorPosition(x - 1, y);
                    cout << ' ';
                    y -= 2;
                }
                break;
            case key_down:
                if (y + 2 < 15) {
                    delete_old_position(x - 1, y);
                    y += 2;
                }
                break;
            case key_enter:
                switch(y) {
                    case 8:
                        system("CLS");
                        title();
                        CursorPosition(x, y);
                        cout << "Name: ";
                        cin >> name;
                        choose_map("Please select a map: ");
                        break;
                    case 10:
                        system("CLS");
                        map_screen();
                        break;
                    case 12:
                        system("CLS");
                        read_player_file();
                        ranking();
                        print_ranking_list();
                        break;
                    case 14:
                        exit(0);
                        break;
                }
                return;
        }
    }
}

void choose_map(string message) {
    int x = 0;
    int y = 1;
    system("CLS");

    vector<string> maps = find_all_maps();
    int max_list = maps.size() * 2;
    cout << message << endl;
    for (string map: maps) {
        cout << " " << map << endl << endl;
    }

    while(1) {
        CursorPosition(x, y);
        cout << ">";
        map_choice = maps[y / 2];

        CursorPosition(0, max_list);
        cout << endl;
        PreviewMap();

        while(!kbhit()){}
        int const ch = getch();
        switch(ch) {
            case key_up:
                if (y - 2 > 0) {
                    CursorPosition(x, y);
                    cout << ' ';
                    y -= 2;
                }
                break;
            case key_down:
                if (y + 2 < max_list) {
                    CursorPosition(x, y);
                    cout << ' ';
                    y += 2;
                }
                break;
            case key_enter:
                system("CLS");
                return;
        }
    }
}

void map_screen() {
    int x = 40;
    int y = 8;
    string command;

    title();

    CursorPosition(x, y);
    cout << "new map" << endl << endl;
    CursorPosition(x, y + 2);
    cout << "edit map" << endl << endl;
    CursorPosition(x, y + 4);
    cout << "return to main menu";

    while(1) {
        CursorPosition(x - 1, y);
        cout << ">";
        while(!kbhit()) {}
        int const ch = getch();
        switch(ch) {
            case key_up:
                if (y - 2 > 7) {
                    CursorPosition(x - 1, y);
                    cout << ' ';
                    y -= 2;
                }
                break;
            case key_down:
                if (y + 2 < 13) {
                    CursorPosition(x - 1, y);
                    cout << ' ';
                    y += 2;
                }
                break;
            case key_enter:
                switch(y) {
                    case 8:
                        {
                            char use_template;
                            string res;
                            while (1) {
                                CursorPosition(x, y + 14);
                                cout << "Use existing maps as template? [y/n]";
                                use_template = _getch();
                                if (tolower(use_template) != 'y' && tolower(use_template) != 'n') {
                                    continue;
                                }
                                break;
                            }
                            cout << endl;
                            CursorPosition(x, y + 15);
                            cout << "Enter the new map name: ";
                            cin >> res;

                            // Copy existing map to new file (input)
                            if (tolower(use_template) == 'y') {
                                choose_map("Choose a map as template: ");
                                CopyFile((map_path + "\\" + map_choice).c_str(), (map_path + "\\" + res + ".map").c_str(), true);
                            }

                            // Start notepad and wait for user to close it
                            command = editor_command(res + ".map");
                            system(command.c_str());

                            main_menu();
                            break;
                        }
                    case 10:
                        choose_map("Choose a map to be edited: ");

                        // Start notepad and wait for user to close it
                        command = editor_command(map_choice);
                        system(command.c_str());
                        main_menu();
                        break;
                    case 12:
                        system("CLS");
                        return main_menu();
                }
                return;
        }
    }
}

void count_time() {
	auto finish = std::chrono::steady_clock::now();
    auto diff = finish - time_start;
    auto playtime = std::chrono::duration_cast<std::chrono::seconds>(diff).count();

    hours = playtime / 3600;
    minutes = (playtime % 3600) / 60;
    seconds = playtime % 60;
    cout << "Time: " << hours << ":" << minutes << ":" << seconds << endl;
}
