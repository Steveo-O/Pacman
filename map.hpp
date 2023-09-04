#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>

#include "global.hpp"

using namespace std;

char default1[20][40] = {
        "+#####################################+",
        "|             ###########             |",
        "| # ####### #           # # ####### # |",
        "| #       # # ######### # #       # # |",
        "| # #       #      E      # #       # |",
        "| # ####### # ########### # ####### # |",
        "| #         #             #         # |",
        "|              #### ####            # |",
        "|  # ###### #      X      # ###### ## |",
        "| #    #     #  |     | #    #     ## |",
        "| #  #   #   #  |#####| #  #   #   ## |",
        "|  # ###### #             # ###### ## |",
        "|     #   #    #### ####    #   #     |",
        "| #     #   #             #   #     # |",
        "| # ####### # ########### # ####### # |",
        "| # #       #             # #       # |",
        "| #       # # ######### # #       # # |",
        "| # ####### #           # # ####### # |",
        "|             ###########             |",
        "+#####################################+"};

char default2[20][40] = {
        "+#####################################+",
        "|         #                #          |",
        "|         #                #          |",
        "|         #                #          |",
        "|         #                #          |",
        "|         #                #          |",
        "|     #####       #        #####      |",
        "|                 #                   |",
        "|                ###                  |",
        "|           #############             |",
        "|                ###                  |",
        "|                 #                   |",
        "|                 #                   |",
        "|     #####                #####      |",
        "|         #                #          |",
        "|         #                #          |",
        "|         #                #          |",
        "|         #                #          |",
        "|         #                #          |",
        "+#####################################+"};

char default3[20][40] = {
        "+#####################################+",
        "|             ###########             |",
        "| # ####### #           # # ####### # |",
        "| #       # # ######### # #       # # |",
        "| # #       #             # #       # |",
        "| # ####### # ########### # ####### # |",
        "| #         #             #         # |",
        "|              #### ####            # |",
        "|  # ####### #     X      # ###### ## |",
        "| #    #   # #  |     | #    #     ## |",
        "| #  #   #   #  |#####| #  #   #   ## |",
        "|  # ####### #            # ###### ## |",
        "|     #   #    #### ####    #   #     |",
        "| #     #   #      E      #   #     # |",
        "| # ####### # ########### # ####### # |",
        "| # #       #             # #       # |",
        "| #       # # ######### # #       # # |",
        "| # ####### #           # # ####### # |",
        "|             ###########             |",
        "+#####################################+"};

char map[40][80];
string search_pattern = "\\*.map";
string search_path = pacman_folder + search_pattern;

void load_default_map() {
    fstream fout1(pacman_folder + "//" + "default1.map", fstream::out);
    fstream fout2(pacman_folder + "//" + "default2.map", fstream::out);
    fstream fout3(pacman_folder + "//" + "default3.map", fstream::out);

    for (auto line: default1) {
        fout1 << line << endl;
    }
    for (auto line: default2) {
        fout2 << line << endl;
    }
    for (auto line: default3) {
        fout3 << line << endl;
    }

    fout1.close();
    fout2.close();
    fout3.close();
}

vector<string> find_all_maps() {
    // Create directory if necessary
    if (CreateDirectory(pacman_folder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
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
        if (maps.size() == 0) {
            load_default_map();
            return find_all_maps();
        }
        return maps;
    }
    exit(1); // TODO: handle the error nicely
}

void InitMap() {
    char ch;
    fstream fin(pacman_folder + "\\" + map_choice, fstream::in);
    int row = 0;
    int col = 0;
    while(fin >> noskipws >> ch) {
        int x = col * 2;
        int y = row * 2;

        // newline indicates a new row
        // update the col and row but don't do anything
        if (ch == '\n') {
            row += 1;
            col = 0;
            continue;
        }

        // replace empty space with dot
        if (ch == ' ') {
            ch = (char)MAP::Dot;
        } else if (ch == 'X') {
            // Player position are referred
            // at the upper left corner
            player.x = x;
            player.y = y;
        } else if (ch == 'E') {
            enemy.x = x;
            enemy.y = y;
        } else {
            ch = (char)MAP::Wall;
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
        if (ch == 'X') {
            map[y][x] = (char)218u;
            map[y][x + 1] = (char)191u;
            map[y + 1][x] = (char)192u;
            map[y + 1][x + 1] = (char)217u;
        } else if (ch == 'E') {
            map[y][x] = (char)250u;
            map[y][x + 1] = (char)250u;
            map[y + 1][x] = (char)205u;
            map[y + 1][x + 1] = (char)205u;
        } else {
            map[y][x] = ch;
            map[y + 1][x + 1] = ch;
            map[y + 1][x] = ch;
            map[y][x + 1] = ch;
        }

        col += 1;
    }
    for(int y = 0; y < 40; y++) 
    {
        for(int x = 0; x < 80 ; x++) {
            cout << map[y][x];
        }
        cout << endl;
    }
}

void PreviewMap() 
{
    string line;
    fstream fin(pacman_folder + "//" + map_choice, fstream::in);
    while(getline(fin, line)) {
        cout << line << endl;
    }
    cout << endl;
}

string editor_command(string map_name) {
     return "notepad \"" + pacman_folder + "\\" + map_name + "\"";
}
