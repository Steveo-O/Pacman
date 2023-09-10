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
        "|     |            E            |     |",
        "|   | |     #  #### ####   #    | |   |",
        "|                 # #                 |",
        "|     || | |      # #      | | ||     |",
        "|               ### ###               |",
        "|   | |           # #           | |   |",
        "|     |  #   ###### ######   #  |     |",
        "|  #     |         X         |     #  |",
        "|   #    |   ###### ######   |   #    |",
        "|                 # #                 |",
        "|  #  ###    #### # # ####   ###  #   |",
        "|    #    #   #   # #   #  #    #     |",
        "|  #              # #               # |",
        "|      | | |   |          |   | | |   |",
        "|  # |       | | |  ##  | | |       | |",
        "|      | | |   |          |   | | |   |",
        "|                                     |",
        "|    #   #   #  # # # # #   #   #     |",
        "+#####################################+",};

char default3[20][40] = {
        "+#####################################+",			
        "|     # # # #     #     # # # #       |",
        "|     #                       #    #  |",
        "|  #    |     #   #   #     |      #  |",
        "|     #   |               |   #       |",
        "|  ###      |     #     |        ###  |",
        "|         #   |       |    #          |",
        "|  #            |   |              #  |",
        "|     #   #   # | X |  #   #   #      |",
        "|  #            |   |              #  |",
        "|         #   |       |    #          |",
        "|     #     |     #     |      #      |",
        "|         |               |           |",
        "|  #    |     #   #   #     |      #  |",
        "|    #     #      E      #       #    |",
        "|  #              #                #  |",
        "|      #####   #######    #####       |",
        "|    #      #     #     #        #    |",
        "|        #                  #         |",
        "+#####################################+",};

char empty_map[20][40] = {
        "+#####################################+",			
        "|                                     |",
        "|                                     |",
        "|                                     |",
        "|                                     |",
        "|                                     |",
        "|                                     |",
        "|                 E                   |",
        "|                                     |",
        "|                                     |",
        "|                                     |",
        "|                 X                   |",
        "|                                     |",
        "|                                     |",
        "|                                     |",
        "|                                     |",
        "|                                     |",
        "|                                     |",
        "|                                     |",
        "+#####################################+",};

char map[40][80];
string search_pattern = "\\*.map";
string search_path = pacman_folder + search_pattern;

void load_default_map();
vector<string> find_all_maps();
void InitMap();
void PreviewMap();
string editor_command(string map_name);

/*
 * Write all default map to `pacman_folder`
 */
void load_default_map() {
    fstream fout1(pacman_folder + "//" + "default1.map", fstream::out);
    fstream fout2(pacman_folder + "//" + "default2.map", fstream::out);
    fstream fout3(pacman_folder + "//" + "default3.map", fstream::out);
    fstream fout4(pacman_folder + "//" + "empty_map.map", fstream::out);

    for (auto line: default1) {
        fout1 << line << endl;
    }
    for (auto line: default2) {
        fout2 << line << endl;
    }
    for (auto line: default3) {
        fout3 << line << endl;
    }
    for (auto line: empty_map) {
        fout4 << line << endl;
    }

    fout1.close();
    fout2.close();
    fout3.close();
    fout4.close();
}

/*
 * Find all maps located in `pacman_folder` path.
 * 1. We create the folder if it is missing (CreateDirectory)
 * 2. We try to find all files that matches with `.map` file extension recursively
 * 3. If there's no map found, load the default maps
 */
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

/*
 * Load map according to the user choice and print them out
 * Here we expand the map before displaying them
 *
 */
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
            map[y][x] = '\\';
            map[y][x + 1] = '/';
            map[y + 1][x] = '/';
            map[y + 1][x + 1] = '\\';
        } else if (ch == 'E') {
            map[y][x] = '^';
            map[y][x + 1] = '^';
            map[y + 1][x] = '=';
            map[y + 1][x + 1] = '=';
        } else {
            map[y][x] = ch;
            map[y + 1][x + 1] = ch;
            map[y + 1][x] = ch;
            map[y][x + 1] = ch;
        }

        col += 1;
    }
    // Print the map
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

/*
 * A helper to construct the command to open notepad for editing the map
 */
string editor_command(string map_name) {
     return "notepad \"" + pacman_folder + "\\" + map_name + "\"";
}
