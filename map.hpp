#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <windows.h>

string editor_command(string map_name);
vector<string> find_all_maps();
void init_map();
void load_default_map();
void preview_map(int x, int y);

using namespace std;

#include "global.hpp"
#include "game.hpp"

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
    "|        E#                #X         |",
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

/*
 * Write all default map to `pacman_folder`
 */
void
load_default_map()
{
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

/*
 * Find all maps located in `pacman_folder` path.
 * 1. We create the folder if it is missing (CreateDirectory)
 * 2. We try to find all files that matches with `.map` file extension recursively
 * 3. If there's no map found, load the default maps
 */
vector<string>
find_all_maps()
{
    // Create directory if necessary (if error was found, it means that directory has existed)
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

        // Load default maps if no map was found
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
void
init_map()
{
    char ch;
    fstream fin(pacman_folder + "\\" + pacman.map_choice, fstream::in);
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
            ch = (char)Map::Dot;
        } else if (ch == 'X') {
            // Player position are taken at top left corner
            player.x = x;
            player.y = y;
        } else if (ch == 'E') {
            // Enemy position are taken at top left corner
            enemy.x = x;
            enemy.y = y;
        } else {
            ch = (char)Map::Wall;
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
            /*
             * Indicate the initial player's position
             *   ┌┐
             *   └┘
             */
            map[y][x] = (char)218u;
            map[y][x + 1] = (char)191u;
            map[y + 1][x] = (char)192u;
            map[y + 1][x + 1] = (char)217u;
        } else if (ch == 'E') {
            // Refer to `enemy.hpp` file
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

    // Print the map
    for(int y = 0; y < 40; y++) 
    {
        for(int x = 0; x < 80 ; x++) {
            cout << map[y][x];
        }
        cout << endl;
    }
}

void
preview_map(int x, int y)
{
    string line;
    fstream fin(pacman_folder + "//" + pacman.map_choice, fstream::in);
    while(getline(fin, line)) {
        cursor_position(x, y++);
        cout << line;
    }
}

/*
 * A helper to construct the command to open notepad for editing the map
 */
string
editor_command(string map_name)
{
    return "notepad \"" + pacman_folder + "\\" + map_name + "\"";
}
