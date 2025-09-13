/**
  #include "monster.h"
#include <cmath>

Monster::Monster(float startX, float startY)
    : x(startX), y(startY), dirX(0.0f), dirY(1.0f) {}


static bool isWalkable(int x, int y, const char* map, int mapWidth, int mapHeight) {
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) return false;
    return map[y * mapWidth + x] == '.';
}

void Monster::moveTowards(float targetX, float targetY, const char* map, int mapWidth, int mapHeight, float speed) {
    float dx = targetX - x;
    float dy = targetY - y;
    float len = std::sqrt(dx*dx + dy*dy);
    if (len < 0.01f) return;
    dx /= len; dy /= len;
    float newX = x + dx * speed;
    float newY = y + dy * speed;
    if (isWalkable((int)newX, (int)newY, map, mapWidth, mapHeight)) {
        x = newX; y = newY;
    }
    dirX = dx; dirY = dy;
}
**/