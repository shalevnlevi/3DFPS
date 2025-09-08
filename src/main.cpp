#include <iostream>
#include <unistd.h>
#include "player.h"
#include "map.h"
#include "utils.h"

// === ASCII Raycasting Renderer ===
void render3DView(const Player& player, const char* map, int mapWidth, int mapHeight) {
    const int screenWidth = 80;
    const int screenHeight = 24;

    float planeX = -player.dirY * 0.66f;
    float planeY = player.dirX * 0.66f;

    // Directly print each row, no screen buffer needed since we're using ANSI colors
    for (int y = 0; y < screenHeight; ++y) {
        for (int x = 0; x < screenWidth; ++x) {
            // Ray setup for each column
            float cameraX = 2 * x / float(screenWidth) - 1;
            float rayDirX = player.dirX + planeX * cameraX;
            float rayDirY = player.dirY + planeY * cameraX;

            int mapX = int(player.x);
            int mapY = int(player.y);

            float deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1.0f / rayDirX);
            float deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1.0f / rayDirY);

            int stepX, stepY;
            float sideDistX, sideDistY;
            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (player.x - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0f - player.x) * deltaDistX;
            }
            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (player.y - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0f - player.y) * deltaDistY;
            }

            // DDA (Digital Differential Analyzer)
            bool hit = false;
            int side = 0;
            while (!hit) {
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                if (mapY >= 0 && mapY < mapHeight && mapX >= 0 && mapX < mapWidth) {
                    if (map[mapY * mapWidth + mapX] == '#') hit = true;
                } else {
                    hit = true;
                }
            }

            float perpWallDist = (side == 0) ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY);
            int lineHeight = int(screenHeight / (perpWallDist + 0.0001f));
            if (lineHeight > screenHeight) lineHeight = screenHeight;

            int drawStart = -lineHeight/2 + screenHeight/2;
            int drawEnd = lineHeight/2 + screenHeight/2;
            if (drawStart < 0) drawStart = 0;
            if (drawEnd >= screenHeight) drawEnd = screenHeight-1;

            // For each row, decide what to print
            if (y < drawStart) {
                std::cout << ' '; // Ceiling
            } else if (y > drawEnd) {
                std::cout << '.'; // Floor
            } else {
                // Wall: use grey shade based on distance
                // ANSI 256 grey: 232 (dark) to 255 (light)
                int shade = 232 + std::min(23, int(perpWallDist * 3));
                if (shade < 232) shade = 255;
                if (shade > 255) shade = 232;
                std::cout << "\033[38;5;" << shade << "m#\033[0m";
            }
        }
        std::cout << "\n";
    }
}
// === End Raycasting Renderer ===


int main() {
    setNonCanonicalMode(true);
    Player player(1.0f, 1.0f);

    bool running = true;
    uint64_t oldTime = getTicks();
    uint64_t time = getTicks();


    while (running) {
        oldTime = time;
        time = getTicks();
        double frameTime = (time - oldTime) / 1000.0;
        double moveSpeed = frameTime * 2*5.0;
        double rotSpeed = frameTime * 2*3.0;

        clearScreen();
        //printMapWithPlayer((int)player.x, (int)player.y);
        render3DView(player, map, mapWidth, mapHeight);      // ADD THIS

        std::cout << "Player X: " << player.x << " Y: " << player.y
                  << " Facing: (" << player.dirX << ", " << player.dirY << ")\n";
        std::cout << "FPS: " << (1.0 / frameTime) << "\n";
        std::cout << "Move with WASD, x to quit: "; 
        std::cout << "\nMINIMAP:\n";
        printMapWithPlayer((int)player.x, (int)player.y, player.dirX, player.dirY);

        if (kbhit()) {
            char c = getch();
            switch (c) {
                case 'w': player.moveForward(moveSpeed, map, mapWidth, mapHeight); break;
                case 's': player.moveBackward(moveSpeed, map, mapWidth, mapHeight); break;
                case 'a': player.moveLeft(moveSpeed, map, mapWidth, mapHeight); break;
                case 'd': player.moveRight(moveSpeed, map, mapWidth, mapHeight); break;
                case 'q': player.rotateLeft(rotSpeed); break;
                case 'e': player.rotateRight(rotSpeed); break;
                case 'x': running = false; break;
            }
        }

        usleep(16000); // ~60 FPS
    }

    setNonCanonicalMode(false);
    return 0;
}
