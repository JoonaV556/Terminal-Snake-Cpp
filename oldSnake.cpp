#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <conio.h>

using namespace std;

enum MoveDirection
{
    none,
    left,
    right,
    up,
    down
};
class Snake
{

public:
    int mapWidth = 11;
    int mapHeight = 11;
    char mapBorder = '#';
    char snake = 'o';
    int playerX;
    int playerY;
    int updates = 0;
    MoveDirection snakeDir = MoveDirection::up;

    bool isRunning = true;

    vector<vector<char>>
        world;

    void init()
    {

        // initialize world with defined size
        world = vector<vector<char>>(
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
        system("cls");
        updatePlayer();
        render();
        updates++;
        cout << "updated " + to_string(updates) + " times" << endl;
    }

    void updatePlayer()
    {
        // update move dir
        if (_kbhit())
        {
            switch (int key = _getch())
            {
            case 72:
                snakeDir = MoveDirection::up;
                break;
            case 77:
                snakeDir = MoveDirection::right;
                break;
            case 80:
                snakeDir = MoveDirection::down;
                break;
            case 75:
                snakeDir = MoveDirection::left;
                break;
            default:
                break;
            }
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
