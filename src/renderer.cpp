#include "renderer.h"
#include "map.h"
#include "player.h"
#include "monster.h"
#include <iostream>
#include <algorithm>
#include <cmath>

struct RayHit {
    int mapX, mapY;
    int side;            // 0 = vertical, 1 = horizontal
    float perpWallDist;
};

static constexpr int kScreenWidth  = 80;
static constexpr int kScreenHeight = 24;
static constexpr float kShadingScale = 4.0f;

static float zBuffer[kScreenWidth]; // wall distances

// --- Raycasting ---
static RayHit castRay(float startX, float startY,
                      float rayDirX, float rayDirY,
                      const char* map, int mapWidth, int mapHeight)
{
    int mapX = int(startX);
    int mapY = int(startY);

    float deltaDistX = (rayDirX == 0) ? 1e30f : std::abs(1.0f / rayDirX);
    float deltaDistY = (rayDirY == 0) ? 1e30f : std::abs(1.0f / rayDirY);

    int stepX, stepY;
    float sideDistX, sideDistY;

    if (rayDirX < 0) { stepX = -1; sideDistX = (startX - mapX) * deltaDistX; }
    else             { stepX =  1; sideDistX = (mapX + 1.0f - startX) * deltaDistX; }

    if (rayDirY < 0) { stepY = -1; sideDistY = (startY - mapY) * deltaDistY; }
    else             { stepY =  1; sideDistY = (mapY + 1.0f - startY) * deltaDistY; }

    int side = 0;
    bool hit = false;

    while (!hit) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX; mapX += stepX; side = 0;
        } else {
            sideDistY += deltaDistY; mapY += stepY; side = 1;
        }

        if (mapY < 0 || mapY >= mapHeight || mapX < 0 || mapX >= mapWidth) {
            hit = true; break; // outside map
        }
        if (isWall(map[mapY * mapWidth + mapX])) hit = true;
    }

    float perpWallDist = (side == 0) ? (sideDistX - deltaDistX)
                                     : (sideDistY - deltaDistY);

    return {mapX, mapY, side, perpWallDist};
}

// --- Wall shading ---
static std::string shadeWall(const RayHit& hit, const char* map,
                             int mapWidth, int mapHeight)
{
    char tile = map[hit.mapY * mapWidth + hit.mapX];
    WallType type = getWallType(tile);
    WallColorInfo ci = getWallColorInfo(type);

    const char* blockChar = (hit.side == 0) ? "▓" : "▒";

    if (type == WALL_GREY) {
        int shade = 232 + std::min(23, int(hit.perpWallDist * kShadingScale));
        shade = std::clamp(shade, 232, 255);
        return "\033[38;5;" + std::to_string(shade) + "m" + blockChar + "\033[0m";
    } else {
        int distanceStep = std::min(ci.range, int(hit.perpWallDist * (ci.range / 6.0f)));
        int shade = std::clamp(ci.base - distanceStep, 16, 231);
        return "\033[38;5;" + std::to_string(shade) + "m" + blockChar + "\033[0m";
    }
}

// --- Main render ---
void render3DView(const Player& player,
                  const char* map, int mapWidth, int mapHeight,
                  const Monster& monster)
{
    float planeX = -player.dirY * 0.66f;
    float planeY =  player.dirX * 0.66f;

    // ==== PASS 1: walls ====
    for (int y = 0; y < kScreenHeight; ++y) {
        for (int x = 0; x < kScreenWidth; ++x) {
            if (y == 0) { // only cast once per column
                float cameraX = 2.0f * x / float(kScreenWidth) - 1.0f;
                float rayDirX = player.dirX + planeX * cameraX;
                float rayDirY = player.dirY + planeY * cameraX;

                RayHit hit = castRay(player.x, player.y, rayDirX, rayDirY,
                                     map, mapWidth, mapHeight);

                int lineHeight = int(kScreenHeight / (hit.perpWallDist + 1e-4f));
                lineHeight = std::min(lineHeight, kScreenHeight);

                int drawStart = std::max(0, -lineHeight/2 + kScreenHeight/2);
                int drawEnd   = std::min(kScreenHeight - 1, lineHeight/2 + kScreenHeight/2);

                zBuffer[x] = hit.perpWallDist; // store distance

                // store rendering info per column
                for (int yy = 0; yy < kScreenHeight; ++yy) {
                    if (yy < drawStart) {
                        std::cout << ' ';
                    } else if (yy > drawEnd) {
                        std::cout << '.';
                    } else {
                        std::cout << shadeWall(hit, map, mapWidth, mapHeight);
                    }
                }
            }
        }
        std::cout << '\n';
    }

    // ==== PASS 2: monster ====
    float dx = monster.x - player.x;
    float dy = monster.y - player.y;

    float invDet = 1.0f / (planeX * player.dirY - player.dirX * planeY);
    float transformX = invDet * ( player.dirY * dx - player.dirX * dy );
    float transformY = invDet * ( -planeX * dx + planeX * dy );

    if (transformY > 0) {
        int spriteScreenX = int((kScreenWidth / 2) * (1 + transformX / transformY));

        int spriteHeight = std::abs(int(kScreenHeight / transformY));
        int spriteWidth  = spriteHeight;

        int drawStartY = std::max(0, -spriteHeight / 2 + kScreenHeight / 2);
        int drawEndY   = std::min(kScreenHeight - 1, spriteHeight / 2 + kScreenHeight / 2);
        int drawStartX = std::max(0, -spriteWidth / 2 + spriteScreenX);
        int drawEndX   = std::min(kScreenWidth - 1, spriteWidth / 2 + spriteScreenX);

        // Draw sprite column by column
        for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
            if (transformY < zBuffer[stripe]) {
                for (int y = drawStartY; y < drawEndY; y++) {
                    std::cout << "\033[" << (y+1) << ";" << (stripe+1) << "H";
                    std::cout << "\033[38;5;196mM\033[0m";
                }
            }
        }
        std::cout.flush();
    }
}
