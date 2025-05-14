#include <iostream>
#include <windows.h>
#include <bits/ostream.tcc>
#include <chrono>
#include "game.cpp"


int updates = 0;
bool isRunning = true;
float lastUpdateTime = 0; // time it took to execute last update


int main(int argc, char *argv[]) {

    Game game = Game();
    game.init();

    constexpr int updatesPerSecond = 100; // per second
    const float sleepTime = 1.0f / updatesPerSecond; // in seconds

    while (game.isRunning)
    {
        auto start = std::chrono::high_resolution_clock::now();
        // clear screen
        system("cls");

        // update game
        game.update();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        lastUpdateTime = elapsed.count();
        updates++;

        std::cout << "update duration was: " + std::to_string(lastUpdateTime) + " seconds" << "\n";
        std::cout << "updated " + std::to_string(updates) + " times" << "\n";

        // wait before executing next update
        float actualWaitTime = sleepTime - lastUpdateTime;
        if (actualWaitTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<float>(actualWaitTime));
        }
    }
    return 0;
}