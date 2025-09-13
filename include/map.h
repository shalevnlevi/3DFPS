#ifndef MAP_H
#define MAP_H
#include <unordered_map>

extern const int mapWidth;
extern const int mapHeight;
extern const char map[];

enum WallType {
    WALL_NONE = 0,
    WALL_GREY,
    WALL_RED,
    WALL_BLUE,
    WALL_GREEN
    // Add more as needed
};

struct WallColorInfo {
    int colorCode;
    bool useShading;
};

static const std::unordered_map<WallType, WallColorInfo> wallColors = {
    { WallType::WALL_GREY,  {0,    true } },   // shading handled separately
    { WallType::WALL_RED,   {196, false} },
    { WallType::WALL_BLUE,  {21,  false} },
    { WallType::WALL_GREEN, {46,  false} }
};

WallType getWallType(char tile);
//WallColorInfo getWallColorInfo(WallType type);
bool isWall(char tile);

void printMapWithPlayer(int playerX, int playerY, float dirX, float dirY);

#endif