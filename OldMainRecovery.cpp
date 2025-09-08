
#include <iostream>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <cmath>



const int mapWidth = 24, mapHeight = 24;
const char map[] =
    "########################"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "#......................#"
    "########################";

float playerX = 12.0f;
float playerY = 12.0f;
float dirX = 1.0f;
float dirY = 0.0f;
float moveSpeed = 0.2f;
float rotSpeed = 0.3f; // Radians per keypress


void printMapWithPlayer() {
    int intPlayerX = playerX;
    int intPlayerY = playerY;
    for (int y=0; y<mapHeight; y++){
        for (int x=0; x<mapWidth; x++){
            if (intPlayerX == x && intPlayerY == y)
                std::cout << 'P';
            else
                std::cout << map [y * mapHeight + x];
            
        }
        std::cout << std::endl;
    }
}

void clearScreen() {
    std::cout << "\x1b[2J\x1b[H";
}

void setNonCanonicalMode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt); // save old terminal settings
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // disable canonical mode & echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restore old settings
    }
}

// Check if a key was pressed (non-blocking)
int kbhit() {
    struct timeval tv = {0, 0};
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
}

// Get a character without waiting
char getch() {
    char c;
    if (read(STDIN_FILENO, &c, 1) < 0) return 0;
    return c;
}

bool isWalkable(int x, int y) {
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
        return false;
    return map[y * mapWidth + x] == '.';
}

uint64_t getTicks() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main(){
    setNonCanonicalMode(true);   // enable raw input
    bool running = true;
    uint64_t oldTime = getTicks();
    uint64_t time = getTicks();


    while (running){
        oldTime = time;
        time = getTicks();
        double frameTime = (time - oldTime) / 1000.0; // seconds
        double moveSpeed = frameTime * 5.0; // squares/second
        double rotSpeed = frameTime * 3.0; // radians/second

        clearScreen();
        printMapWithPlayer();
        std::cout << "Player X: " << playerX << " Y: " << playerY
                  << " Facing: (" << dirX << ", " << dirY << ")\n";
        std::cout << "FPS: " << (1.0 / frameTime) << "\n";
        std::cout << "Move with WASD, q to quit: ";
        if (kbhit()) {
            char c = getch();
            float newX = playerX;
            float newY = playerY;
            //To rotate we multiply the vector with rotation matrix:
            // [ cos(a) -sin(a) ]
            // [ sin(a)  cos(a) ]
            switch (c) {
                case 'w':
                {
                    std::cout << "Move forward\n";
                    newX += dirX * moveSpeed;
                    newY += dirY * moveSpeed;
                    std::cout << "newXXXXXXXXXX = " << newX <<std::endl;
                    break;
                }
                case 's':
                {
                    std::cout << "Move back\n";
                    newX -= dirX * moveSpeed;
                    newY -= dirY * moveSpeed;
                    break;
                }
                case 'a':
                {
                    std::cout << "Rotate Left\n";
                    float oldDirX = dirX;
                    dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
                    dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
                    // Normalize it to be a unit vector
                    float len = std::sqrt(dirX * dirX + dirY * dirY);
                    dirX /= len;
                    dirY /= len;
                    break;
                }
                case 'd':
                {
                    std::cout << "Rotate right\n";
                    float oldDirX = dirX;
                    dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
                    dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
                    // Normalize it to be a unit vector
                    float len = std::sqrt(dirX * dirX + dirY * dirY);
                    dirX /= len;
                    dirY /= len;
                    break;
                    break;
                }
                    
                case 'q': running = false; break; // quit
            }
            if (isWalkable(newX, newY)) {
                std::cout << "success @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
                std::cout << "OldX = " << playerX <<std::endl;

                playerX = newX;
                playerY = newY;

                std::cout << "newX = " << newX <<std::endl;

            }
        }
        usleep(160000); // ~60 FPS

        // Optional: add a small delay for smoother experience
        // std::this_thread::sleep_for(std::chrono::milliseconds(100))
    }
        setNonCanonicalMode(false);
        return 0;
    
}

const char map1[] =
    "########################"
    "#..........#...........#"
    "#..........#...........#"
    "#......................#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#..........#...........#"
    "#......................#"
    "#......................#"
    "#......................#"
    "########################";