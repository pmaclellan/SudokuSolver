/*
EECE 3326
Lab 4b
11/12/2014

Peter MacLellan
Ben Soper

This header file defines the Board class.  It also contains implementation
details for getting and setting cells on the board, updating and printing
conflict vectors, and ways for finding if the board is complete.  Additionally,
it includes a method for finding the next cell to fill that has the fewest
possible numbers without conflicts.
*/

#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>
#include <vector>

using namespace std;

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank

const int SquareSize = 3;  //  The number of cells in a small square
//  (usually 3).  The board has
//  SquareSize^2 rows and SquareSize^2
//  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;

class Board
    // Stores the entire Sudoku Board
{
public:
    Board(int);
    void clear();
    void initialize(ifstream &fin);
    void print();
    void printToFile(ofstream&);
    bool isBlank(int, int);
    ValueType getCell(int, int);
    void findBlankCell(int&, int&);
    void setCell(int, int, int);
    void clearCell(int, int);
    void updateConflicts(int, int, int);
    int  numConflicts(int, int);
    bool noConflicts(int, int, int);
    void printConflicts(ofstream&);
    bool isComplete();

private:

    // The following matrices go from 1 to BoardSize in each
    // dimension, i.e., they are each (BoardSize+1) * (BoardSize+1)
    matrix<ValueType> value;
    matrix<bool> conflict_rows;
    matrix<bool> conflict_cols;
    matrix<bool> conflict_sqs;
};

int squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 0 to BoardSize-1
{
    // Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
    // coordinates of the square that i,j is in.  

    return SquareSize * (i / SquareSize) + (j / SquareSize);
}

//Finds the next cell to fill based on which one has the most conflicts
//and therefore the fewest possible moves
void Board::findBlankCell(int& i, int& j)
{
    int max = 0;
    for (int x = 0; x < BoardSize; x++)
    {
        for (int y = 0; y < BoardSize; y++)
        {
            if (isBlank(x, y) && numConflicts(x, y) > max)
            {
                i = x;
                j = y;
                max = numConflicts(x, y);
                if (max == 8)
                {
                    return;
                }
            }
        }
    }
}

// Adds a vlue to a cell and updates conflict vectors
void Board::setCell(int i, int j, int num)
{
    //used when resetting a cell
    if (num == Blank)
    {
        value[i][j] = num;
        return;
    }

    int sq_id = squareNumber(i, j);
    if (!conflict_rows[i][num-1] && !conflict_cols[j][num-1] && !conflict_sqs[sq_id][num-1])
    {
        value[i][j] = num;
        updateConflicts(i, j, num);
    }
}

// Clears a cell by filling it with Blank and updating conflict vectors.
void Board::clearCell(int i, int j)
{
    int sq_id = squareNumber(i, j);
    int num = getCell(i, j);

    conflict_rows[i][num - 1] = false;
    conflict_cols[j][num - 1] = false;
    conflict_sqs[sq_id][num - 1] = false;

    setCell(i, j, Blank);
}

// updates conflict vectors after adds
void Board::updateConflicts(int i, int j, int num)
{
    int sq_id = squareNumber(i, j);

    conflict_rows[i][num - 1] = true;
    conflict_cols[j][num - 1] = true;
    conflict_sqs[sq_id][num - 1] = true;
}

// returns true if there are no conflicts at a given cell for a given value
bool Board::noConflicts(int i, int j, int n)
{
    int sq_id = squareNumber(i, j);
    return !conflict_rows[i][n - 1] && !conflict_cols[j][n - 1] 
            && !conflict_sqs[sq_id][n - 1];
}

// returns the number of conflicts for a given cell
int Board::numConflicts(int i, int j)
{
    int count = 0;
    for (int n = 1; n <= 9; n++)
    {
        if (!noConflicts(i, j, n))
        {
            count++;
        }
    }
    return count;
}

// Loops through all three conflict vectors for a given cell and prints them
void Board::printConflicts(ofstream& fout)
{
    for (int i = 0; i < 9; i++)
    {
        fout << "\nRow " << i + 1 << ": ";
        for (int n = 0; n < 9; n++)
        {
            fout << conflict_rows[i][n];
        }
    }

    fout << endl;

    for (int i = 0; i < 9; i++)
    {
        fout << "\nCol " << i + 1 << ": ";
        for (int n = 0; n < 9; n++)
        {
            fout << conflict_cols[i][n];
        }
    }

    fout << endl;

    for (int i = 0; i < 9; i++)
    {
        fout << "\nSquare " << i + 1 << ": ";
        for (int n = 0; n < 9; n++)
        {
            fout << conflict_sqs[i][n];
        }
    }

    fout << endl;
}

// Board constructor
Board::Board(int sqSize)
: value(BoardSize, BoardSize)
, conflict_cols(BoardSize, BoardSize)
, conflict_rows(BoardSize, BoardSize)
, conflict_sqs(BoardSize, BoardSize)

{
    clear();
}

// Mark all possible values as legal for each Board entry
void Board::clear()
{
    for (int i = 0; i < BoardSize; i++)
    {
        for (int j = 0; j < BoardSize; j++)
        {
            value[i][j] = Blank;
        }
    }

}

// Read a Sudoku Board from the input file.
void Board::initialize(ifstream &fin)
{
    // Set conflicts to false
    for (int i = 0; i < BoardSize; i++)
    {
        for (int j = 0; j < BoardSize; j++)
        {
            conflict_rows[i][j] = false;
            conflict_cols[i][j] = false;
            conflict_sqs[i][j] = false;
        }
    }

    char ch;

    clear();

    for (int i = 0; i < BoardSize; i++)
    {
        for (int j = 0; j < BoardSize; j++)
        {
            fin >> ch;

            // If the read char is not Blank
            if (ch != '.')
                setCell(i, j, ch - '0');   // Convert char to int
        }
    }

    // Initialize conflicts list
    for (int i = 0; i < BoardSize; i++)
    {
        for (int j = 0; j < BoardSize; j++)
        {
            if (!isBlank(i, j))
            {
                int value = getCell(i, j);
                int sq_id = squareNumber(i, j);

                conflict_rows[i][value - 1] = true;
                conflict_cols[j][value - 1] = true;
                conflict_sqs[sq_id][value - 1] = true;
            }
        }
    }
}

ValueType Board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
    if (i >= 0 && i < BoardSize && j >= 0 && j < BoardSize)
        return value[i][j];
    else
        throw rangeError("bad value in getCell");
}

bool Board::isBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
    if (i < 0 || i >= BoardSize || j < 0 || j >= BoardSize)
        throw rangeError("bad value in setCell");

    return (getCell(i, j) == Blank);
}

void Board::print()
// Prints the current Board.
{
    for (int i = 0; i < BoardSize; i++)
    {
        if (i % SquareSize == 0)
        {
            cout << " -";
            for (int j = 0; j < BoardSize; j++)
                cout << "---";
            cout << "-";
            cout << endl;
        }
        for (int j = 0; j < BoardSize; j++)
        {
            if (j % SquareSize == 0)
                cout << "|";
            if (!isBlank(i, j))
                cout << " " << getCell(i, j) << " ";
            else
                cout << "   ";
        }
        cout << "|";
        cout << endl;
    }

    cout << " -";
    for (int j = 1; j <= BoardSize; j++)
        cout << "---";
    cout << "-";
    cout << endl;
}

void Board::printToFile(ofstream& fout)
// Prints the current Board.
{
    for (int i = 0; i < BoardSize; i++)
    {
        if (i % SquareSize == 0)
        {
            fout << " -";
            for (int j = 0; j < BoardSize; j++)
                fout << "---";
            fout << "-";
            fout << endl;
        }
        for (int j = 0; j < BoardSize; j++)
        {
            if (j % SquareSize == 0)
                fout << "|";
            if (!isBlank(i, j))
                fout << " " << getCell(i, j) << " ";
            else
                fout << "   ";
        }
        fout << "|";
        fout << endl;
    }

    fout << " -";
    for (int j = 1; j <= BoardSize; j++)
        fout << "---";
    fout << "-";
    fout << endl;
}

bool Board::isComplete()
{
    for (int i = 0; i < BoardSize; i++)
    {
        for (int j = 0; j < BoardSize; j++)
        {
            if (isBlank(i, j))
            {
                return false;
            }
        }
    }
    return true;
}