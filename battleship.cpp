#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

class Boat
{
private:
    char dir;
    int length;
    vector<int> boatrow;
    vector<int> boatcol;
    int hits;
    string name;

public:
    // default constructor
    Boat(char d, int lgth, vector<int> r, vector<int> c, int hit, string nme)
    {
        dir = d;
        length = lgth;
        boatrow = r;
        boatcol = c;
        hits = hit;
        name = nme;
    }

    // sets boat hit
    void setHit()
    {
        hits++;
    }

    // gets boat hit
    int getHit()
    {
        return hits;
    }

    // checks if boat has sunk
    int checkSunk(int BoatSize)
    {
        if (hits >= BoatSize)
        {
            return 9;
        }
        else
        {
            return 0;
        }
    }

    // gets boat coordinates on game board
    void boatCoordinates()
    {
        cout << "Coordinates for boat " << name << endl
             << endl;
        for (int i = 0; i < length; i++)
        {
            cout << "Grid [" << boatrow[i] << "][" << boatcol[i] << "]" << endl;
        }
        cout << endl;
    }

    // gets name of boat on bombed coordinate
    string getBoat(int r, int c)
    {
        for (int i = 0; i < length; i++)
        {
            if ((boatrow[i] == r) && (boatcol[i] == c))
            {
                return name;
            }
        }
        return "";
    }
};

enum BoatSize
{
    Destroyer = 2,
    Submarine = 3,
    Cruiser = 3,
    Battleship = 4,
    Carrier = 5
};

void initGrid(int grid[][10]);
void printBoard(int grid[][10]);
void printGameBoard(int grid[][10]);
int resetColAndRow(int col, int &row, int BoatSize, char dir);
char getDirection(int dir);
int checkSpaces(int grid[][10], int c, int r, int s, char dir);
void editGrid(int grid[][10], int col, int row, int BoatSize, char dir);
bool setBoat(int grid[][10], int BoatSize, int name, vector<Boat> &boatList);
bool setBoat2(int grid[][10], int BoatSize, int name, vector<Boat> &boatList);
void editBoatInfo(int grid[][10], int c, int r, int BoatSize, char dir, vector<Boat> &boatList, int name);
int playGame(int grid[][10], vector<Boat> &boatList);
int getSpace(int grid[][10], int row, int col);
int trackMisses();

int main()
{
    int play = 0;
    bool gameInProgress = true;
    int hits1 = 0, hits2 = 0;
    int misses = 0;
    int counter = 0;
    int grid[10][10];
    int grid2[10][10];
    vector<Boat> boatList;
    vector<Boat> boatList2;

    // initialize game boards for both players
    initGrid(grid);
    initGrid(grid2);
    // place boats onto Player1's game board
    setBoat(grid, Carrier, 1, boatList);
    setBoat(grid, Battleship, 2, boatList);
    setBoat(grid, Cruiser, 3, boatList);
    setBoat(grid, Submarine, 4, boatList);
    setBoat(grid, Destroyer, 5, boatList);
    // place boats onto Player2's game board
    setBoat2(grid2, Carrier, 1, boatList2);
    setBoat2(grid2, Battleship, 2, boatList2);
    setBoat2(grid2, Cruiser, 3, boatList2);
    setBoat2(grid2, Submarine, 4, boatList2);
    setBoat2(grid2, Destroyer, 5, boatList2);

    cout << "***** Welcome to Battleship! *****" << endl;
    cout << "Rules of the Game:" << endl;
    cout << "1. This is a two player game" << endl;
    cout << "2. There is a Player1 and a Player2" << endl;
    cout << "3. The objective of the game is to destroy the opposing player's fleet" << endl;
    cout << "4. Each player will take turns guessing the coordinates of the opposing player's fleet" << endl;
    cout << "5. Each player's board is 10x10 with a randomly placed Carrier (5 in length), Battleship (4 in length), Cruiser and Submarine (3 in length), and Destroyer (2 in length)" << endl;
    cout << "6. The board will show a 1 for a hit and a 9 for a miss" << endl << endl;

    cout << "Enter 1 to play the game: ";
    cin >> play;
    cin.sync();
    if (play == 1)
    {
        while (gameInProgress) // start Battleship game
        {
            if (counter % 2 == 0) // alternate turns between players
            {
                cout << "\nPlayer1's Turn:" << endl;
                playGame(grid, boatList);
                hits1 = boatList.at(0).getHit() + boatList.at(1).getHit() + boatList.at(2).getHit() + boatList.at(3).getHit() + boatList.at(4).getHit();
                if (hits1 == 17)
                {
                    gameInProgress = false;
                }
            }
            else
            {
                cout << "\nPlayer2's Turn:" << endl;
                playGame(grid2, boatList2);
                hits2 = boatList2.at(0).getHit() + boatList2.at(1).getHit() + boatList2.at(2).getHit() + boatList2.at(3).getHit() + boatList2.at(4).getHit();
                if (hits2 == 17)
                {
                    gameInProgress = false;
                }
            }

            counter++;
        }

        // misses = trackMisses() - 1;

        cout << "\nGame Over!" << endl;

        if (hits1 == 17)
        {
            cout << "\nCongrats Player1! You have destroyed all of Player2's fleet." << endl;
        }
        else
        {
            cout << "\nCongrats Player2! You have destroyed all of Player1's fleet." << endl;
        }
    }

    exit(0);
}

// initializes a game board grid
void initGrid(int grid[][10])
{
    for (int col = 0; col < 10; col++)
    {
        for (int row = 0; row < 10; row++)
        {
            grid[col][row] = 0;
        }
    }
}

// prints the game board with visible boats on it (for developer use)
void printBoard(int grid[][10])
{
    cout << "   0|1|2|3|4|5|6|7|8|9" << endl
         << endl;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (j == 0)
            {
                cout << i << "  ";
            }
            cout << grid[i][j];
            if (j != 9)
            {
                cout << "|";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// prints the game board without visible boats on it (for gameplay use)
void printGameBoard(int grid[][10])
{
    cout << "   0|1|2|3|4|5|6|7|8|9" << endl
         << endl;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (j == 0)
            {
                cout << i << "  ";
            }
            if (grid[i][j] == 1)
            {
                cout << 1;
            }
            else if (grid[i][j] == 9)
            {
                cout << 9;
            }
            else
            {
                cout << 0;
            }

            if (j != 9)
            {
                cout << "|";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// randomly places a boat onto the game board (for Player1)
bool setBoat(int grid[][10], int BoatSize, int name, vector<Boat> &boatList)
{
    srand(time(0));
    int col = 0;
    int row = 0;
    char dir = 'v';
    bool placementFailure = true;
    char check = 'v';
    int cS = 0;

    dir = getDirection(rand() % 10);
    col = resetColAndRow(col, row, BoatSize, dir);

    while (placementFailure)
    {
        if (dir == 'h')
        {
            cS = checkSpaces(grid, col, row, BoatSize, dir);
            if (cS == 1)
            {
                dir = getDirection(rand() % 10);
                col = resetColAndRow(col, row, BoatSize, dir);
                cS == 0;
                continue;
            }
            editGrid(grid, col, row, BoatSize, dir);
            editBoatInfo(grid, col, row, BoatSize, dir, boatList, name);
            return 0;
        }
        else if (dir == 'v')
        {
            cS = checkSpaces(grid, col, row, BoatSize, dir);
            if (cS == 1)
            {
                dir = getDirection(rand() % 10);
                col = resetColAndRow(col, row, BoatSize, dir);
                cS == 0;
                continue;
            }
            editGrid(grid, col, row, BoatSize, dir);
            editBoatInfo(grid, col, row, BoatSize, dir, boatList, name);
            return 0;
        }
    }

    return 0;
}

// randomly places a boat onto the game board (for Player2)
// (generates different directions for second player)
bool setBoat2(int grid[][10], int BoatSize, int name, vector<Boat> &boatList)
{
    srand(time(0));
    int col = 0;
    int row = 0;
    char dir = 'v';
    bool placementFailure = true;
    char check = 'v';
    int cS = 0;

    dir = getDirection(rand() % 1);
    col = resetColAndRow(col, row, BoatSize, dir);

    while (placementFailure)
    {
        if (dir == 'h')
        {
            cS = checkSpaces(grid, col, row, BoatSize, dir);
            if (cS == 1)
            {
                dir = getDirection(rand() % 1);
                col = resetColAndRow(col, row, BoatSize, dir);
                cS == 0;
                continue;
            }
            editGrid(grid, col, row, BoatSize, dir);
            editBoatInfo(grid, col, row, BoatSize, dir, boatList, name);
            return 0;
        }
        else if (dir == 'v')
        {
            cS = checkSpaces(grid, col, row, BoatSize, dir);
            if (cS == 1)
            {
                dir = getDirection(rand() % 1);
                col = resetColAndRow(col, row, BoatSize, dir);
                cS == 0;
                continue;
            }
            editGrid(grid, col, row, BoatSize, dir);
            editBoatInfo(grid, col, row, BoatSize, dir, boatList, name);
            return 0;
        }
    }

    return 0;
}

// chooses a direction to place boat
char getDirection(int dir)
{
    if (dir > 4)
    {
        return 'h'; // horizontal
    }
    else
    {
        return 'v'; // vertical
    }
}

// places the boat type's size number (2-5) on the game board
void editGrid(int grid[][10], int col, int row, int BoatSize, char dir)
{
    if (dir == 'h')
    {
        for (int i = 0; i < BoatSize; i++)
        {
            grid[row][col] = BoatSize;
            col++;
            cout << endl;
        }
    }
    else if (dir == 'v')
    {
        for (int i = 0; i < BoatSize; i++)
        {
            grid[row][col] = BoatSize;
            row++;
            cout << endl;
        }
    }
    else
    {
        cout << "Error!  No direction passed." << endl;
    }
}

// checks the game board to ensure that the boats do not overlap
int checkSpaces(int grid[][10], int c, int r, int s, char dir)
{
    int check = 0;
    if (dir == 'h')
    {
        for (int i = c; i < c + s; i++)
        {
            check = grid[r][i];
            if (check != 0)
            {
                return 1;
            }
        }

        return 0;
    }
    else
    {
        for (int i = r; i < r + s; i++)
        {
            check = grid[i][c];
            if (check != 0)
            {
                return 1;
            }
        }

        return 0;
    }
}

// generates a random column and row based on boat size so that the boats are not placed beyond the range of the game board
int resetColAndRow(int col, int &row, int BoatSize, char dir)
{
    switch (BoatSize)
    {
    case Destroyer:
        if (dir == 'h')
        {
            col = rand() % 7;
            row = rand() % 10;
        }
        else
        {
            col = rand() % 10;
            row = rand() % 7;
        }
        break;
    case Submarine:
        if (dir == 'h')
        {
            col = rand() % 8;
            row = rand() % 10;
        }
        else
        {
            col = rand() % 10;
            row = rand() % 8;
        }
        break;
    case Battleship:
        if (dir == 'h')
        {
            col = rand() % 6;
            row = rand() % 10;
        }
        else
        {
            col = rand() % 10;
            row = rand() % 6;
        }
        break;
    case Carrier:
        if (dir == 'h')
        {
            col = rand() % 5;
            row = rand() % 10;
        }
        else
        {
            col = rand() % 10;
            row = rand() % 5;
        }
    }
    return col;
}

// creates the boat objects
void editBoatInfo(int grid[][10], int c, int r, int BoatSize, char dir, vector<Boat> &boatList, int name)
{
    switch (name)
    {
    case 1:
        if (dir == 'h')
        {
            vector<int> r1(5);
            for (int i = 0; i < (int)r1.size(); ++i)
            {
                r1.at(i) = r;
            }
            vector<int> c1(5);
            for (int i = 0; i < (int)c1.size(); ++i)
            {
                c1.at(i) = c;
                c++;
            }
            Boat carrierBoat('h', 5, r1, c1, 0, "Carrier");
            boatList.push_back(carrierBoat);
        }
        else if (dir == 'v')
        {
            vector<int> r1(5);
            for (int i = 0; i < (int)r1.size(); ++i)
            {
                r1.at(i) = r;
                r++;
            }
            vector<int> c1(5);
            for (int i = 0; i < (int)c1.size(); ++i)
            {
                c1.at(i) = c;
            }
            Boat carrierBoat('v', 5, r1, c1, 0, "Carrier");
            boatList.push_back(carrierBoat);
        }
        break;
    case 2:
        if (dir == 'h')
        {
            vector<int> r1(4);
            for (int i = 0; i < (int)r1.size(); ++i)
            {
                r1.at(i) = r;
            }
            vector<int> c1(4);
            for (int i = 0; i < (int)c1.size(); ++i)
            {
                c1.at(i) = c;
                c++;
            }
            Boat battleshipBoat('h', 4, r1, c1, 0, "Battleship");
            boatList.push_back(battleshipBoat);
        }
        else if (dir == 'v')
        {
            vector<int> r1(4);
            for (int i = 0; i < (int)r1.size(); ++i)
            {
                r1.at(i) = r;
                r++;
            }
            vector<int> c1(4);
            for (int i = 0; i < (int)c1.size(); ++i)
            {
                c1.at(i) = c;
            }
            Boat battleshipBoat('v', 4, r1, c1, 0, "Battleship");
            boatList.push_back(battleshipBoat);
        }
        break;
    case 3:
        if (dir == 'h')
        {
            vector<int> r1(3);
            for (int i = 0; i < (int)r1.size(); ++i)
            {
                r1.at(i) = r;
            }
            vector<int> c1(3);
            for (int i = 0; i < (int)c1.size(); ++i)
            {
                c1.at(i) = c;
                c++;
            }
            Boat cruiser('h', 3, r1, c1, 0, "Cruiser");
            boatList.push_back(cruiser);
        }
        else if (dir == 'v')
        {
            vector<int> r1(3);
            for (int i = 0; i < (int)r1.size(); ++i)
            {
                r1.at(i) = r;
                r++;
            }
            vector<int> c1(3);
            for (int i = 0; i < (int)c1.size(); ++i)
            {
                c1.at(i) = c;
            }
            Boat cruiser('v', 3, r1, c1, 0, "Cruiser");
            boatList.push_back(cruiser);
        }
        break;
    case 4:
        if (dir == 'h')
        {
            vector<int> r1(3);
            for (int i = 0; i < (int)r1.size(); ++i)
            {
                r1.at(i) = r;
            }
            vector<int> c1(3);
            for (int i = 0; i < (int)c1.size(); ++i)
            {
                c1.at(i) = c;
                c++;
            }
            Boat submarine('h', 3, r1, c1, 0, "Submarine");
            boatList.push_back(submarine);
        }
        else if (dir == 'v')
        {
            vector<int> r1(3);
            for (int i = 0; i < (int)r1.size(); ++i)
            {
                r1.at(i) = r;
                r++;
            }
            vector<int> c1(3);
            for (int i = 0; i < (int)c1.size(); ++i)
            {
                c1.at(i) = c;
            }
            Boat submarine('v', 3, r1, c1, 0, "Submarine");
            boatList.push_back(submarine);
        }
        break;
    case 5:
        if (dir == 'h')
        {
            vector<int> r1(2);
            for (int i = 0; i < (int)r1.size(); ++i)
            {
                r1.at(i) = r;
            }
            vector<int> c1(2);
            for (int i = 0; i < (int)c1.size(); ++i)
            {
                c1.at(i) = c;
                c++;
            }
            Boat destroyerBoat('h', 2, r1, c1, 0, "Destroyer");
            boatList.push_back(destroyerBoat);
        }
        else if (dir == 'v')
        {
            vector<int> r1(2);
            for (int i = 0; i < (int)r1.size(); ++i)
            {
                r1.at(i) = r;
                r++;
            }
            vector<int> c1(2);
            for (int i = 0; i < (int)c1.size(); ++i)
            {
                c1.at(i) = c;
            }
            Boat destroyerBoat('v', 2, r1, c1, 0, "Destroyer");
            boatList.push_back(destroyerBoat);
        }
        break;
    }
}

// starts the Battleship game
int playGame(int grid[][10], vector<Boat> &boatList)
{
    bool input = true;
    int row = 0;
    int col = 0;
    int guess = 0;
    int hit = 0;
    int miss = 0;
    int space = 0;
    string btname = "";
    int sunk = 0;

    printGameBoard(grid);
    // printBoard(grid);  // uncomment to see the game board with all the boats on it

    while (input)
    {
        cout << "Enter a row|column coordinate (0 0): ";
        cin >> row;
        cin >> col;

        if (!cin)
        {
            cout << "Coordinates must be integers only." << endl << endl;
            cin.clear();
            cin.sync();
            continue;
        }
        cin.sync();

        if (row > 9 || row < 0 || col > 9 || col < 0)
        {
            cout << "Coordinates must be in range." << endl << endl;
            continue;
        }

        input = false;
    }
    cout << endl;
    guess++;
    space = getSpace(grid, row, col);

    // determine if a space/coordinate was bombed or missed
    // determine the type of boat if it was successfully bombed
    // determine if the boat has been sunk
    switch (space)
    {
    case 0:
        cout << "You have missed." << endl;
        grid[row][col] = 9;
        miss++;
        trackMisses();
        break;
    case 1:
        cout << "That coordinate has already been guessed.  You have wasted a turn!" << endl;
        break;
    case 2:
        grid[row][col] = 1;
        hit++;

        btname = boatList[4].getBoat(row, col);
        cout << "You have bombed the " << btname << '!' << endl;
        boatList[4].setHit();

        sunk = boatList[4].checkSunk(Destroyer);
        if (sunk == 9)
        {
            cout << "The " << btname << " has sunk." << endl;
        }
        btname.clear();
        break;
    case 3:
        grid[row][col] = 1;
        hit++;

        btname = boatList[3].getBoat(row, col);
        if (btname == "Submarine") // check to see if boat of length 3 is a Submarine
        {
            cout << "You have bombed the " << btname << '!' << endl;
            boatList[3].setHit();
            sunk = boatList[3].checkSunk(Submarine);
            if (sunk == 9)
            {
                cout << "The " << btname << " has sunk." << endl;
            }
        }

        btname = boatList[2].getBoat(row, col);
        if (btname == "Cruiser") // check to see if boat of length 3 is a Cruiser
        {
            cout << "You have bombed the " << btname << '!' << endl;
            boatList[2].setHit();
            sunk = boatList[2].checkSunk(Cruiser);
            if (sunk == 9)
            {
                cout << "The " << btname << " has sunk." << endl;
            }
        }
        btname.clear();
        break;
    case 4:
        grid[row][col] = 1;
        hit++;

        btname = boatList[1].getBoat(row, col);
        cout << "You have bombed the " << btname << '!' << endl;
        boatList[1].setHit();

        sunk = boatList[1].checkSunk(Battleship);
        if (sunk == 9)
        {
            cout << "The " << btname << " has sunk." << endl;
        }
        btname.clear();
        break;
    case 5:
        grid[row][col] = 1;
        hit++;

        btname = boatList[0].getBoat(row, col);
        cout << "You have bombed the " << btname << '!' << endl;
        boatList[0].setHit();

        sunk = boatList[0].checkSunk(Carrier);
        if (sunk == 9)
        {
            cout << "The " << btname << " has sunk." << endl;
        }
        btname.clear();
        break;
    }

    return 0;
}

// gets the space number (0-5, or 9 for a miss)
int getSpace(int grid[][10], int row, int col)
{
    int space = 0;
    space = grid[row][col];

    return space == 9 ? 1 : space; // if the space/coordinate had already been bombed, and it was a miss (9), return 1; otherwise, return space number
}

// keeps track of the misses
int trackMisses()
{
    static int miss = 0;
    miss++;

    return miss;
}