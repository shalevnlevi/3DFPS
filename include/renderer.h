#pragma once
#include "player.h"
#include "map.h"
#include "monster.h"
void render3DView(const Player& player,
                  const char* map, int mapWidth, int mapHeight,
                  const Monster& monster); 