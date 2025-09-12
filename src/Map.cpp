#include "map.h"
#include <iostream>


const int mapWidth = 16, mapHeight = 16;
const char map[] =
    "################"
    "#..............#"
    "#....######....#"
    "#....#....#....#"
    "#....#....#....#"
    "#....#....#....#"
    "#.........#....#"
    "#.........#....#"
    "#....######....#"
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

WallType getWallType(char tile){
    switch (tile) {
        case '#': return WALL_GREY;
        //case '1': return WALL_RED;
        case '1': return WALL_BLUE;
        default: return WALL_NONE;
    }
}

bool isWall(char tile){
    return (getWallType(tile) != WALL_NONE);
}

WallColorInfo getWallColorInfo(WallType wall) {
    switch (wall){
        case WALL_GREY: return {232, 24};    // 232->255, bright is high
        case WALL_BLUE: return {153, 133};  // 153->21(light->strong blue)
        // Add more: e.g. {red_base, red_range}
        default:        return {244, 1};     // fallback

    }
    
}
char getArrow(float dirX, float dirY) {
    if (std::abs(dirX) >= std::abs(dirY))
        return (dirX >= 0) ? '<' : '>';
    else
        return (dirY >= 0) ? '^' : 'v';
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

void printMapWithPlayerAndMonster(int playerX, int playerY, float dirX, float dirY, int monsterX, int monsterY) {
    int arrowX = playerX + std::round(dirX);
    int arrowY = playerY + std::round(dirY);
    char arrowChar = getArrow(dirX, dirY);

    for (int y = mapHeight - 1; y >= 0; y--) {
        for (int x = 0; x < mapWidth; ++x) {
            if (x == playerX && y == playerY)
                std::cout << 'P';
            else if (x == arrowX && y == arrowY)
                std::cout << arrowChar;
            else if (x == monsterX && y == monsterY)
                std::cout << 'M';
            else
                std::cout << map[y * mapWidth + x];
        }
        std::cout << "\n";
    }
}