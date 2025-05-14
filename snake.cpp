#include <iostream>
#include <windows.h>
#include <bits/ostream.tcc>
#include "oldSnake.cpp"


int updateCount = 0;
bool isRunning = true;
void update() {
    updateCount++;
    std::cout << "updated " + std::to_string(updateCount) + " times" << "\n";
}

int main(int argc, char *argv[]) {

    Snake snake = Snake();
    snake.init();

    constexpr float updatesPerSecond = 100; // per second
    const float waitTime = 1.0f / updatesPerSecond;

    while (snake.isRunning)
    {
        snake.update();
        Sleep(waitTime * 1000);
    }

    return 0;
}

