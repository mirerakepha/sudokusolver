#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>


extern int UNSOLVED;
extern int SIZE_ROWS;
extern int SIZE_COLUMNS;


typedef struct Sudoku
{
    struct Square *** squares;
    struct Box ** boxes;

} Sudoku;


typedef struct Box
{
    struct Square ** squares;
    int numbers;
    int possible[9];
    int solvable;
    struct Box * next;
       
} Box;






typedef struct Square
{
    int number;
    //for a 0 -> it can be any number from 1-9 but a number (sth already 1-9) cannot change

    //lets decode the algo using the top left most square

    /*
    if a number has a possibillity to occupy the square then it remains 0 but if it does not it becomes 1
    111101011    -> so here only 5 or 3 can occupy
    987654321    
    */

    int possible[9];
    int solvable;

    Box * box;
    int row;
    int column;
    
} Square;




int ** createPuzzle();

void printPuzzle(Square *** puzzle);

Sudoku * setUpPuzzle(int ** puzzle);

Sudoku * createSudoku(Square *** puzzle, Box ** boxes);

int updateSudoku(Square *** sudoku, int row, int column);

int checkPuzzle(Square *** sudoku, Box ** boxes);

int checkRows(Square *** sudoku, Box ** boxes);

int boxSingles(Square *** sudoku, Box ** boxes);

int solveSquare(Square * square);

Box ** createBoxes();

int updateBoxes(Square *** sudoku, int row, int column);

#endif