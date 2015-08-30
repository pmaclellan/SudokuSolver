# SudokuSolver
Lab 4 from Optimization Methods (EECE3326). Uses recursion to solve sudoku puzzles.

From project description:

Write a program that solves Sudoku puzzles. The input to Sudoku is a 9x9 board that is subdivided
into 3x3 squares. Each cell is either blank or contains an integer from 1 to 9.
A solution to a puzzle is the same board with every blank cell filled in with a digit from 1 to 9 such
that every digit appears exactly once in every row, column, and square.
The input to the program is a text file containing a collection of Sudoku boards, with one board
per line.

Your algorithm should read each board from the text file, solve it, print the solution, and
print the number of recursive iterations needed to solve that board. After all boards have
been solved, print the average number of recursive calls needed to solve all the boards.
Your algorithm should minimize the average number of recursive calls needed to solve all
the boards.
