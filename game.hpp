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
        cout << " ";
}

bool check_obstacles(short x, short y) {
    return map[y][x] == '#' || map[y][x] == '|';
}

void InitializeTotalDots() {
    // Count the initial number of dots
    for (int row = 0; row < 20; row++) {
        for (int column = 0; column < 80; column++) {
            if (map[row][column] == '.') 
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

void print_title() {
    cout <<
    "#######        #           #######     ###     ###          #         #       #\n" <<
    "#      #      # #         #            #  #   #  #         # #        # #     #\n" <<
    "#      #     #   #       #             #   # #   #        #   #       #  #    #\n" <<
    "######      #     #     #              #    #    #       #     #      #   #   #\n" <<
    "#          # ##### #     #             #         #      # ##### #     #    #  #\n" <<
    "#         #         #     #            #         #     #         #    #     # #\n" <<
    "#        #           #     #######     #         #    #           #   #       #\n";
}

void cover() {
    int x, y;
    x = 40;
    y = 8;
    print_title();
    CursorPosition(x, y);
    cout << "start\n";
    cout << endl;
    CursorPosition(x, y + 2);
    cout << "leaderboard";
    CursorPosition(x, y + 4);
    cout << "quit";
    while(1) {

        if(GetAsyncKeyState(VK_DOWN) & 0x1 && y + 2 < 13) {
            delete_old_position(x - 1, y);
            y = y + 2;
        }
        if(GetAsyncKeyState(VK_UP) & 0x1 && y - 2 > 7) {
            delete_old_position(x - 1, y);
            y = y - 2;
        }
        CursorPosition(x - 1, y);
        cout << ">";
        if(GetAsyncKeyState(VK_RETURN)) {
            switch(y) {
                case 8:
                    system("CLS");
                    print_title();
                    cout << "\n";
                    cout << "Name: ";
                    cin >> name;
                    system("CLS");
                    break;
                case 10:
                    system("CLS");
                    read_player_file();
                    ranking();
                    print_ranking_list();
                    system("pause");
                    exit(0);
                    break;
                case 12:
                    exit(0);
                    break;
            }
            break;
        }
    }
}

void check_map() {
    int x, y;
    int i = 0;
    x = 0;
    y = 1;
    cout << "Pls choose the map:\n";
    cout << ">map1\n";
    cout << endl;
    for(int i = 2; i < 4; i++) {
        cout << " map" << i << "\n";
        cout << endl;
    }
    while(1) {

        if(GetAsyncKeyState(VK_DOWN) & 0x1 && y + 2 < 6) {
            delete_old_position(x, y);
            y = y + 2;
        }
        if(GetAsyncKeyState(VK_UP) & 0x1 && y - 2 > 0) {
            delete_old_position(x, y);
            y = y - 2;
        }
        CursorPosition(x, y);
        cout << ">";
        switch(y) {
                case 1:
                    map_num = 1;
                    break;
                case 3:
                    map_num = 2;
                    break;
                case 5:
                    map_num = 3;
                    break;
        }
        CursorPosition(0, 6);
        cout << endl;
        PreviewMap();
        if(GetAsyncKeyState(VK_RETURN))
            i++;
        if(GetAsyncKeyState(VK_RETURN) && i >= 2) {
            switch(y) {
                case 1:
                    map_num = 1;
                    break;
                case 3:
                    map_num = 2;
                    break;
                case 5:
                    map_num = 3;
                    break;
            }
            system("CLS");
            break;
        }
    }

}

void count_time(auto start) {
	auto finish = std::chrono::steady_clock::now();
    auto diff = finish - start;
    auto playtime = std::chrono::duration_cast<std::chrono::seconds>(diff).count();

    hours = playtime / 3600;
    minutes = (playtime % 3600) / 60;
    seconds = playtime % 60;
    cout << "Time: " << hours << ":" << minutes << ":" << seconds << endl;
}