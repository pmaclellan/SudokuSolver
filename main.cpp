/*
EECE 3326
Lab 4b
11/12/2014

Peter MacLellan
Ben Soper

This is the main file for lab 4b.  It calls functions to read in 3 separate
sudoku boards and intialize their conflict vectors and then prints them out 
to a file.  It then solves each board, keeping track of how many recursive
calls each one made and printing the average.
*/

#include "board.h"

using namespace std;
int COUNTREC = 0;
int totalCount = 0;

void solve(Board&, ofstream&);

int main()
{
    ifstream fin;
    ofstream fout;

    fout.open("boardLog.txt");

    // Begin board 1
    string fileName = "sudoku1.txt";

    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        system("Pause");
        exit(1);
    }

    try
    {
        Board b1(SquareSize);

        while (fin && fin.peek() != 'Z')
        {
            b1.initialize(fin);
            b1.printToFile(fout);
            b1.printConflicts(fout);
            solve(b1, fout);
            if (b1.isComplete())
            {
                b1.print();
                cout << "board complete in " << COUNTREC << " recursive calls\n";
                totalCount += COUNTREC;
                COUNTREC = 0;
            }
            else cout << "board not complete\n";
        }
    }
    catch (indexRangeError &ex)
    {

        cout << ex.what() << endl;
        system("Pause");
        exit(1);
    }
    fin.close();

    // Begin board 2
    fileName = "sudoku2.txt";

    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        system("Pause");
        exit(1);
    }

    try
    {
        Board b2(SquareSize);

        while (fin && fin.peek() != 'Z')
        {
            b2.initialize(fin);
            b2.printToFile(fout);
            b2.printConflicts(fout);
            solve(b2, fout);
            if (b2.isComplete())
            {
                b2.print();
                cout << "board complete in " << COUNTREC << " recursive calls\n";
                totalCount += COUNTREC;
                COUNTREC = 0;
            }
            else cout << "board not complete\n";
        }
        fin.close();
    }
    catch (indexRangeError &ex)
    {

        cout << ex.what() << endl;
        system("Pause");
        exit(1);
    }

    // Begin board 3
    fileName = "sudoku3.txt";

    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        system("Pause");
        exit(1);
    }

    try
    {
        Board b3(SquareSize);

        while (fin && fin.peek() != 'Z')
        {
            b3.initialize(fin);
            b3.printToFile(fout);
            b3.printConflicts(fout);
            solve(b3, fout);
            if (b3.isComplete())
            {
                b3.print();
                cout << "board complete in " << COUNTREC << " recursive calls\n";
                totalCount += COUNTREC;
                COUNTREC = 0;
            }
            else cout << "board not complete\n";
        }
        fin.close(); 
    }
    catch (indexRangeError &ex)
    {

        cout << ex.what() << endl;
        system("Pause");
        exit(1);
    }

    cout << "Total Recursive Calls: " << totalCount << endl;
    cout << "Average Recursive Calls: " << totalCount / 3 << endl;
    fout.close();
    system("Pause");
}

//recursively solves a given sudoku board with backtracking
void solve(Board& board, ofstream& fout)
{
    int i = -1;
    int j = -1;

    COUNTREC++;

    if (board.isComplete())
    {
        return;
    }

    //find the blank cell with the lowest number of conflicts
    board.findBlankCell(i ,j);

    for (int n = 1; n <= 9; n++)
    {
        if (board.noConflicts(i, j, n))
        {
            board.setCell(i, j, n);
            board.printToFile(fout);
            board.printConflicts(fout);
            solve(board, fout);
            if (board.isComplete())
            {
                return;
            }
            board.clearCell(i, j);
            board.printToFile(fout);
            board.printConflicts(fout);
        }
    }
}