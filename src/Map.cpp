#include "map.h"
#include <iostream>


const int mapWidth = 16, mapHeight = 16;
const char map[] =
    "################"
    "#....####......#"
    "#....#.........#"
    "#.....#........#"
    "#.....#........#"
    "#.....#........#"
    "#.....####.....#"
    "#..............#"
    "#..............#"
    "#..............#"
    "#..............#"
    "#..............#"
    "#..............#"
    "#..............#"
    "#..............#"
    "################";

const char map1[] =
    "########################"
    "#......................#"
    "#......................#"
    "#......................#"
    "#........#####.........#"
    "#........#.............#"
    "#........#.............#"
    "#........#.............#"
    "#........#6.............#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "########################";


char getArrow(float dirX, float dirY) {
    
    if (std::abs(dirX) > std::abs(dirY))
        if (dirX > 0) return '<';
        else return '>';
    else
        if (dirY > 0) return '^';
        else return 'v';
}


void printMapWithPlayer(int playerX, int playerY, float dirX, float dirY) {
    int arrowX = playerX + std::round(dirX);
    int arrowY = playerY + std::round(dirY);
    char arrowChar = getArrow(dirX, dirY);

    for (int y = mapHeight - 1; y >= 0; y--) {
        for (int x = mapWidth; x >= 0; x--) {
            if (x == playerX && y == playerY)
                std::cout << 'P';
            else if (x == arrowX && y == arrowY)
                std::cout << arrowChar;
            else
                std::cout << map[y * mapWidth + x];
        }
        std::cout << "\n";
    }
}
