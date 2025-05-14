#include <iostream>
#include <string>
#include <vector>
#include <winuser.h>

using namespace std;

enum MoveDirection
{
    none,
    left,
    right,
    up,
    down
};
class Game
{

public:
    int mapWidth = 11;
    int mapHeight = 11;
    char mapBorder = '#';
    char snake = 'o';
    int playerX;
    int playerY;
    MoveDirection snakeDir = none;

    bool isRunning = true;

    vector<vector<char>>
        world;

    void init()
    {

        // initialize world with defined size
        world = vector(
            mapHeight + 2,               // add 2 extra for borders
            vector<char>(mapWidth + 2)); // add 2 extra for borders

        // populate world with borders and empty spaces
        // TODO - take into account possibly different horizontal & vertical size (see !!)
        for (int y = 0; y < world.size(); y++)
        {
            for (int x = 0; x < world[0].size(); x++)
            {
                bool isTopBorder = (y == 0);
                bool isLowerBorder = (y == world.size() - 1);
                bool isLeftBorder = (x == 0);
                bool isRightBorder = (x == world[y].size() - 1);

                // populate borders
                if (isTopBorder || isLowerBorder || isLeftBorder || isRightBorder)
                {
                    world[y][x] = mapBorder;
                    continue;
                }

                // contents (empty tile)
                world[y][x] = ' ';
            }
        }

        // place player at map center at start
        playerX = (mapWidth + 2) / 2;
        playerY = (mapHeight + 2) / 2;
    }

    void update()
    {
        updatePlayer();
        render();
    }

    void updatePlayer()
    {
        // update snake move direction based on player input
        // if GetAsyncKeyState(VK_EXAMPLE_KEY) & 0x8000 == true - means EXAMPLE_KEY is currently pressed down
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            snakeDir = MoveDirection::left;
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            snakeDir = MoveDirection::right;
        }
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            snakeDir = MoveDirection::up;
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            snakeDir = MoveDirection::down;
        }

        // move player
        switch (snakeDir)
        {
        case MoveDirection::none:
            /* code */
            break;
        case MoveDirection::left:
            playerX--;
            break;
        case MoveDirection::right:
            playerX++;
            break;
        case MoveDirection::up:
            playerY--;
            break;
        case MoveDirection::down:
            playerY++;
            break;
        default:
            break;
        }

        // limit player location inside map borders

        bool xIn = playerX >= 0 && playerX < world[0].size();
        bool yIn = playerY >= 0 && playerY < world.size();
        if (xIn && yIn)
        {
            world[playerY][playerX] = snake;
        }
    }

    void render()
    {
        // render world data
        for (int y = 0; y < world.size(); y++)
        {
            // assemble world row into a string
            string rowStr;
            rowStr.assign(world[y].begin(), world[y].end());
            // print row
            cout << rowStr << endl;
        }
    }
};
