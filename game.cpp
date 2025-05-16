#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <cstdlib>
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
    // map properties
    int mapWidth = 12;
    int mapHeight = 12;
    bool enableHorizontalPadding = false;
    char mapBorderVisual = '#';

    // snake properties
    char snakeVisual = 'o';
    MoveDirection snakeDir = none;
    vector<tuple<int, int>> snake =
        {
        make_tuple(0, 0), // init head
        };

    // score properties
    tuple<int, int> scorePosition = make_tuple(0, 0);
    char scoreVisual = '*';
    int score = 0;

    // how fast the game runs / how many updates per second
    float gameUpdatesPerSecond = 2;
    float lastUpdateTime = 0;
    float elapsedGameTime = 0;

    bool isGameRunning = true;

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
                    world[y][x] = mapBorderVisual;
                    continue;
                }

                // contents (empty tile)
                world[y][x] = ' ';
            }
        }
    }

    void placeScoreInRandomEmptyPosition() {
        // get free tiles ()
        vector<tuple<int, int>> freeTiles; // x, y
        for (int y = 1; y <= mapHeight; y++) {
            for (int x = 1; x <= mapWidth; x++) {
                bool isFree = true;
                for (tuple<int, int> member : snake) {
                    if (y != get<1>(member) && x != get<0>(member)) {
                        isFree = false;
                        break;
                }
                    if (isFree) {
                        freeTiles.push_back(make_tuple(x, y));
                    }
                }
            }
        }
        // pick random from free tiles and place score there
        int randomTile = rand() % freeTiles.size();
        get<0>(scorePosition) = get<0>(freeTiles[randomTile]);
        get<1>(scorePosition) = get<1>(freeTiles[randomTile]);
    }

    void init()
    {
        // initialize world with defined size
        world = vector(
            mapHeight + 2,               // add 2 extra for borders
            vector<char>(mapWidth + 2)); // add 2 extra for borders

        // populate world with borders and empty spaces
        refreshWorld();

        // place player at map center at start
        setSnakeMemberPosition(0, (mapWidth + 2) / 2, (mapHeight + 2) / 2);

        // place score point
        placeScoreInRandomEmptyPosition();

        // ask player for optional horizontal padding
        cout << "NOTE: " << endl;
        cout << "The game level might look asymmetrical depending on terminal styling. You can combat the issue by adding horizontal padding." << endl;
        cout << "Type '1' to enable padding or '0' to disable:" << endl;
        string choice;
        cin >> choice;
        if (choice == "1") {
            enableHorizontalPadding = true;
        }
    }

    // Assigns given position to snake member - does not place it in the world tile
    // memberIndex 0 = snake head
    void setSnakeMemberPosition(int memberIndex, int x, int y)
    {
        get<0>(snake[memberIndex]) = x;
        get<1>(snake[memberIndex]) = y;
    }

    // places snake members in world on their given coords
    void placeSnakeInWorld() {
        for (int i = 0; i < snake.size(); i++) {
            world[get<1>(snake[i])][get<0>(snake[i])] = snakeVisual;
        }
    }

    tuple<int, int> getSnakePos(int memberIndex) {
        return snake[memberIndex];
    }

    void update(float elapsedTime)
    {
        // update snake move-direction based on player input
        updateMoveDirection();

        elapsedGameTime = elapsedTime;
        float secondsBetweenUpdates = 1 / gameUpdatesPerSecond;
        if (elapsedGameTime - lastUpdateTime > secondsBetweenUpdates) {
            // clear screen
            system("cls");
            // clear world tiles from previous update
            refreshWorld();

            // place score
            world[get<1>(scorePosition)][get<0>(scorePosition)] = scoreVisual;

            // update snake location
            updateSnake();

            // consume score point if snake hits one
            int snakeX = get<0>(snake[0]);
            int snakeY = get<1>(snake[0]);
            if (snakeX == get<0>(scorePosition) && snakeY == get<1>(scorePosition)) {
                score++;
                // spawn new score point / place score in another tile
                placeScoreInRandomEmptyPosition();

                // todo - grow snake
            }

            render();
            lastUpdateTime = elapsedGameTime;
        }
    }

    // updates snake move direction based on user input
    void updateMoveDirection() {

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
    }

    void updateSnake()
    {
        // move snake based on its current direction
        switch (snakeDir)
        {
        case MoveDirection::none:
            /* code */
            break;
        case MoveDirection::left:
                setSnakeMemberPosition(0, get<0>(snake[0])-1, get<1>(snake[0])  );
            break;
        case MoveDirection::right:
                setSnakeMemberPosition(0, get<0>(snake[0])+1, get<1>(snake[0])  );
            break;
        case MoveDirection::up:
                setSnakeMemberPosition(0, get<0>(snake[0]), get<1>(snake[0])-1  );
            break;
        case MoveDirection::down:
                setSnakeMemberPosition(0, get<0>(snake[0]), get<1>(snake[0])+1  );
            break;
        default:
            break;
        }

        // get snake head position
        int snakeX = get<0>(snake[0]);
        int snakeY = get<1>(snake[0]);

        // limit snake location inside map borders
        if (snakeX <= 0) {
            setSnakeMemberPosition(0, 1, snakeY);
        }
        if (snakeX > mapWidth) {
            setSnakeMemberPosition(0, mapWidth, snakeY);
        }
        if (snakeY <= 0) {
            setSnakeMemberPosition(0, snakeX, 1);
        }
        if (snakeY > mapHeight) {
            setSnakeMemberPosition(0, snakeX, mapHeight);
        }

        // place snake members in world
        placeSnakeInWorld();
    }

    void render()
    {
        // render world data
        for (int y = 0; y < world.size(); y++)
        {
            // assemble world row into a printable string
            string rowStr;
            rowStr.assign(world[y].begin(), world[y].end());

            // add gaps between tiles
            // todo - \/ \/ fix horrible mess \/ \/
            string withGaps(rowStr.size()*2-1, '_'); // create new empty string with correct size
            bool add = false;
            int oIndex = 0;
            // populate new string with gaps in between
            for (int g = 0; g < withGaps.size() ; g++) {
                if (!add) {
                    add = true;
                    withGaps[g] = rowStr[oIndex];
                    oIndex++;
                    continue;
                }
                add = false;
                if (g == withGaps.size() - 1) { break;}
                withGaps[g] = ' ';
            }

            // print row
            if (enableHorizontalPadding) {
                cout << withGaps << "\n";
            } else {
                cout << rowStr << "\n";
            }
        }
        // render score
        cout << "Score: " << score << endl;
    }
};
