#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>

#include "global.hpp"



using namespace std;

char map[40][80];
char* appdata = getenv("APPDATA");
string map_folder = "\\pacman_game";
string map_path = appdata + map_folder;
string search_pattern = "\\*.map";
string search_path = map_path + search_pattern;

vector<string> find_all_maps() {
    // Create directory if necessary
    if (CreateDirectory(map_path.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
        vector<string> maps;
        WIN32_FIND_DATA fd;

        // Try to find the file by pattern (`search_pattern`)
        // All .map file extension will be matched (*.map)
        HANDLE hFind = FindFirstFile(search_path.c_str(), &fd);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                // Only append those result that are files
                // Meaning exclude . and .. directory
                if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    maps.push_back(fd.cFileName);
                }
            } while (FindNextFile(hFind, &fd));
            FindClose(hFind);
        }
        return maps;
    }
    exit(1); // TODO: handle the error nicely
}

void ReadMap() {
    char ch;
    fstream fin(map_path + "//" + map_choice, fstream::in);
    int row = 0;
    int col = 0;
    while(fin >> noskipws >> ch) {
        // newline indicates a new row
        // update the col and row but don't do anything
        if (ch == '\n') {
            row += 1;
            col = 0;
            continue;
        }
        // replace empty space with dot
        if (ch == ' ') {
            ch = '.';
        }

        /* Expand the map to the following format
            |x|x|   --->   |x|#|x|#|
            -----          ---------
            |x|x|          |#|#|#|#|
                           ---------
                           |x|#|x|#|
                           ---------
                           |#|#|#|#|
            where the int x and int y refers to
            `x` coordinate.

            And we fill up the neighbour
            (horizontal, vertical and diagonal) with the `ch`.
        */
        int x = col * 2;
        int y = row * 2;
        map[y][x] = ch;
        map[y + 1][x + 1] = ch;
        map[y + 1][x] = ch;
        map[y][x + 1] = ch;

        col += 1;
    }
}

void ShowMap()  
{
    for(int y = 0; y < 40; y++) 
    {
        for(int x = 0; x < 80; x++) {
            // find where the initial player position
            if(map[y][x] == 'X') {
                player.x = x;
                player.y = y;
            }
            cout << map[y][x];
        }
        cout << endl;
    }
}

void PreviewMap() 
{
    string line;
    fstream fin(map_path + "//" + map_choice, fstream::in);
    while(getline(fin, line)) {
        cout << line << endl;
    }
    cout << endl;
}

string editor_command(string map_name) {
     return "notepad \"" + map_path + "\\" + map_name + "\"";
}
