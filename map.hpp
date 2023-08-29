#pragma once
#include <iostream>
#include <windows.h>

#include "global.hpp"


/*
a = aa
    aa

*/

using namespace std;

char tmp_map[20][40] = {
    "+$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$+",
    "|             $$$$$$$$$$$             |",
    "| $ $$$$$$$ $           $ $ $$$$$$$ $ |",
    "| $       $ $ $$$$$$$$$ $ $       $ $ |",
    "| $ $       $             $ $       $ |",
    "| $ $$$$$$$ $ $$$$$$$$$$$ $ $$$$$$$ $ |",
    "| $         $                       $ |",
    "|              $$$$ $$$$              |",
    "|                  X                  |",
    "|               |     |               |",
    "|               |$$$$$|               |",
    "|                                     |",
    "|                                     |",
    "|                                     |",
    "|                                     |",
    "|                                     |",
    "|                                     |",
    "|                                     |",
    "|                                     |",
    "+$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$+"};

char map[20][80];

void ShowMap()  
{  
    int first, second;

    for (int row = 0; row < 20; row++)  
    {  
        first = -1;
        second = 0;
        // print out the whole map
        for(int column = 0; column < 40; column++ ) {
            
            for(int i = 0; i < 2; i++ ) {
                if(i == 0) {
                    first++;
                    map[row][column + first] = tmp_map[row][column];

                }
                else {
                    second++;
                    map[row][column + second] = tmp_map[row][column];

                }
            }

        }
    }
    for(int y = 0; y < 20; y++) 
    {
        for(int x = 0; x < 80; x++) {
            // find where the initial player position
            if(map[y][x] == 'X') {
                player.x = x;
                player.y = y;
            }
            else if (map[y][x] == ' ') {
                // Add a dot to empty spaces
                map[y][x] = '.';
            }
            cout << map[y][x];
        }
        cout << endl;
    }  
}  
