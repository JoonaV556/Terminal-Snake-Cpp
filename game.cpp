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
    int snakeX;
    int snakeY;
    MoveDirection snakeDir = none;

    bool isRunning = true;

    vector<vector<char>>
        world;

/*
 * Builds / re-populates world with borders and empty tiles according to declared map size.
 * Can also be used to refresh world data from previous update (clearing trails left behind by snake)
 */
    void refreshWorld() {
        for (int y = 0; y < world.size(); y++)
        {
            for (int x = 0; x < world[0].size(); x++)
            {
                bool isTopBorder    =   (y == 0);
                bool isLowerBorder  =   (y == world.size() - 1);
                bool isLeftBorder   =   (x == 0);
                bool isRightBorder  =   (x == world[y].size() - 1);

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
    }

    void init()
    {
        // initialize world with defined size
        world = vector(
            mapHeight + 2,               // add 2 extra for borders
            vector<char>(mapWidth + 2)); // add 2 extra for borders

        // populate world with borders and empty spaces
        // TODO - take into account possibly different horizontal & vertical size (see !!)
        refreshWorld();

        // place player at map center at start
        snakeX = (mapWidth + 2) / 2;
        snakeY = (mapHeight + 2) / 2;
    }

    void update()
    {
        // clear world tiles from previous update
        refreshWorld();
        // update snake location
        updateSnake();
        render();
    }

    void updateSnake()
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

        // move snake based on its current direction
        switch (snakeDir)
        {
        case MoveDirection::none:
            /* code */
            break;
        case MoveDirection::left:
            snakeX--;
            break;
        case MoveDirection::right:
            snakeX++;
            break;
        case MoveDirection::up:
            snakeY--;
            break;
        case MoveDirection::down:
            snakeY++;
            break;
        default:
            break;
        }

        // limit snake location inside map borders
        if (snakeX <= 0) {snakeX = 1;}
        if (snakeX > mapWidth) {snakeX = mapWidth;}
        if (snakeY <= 0) {snakeY = 1;}
        if (snakeY > mapHeight) {snakeY = mapHeight;}

        // place snake on associated map tile
        bool xIn = snakeX >= 0 && snakeX < world[0].size();
        bool yIn = snakeY >= 0 && snakeY < world.size();
        if (xIn && yIn)
        {
            world[snakeY][snakeX] = snake;
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
