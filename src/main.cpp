#include <iostream>
#include <unistd.h>
#include "player.h"
#include "map.h"
#include "utils.h"

int main() {
    setNonCanonicalMode(true);
    Player player(12.0f, 12.0f);

    bool running = true;
    uint64_t oldTime = getTicks();
    uint64_t time = getTicks();

    while (running) {
        oldTime = time;
        time = getTicks();
        double frameTime = (time - oldTime) / 1000.0;
        double moveSpeed = frameTime * 5.0;
        double rotSpeed = frameTime * 3.0;

        clearScreen();
        printMapWithPlayer((int)player.x, (int)player.y);
        std::cout << "Player X: " << player.x << " Y: " << player.y
                  << " Facing: (" << player.dirX << ", " << player.dirY << ")\n";
        std::cout << "FPS: " << (1.0 / frameTime) << "\n";
        std::cout << "Move with WASD, q to quit: ";

        if (kbhit()) {
            char c = getch();
            switch (c) {
                case 'w': player.moveForward(moveSpeed, map, mapWidth, mapHeight); break;
                case 's': player.moveBackward(moveSpeed, map, mapWidth, mapHeight); break;
                case 'a': player.rotateLeft(rotSpeed); break;
                case 'd': player.rotateRight(rotSpeed); break;
                case 'q': running = false; break;
            }
        }

        usleep(16000); // ~60 FPS
    }

    setNonCanonicalMode(false);
    return 0;
}
