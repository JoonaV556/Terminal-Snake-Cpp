#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <deque>
#include <winuser.h>

using namespace std;

enum MoveDirection {
    none,
    left,
    right,
    up,
    down
};

class Game {
public:
    // map properties
    int mapWidth = 12;
    int mapHeight = 12;
    bool enableHorizontalPadding = false;
    char mapBorderVisual = '#';

    // snake properties
    char snakeVisual = 'o';
    int snakeLength = 0;
    MoveDirection snakeDir = none;
    deque<tuple<int, int> > headPositionHistory = deque<tuple<int, int> >();
    const int maxHeadPositionHistoryLength = 999;
    vector<tuple<int, int> > snake =
    {
        make_tuple(0, 0), // init head
    };
    tuple<int, int> tailLastPosition = make_tuple(0, 0);

    // score properties
    tuple<int, int> scorePosition = make_tuple(0, 0);
    char scoreVisual = '*';
    int score = 0;
    int highScore = 0;

    // how fast the game runs / how many updates per second
    float gameUpdatesPerSecond = 2;
    float lastUpdateTime = 0;
    float elapsedGameTime = 0;

    bool isGameRunning = true;

    vector<vector<char> >
    world;

    /*
     * Builds / re-populates world with borders and empty tiles according to declared map size.
     * Can also be used to refresh world data from previous update (clearing trails left behind by snake)
     */
    void refreshWorld() {
        for (int y = 0; y < world.size(); y++) {
            for (int x = 0; x < world[0].size(); x++) {
                bool isTopBorder = (y == 0);
                bool isLowerBorder = (y == world.size() - 1);
                bool isLeftBorder = (x == 0);
                bool isRightBorder = (x == world[y].size() - 1);

                // populate borders
                if (isTopBorder || isLowerBorder || isLeftBorder || isRightBorder) {
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
        vector<tuple<int, int> > freeTiles; // x, y
        for (int y = 1; y <= mapHeight; y++) {
            for (int x = 1; x <= mapWidth; x++) {
                // snake in tile
                if (world[y][x] != snakeVisual) {
                    freeTiles.push_back(make_tuple(x, y));
                }
            }
        }
        // pick random from free tiles and place score there
        int randomTile = rand() % freeTiles.size();
        get<0>(scorePosition) = get<0>(freeTiles[randomTile]);
        get<1>(scorePosition) = get<1>(freeTiles[randomTile]);
    }

    void init() {
        system("cls");

        // initialize world with defined size
        world = vector(
            mapHeight + 2, // add 2 extra for borders
            vector<char>(mapWidth + 2)); // add 2 extra for borders

        // populate world with borders and empty spaces
        refreshWorld();

        // place snake at map center
        snakeLength = 0;
        setSnakeMemberPosition(0, (mapWidth + 2) / 2, (mapHeight + 2) / 2);
        placeSnakeInWorld();
        snakeDir = none;

        // place score point
        placeScoreInRandomEmptyPosition();

        // ask player for optional horizontal padding
        cout << "NOTE: " << endl;
        cout <<
                "The game level might look asymmetrical depending on terminal styling. You can combat the issue by adding horizontal padding."
                << endl;
        cout << "Type '1' to enable padding or '0' to disable:" << endl;
        string choice;
        cin >> choice;
        if (choice == "1") {
            enableHorizontalPadding = true;
        }
    }

    // Assigns given position to snake member - does not place it in the world tile
    // memberIndex 0 = snake head
    void setSnakeMemberPosition(int memberIndex, int x, int y) {
        get<0>(snake[memberIndex]) = x;
        get<1>(snake[memberIndex]) = y;
    }

    // places snake members in world on their given coords
    void placeSnakeInWorld() {
        for (int i = 0; i < snake.size(); i++) {
            world[get<1>(snake[i])][get<0>(snake[i])] = snakeVisual;
        }
    }

    void gameOverReset() {
        cout << "Game Over! Suckass..." << endl;
        cout << "Score: " << score << endl;
        // update hi score
        if (score > highScore) {
            highScore = score;
            cout << "New high score! " << highScore << endl;
        }
        cout << " " << endl;
        cout << "Enter anything to continue...";
        string poo;
        cin >> poo;
        score = 0;
        init();
    }

    tuple<int, int> getSnakePos(int memberIndex) {
        return snake[memberIndex];
    }

    void update(float elapsedTime) {
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

            render();
            lastUpdateTime = elapsedGameTime;
        }
    }

    // updates snake move direction based on user input
    void updateMoveDirection() {
        // if GetAsyncKeyState(VK_EXAMPLE_KEY) & 0x8000 == true - means EXAMPLE_KEY is currently pressed down
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            if (snakeDir != MoveDirection::right) {
                snakeDir = MoveDirection::left;
            }
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            if (snakeDir != MoveDirection::left) {
                snakeDir = MoveDirection::right;
            }
        }
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            if (snakeDir != MoveDirection::down) {
                snakeDir = MoveDirection::up;
            }
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            if (snakeDir != MoveDirection::up) {
                snakeDir = MoveDirection::down;
            }
        }
    }

    void consumeScore() {
        score++;
        snakeLength++;
        // spawn new score point / place score in another tile
        placeScoreInRandomEmptyPosition();
    }

    void updateSnake() {
        int xChange = 0;
        int yChange = 0;
        switch (snakeDir) {
            case MoveDirection::none:
                /* code */
                break;
            case MoveDirection::left:
                xChange = -1;
                break;
            case MoveDirection::right:
                xChange = 1;
                break;
            case MoveDirection::up:
                yChange = -1;
                break;
            case MoveDirection::down:
                yChange = 1;
                break;
            default:
                break;
        }

        // check next position
        int nextX = get<0>(snake[0]) + xChange;
        int nextY = get<1>(snake[0]) + yChange;
        // store previous position for tail growth
        int xBeforeMove = get<0>(snake[0]);
        int yBeforeMove = get<1>(snake[0]);
        char nextTile = world[nextY][nextX];
        // hit wall or snake
        if (nextTile == mapBorderVisual || nextTile == snakeVisual) {
            gameOverReset();
            return;
        }

        // move snake
        // update head position
        setSnakeMemberPosition(0, nextX, nextY);
        world[get<1>(snake[0])][get<0>(snake[0])] = snakeVisual;

        // update position history for growth purposes
        tuple<int, int> headPositionPreMove = make_tuple(xBeforeMove, yBeforeMove);
        headPositionHistory.push_front(headPositionPreMove);
        // keep history size under control
        if (headPositionHistory.size() > maxHeadPositionHistoryLength) {
            headPositionHistory.pop_back();
        }

        // update tail member positions
        for (int i = 0; i < snakeLength; i++) {
            tuple<int, int> memberPos = headPositionHistory[i];
            // check if snake has eaten itself
            if (get<0>(memberPos) == get<0>(snake[0]) && get<1>(memberPos) == get<1>(snake[0])) {
                // game over
                gameOverReset();
                return;
            }
            // place member
            world[get<1>(memberPos)][get<0>(memberPos)] = snakeVisual;
        }

        // hit score
        if (nextTile == scoreVisual) {
            consumeScore();
        }
    }

    void render() {
        // render world data
        for (int y = 0; y < world.size(); y++) {
            // assemble world row into a printable string
            string rowStr;
            rowStr.assign(world[y].begin(), world[y].end());

            // add gaps between tiles
            // todo - \/ \/ fix horrible mess \/ \/
            string withGaps(rowStr.size() * 2 - 1, '_'); // create new empty string with correct size
            bool add = false;
            int oIndex = 0;
            // populate new string with gaps in between
            for (int g = 0; g < withGaps.size(); g++) {
                if (!add) {
                    add = true;
                    withGaps[g] = rowStr[oIndex];
                    oIndex++;
                    continue;
                }
                add = false;
                if (g == withGaps.size() - 1) { break; }
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
        cout << "Hi-Score: " << highScore << endl;
        // ESC to quit message
        cout << "Press ESC to quit" << endl;
    }
};
