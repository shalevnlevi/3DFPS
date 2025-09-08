#ifndef PLAYER_H
#define PLAYER_H


struct Player {
    float x, y;
    float dirX, dirY;

    Player(float startX, float startY);

    void moveForward(float speed, const char* map, int mapWidth, int mapHeight);
    void moveBackward(float speed, const char* map, int mapWidth, int mapHeight);
    void rotateLeft(float angle);
    void rotateRight(float angle);
};

#endif