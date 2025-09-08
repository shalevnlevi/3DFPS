#include "player.h"
#include <cmath>

static bool isWalkable(int x, int y, const char* map, int mapWidth, int mapHeight) {
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) return false;
    return map[y * mapWidth + x] == '.';
}

Player::Player(float startX, float startY) 
    : x(startX), y(startY), dirX(0.0f), dirY(1.0f) {}

void Player::moveForward(float speed, const char* map, int mapWidth, int mapHeight) {
    float newX = x + dirX * speed;
    float newY = y + dirY * speed;
    if (isWalkable((int)newX, (int)newY, map, mapWidth, mapHeight)) {
        x = newX; y = newY;
    }
}

void Player::moveBackward(float speed, const char* map, int mapWidth, int mapHeight) {
    float newX = x - dirX * speed;
    float newY = y - dirY * speed;
    if (isWalkable((int)newX, (int)newY, map, mapWidth, mapHeight)) {
        x = newX; y = newY;
    }
}

void Player::moveLeft(float speed, const char* map, int mapWidth, int mapHeight) {
    //moving right should be in perp to dir vector
    float newX = x + dirY * speed;
    float newY = y - dirX * speed;
    if (isWalkable((int)newX, (int)newY, map, mapWidth, mapHeight)) {
        x = newX; y = newY;
    }
}
void Player::moveRight(float speed, const char* map, int mapWidth, int mapHeight) {
    float newX = x - dirY * speed;
    float newY = y + dirX * speed;
    if (isWalkable((int)newX, (int)newY, map, mapWidth, mapHeight)) {
        x = newX; y = newY;
    }
}

void Player::rotateLeft(float angle) {
    float oldDirX = dirX;
    dirX = dirX * cos(-angle) - dirY * sin(-angle);
    dirY = oldDirX * sin(-angle) + dirY * cos(-angle);
    float len = std::sqrt(dirX*dirX + dirY*dirY);
    dirX /= len; dirY /= len;
}

void Player::rotateRight(float angle) {
    float oldDirX = dirX;
    dirX = dirX * cos(angle) - dirY * sin(angle);
    dirY = oldDirX * sin(angle) + dirY * cos(angle);
    float len = std::sqrt(dirX*dirX + dirY*dirY);
    dirX /= len; dirY /= len;
}
