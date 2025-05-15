#include <iostream>
#include <windows.h>
#include <bits/ostream.tcc>
#include <chrono>
#include <thread>
#include <winuser.h>

#include "game.cpp"

using namespace std;
using namespace std::chrono;

int updates = 0;

bool isRunning = true;

float lastUpdateTime = 0; // time it took to execute last update
float elapsedTimeSeconds = 0;
time_point<high_resolution_clock> startupTime;

int main(int argc, char *argv[]) {

    // create game
    Game game = Game();
    game.init();

    constexpr int updatesPerSecond = 100; // per second
    const float sleepTime = 1.0f / updatesPerSecond; // in seconds
    startupTime = high_resolution_clock::now();

    // update game until it is over
    while (game.isGameRunning)
    {
        // end game if esc key is pressed down
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }

        auto start = std::chrono::high_resolution_clock::now();
        // update game
        game.update(elapsedTimeSeconds);
        auto end = high_resolution_clock::now();
        duration<double> elapsed = end - start;
        lastUpdateTime = elapsed.count();
        updates++;

        // print general info below game grid
        // std::cout << "update duration was: " + std::to_string(lastUpdateTime) + " seconds" << "\n";
        // std::cout << "updated " + std::to_string(updates) + " times" << "\n";

        // update elapsed time
        duration<double> totalDuration = high_resolution_clock::now() - startupTime;
        elapsedTimeSeconds = totalDuration.count();
        // std::cout << "total runtime in seconds: " + std::to_string(elapsedTimeSeconds) << "\n";

        // wait before executing next update
        float actualWaitTime = sleepTime - lastUpdateTime;
        if (actualWaitTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<float>(actualWaitTime));
        }
    }

    return 0;
}