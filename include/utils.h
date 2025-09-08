#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

void clearScreen();
void setNonCanonicalMode(bool enable);
int kbhit();
char getch();
uint64_t getTicks();

#endif
