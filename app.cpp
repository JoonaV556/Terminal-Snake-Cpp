#include "snake.cpp"

int main()
{
    Snake snake = Snake();
    snake.init();
    float updateSpeed = 1; // per second
    float waitTime = 1.0 / updateSpeed;

    while (snake.isRunning)
    {
        snake.update();
        // wait for
        // this_thread::sleep_for(chrono::duration<float>(waitTime));
        Sleep(waitTime * 1000);
    }

    // ask input before exitings
    cout << "Enter something to quit: ";
    string x;
    cin >> x;
    return 0;
};
