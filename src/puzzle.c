#include "sudoku.h"
#include <ncurses.h>


Sudoku * createSudoku(Square *** squares, Box ** boxes){
    Sudoku * sudoku;
    sudoku = malloc(sizeof(Sudoku));
    sudoku -> squares = squares;
    sudoku -> boxes = boxes;

    return sudoku;

}

Sudoku * setUpPuzzle(int ** puzzle){
    Square *** sudoku;
    Box ** boxes;
    int i, j, x;
    int currentBox = 0;

    sudoku = (Square***)malloc(sizeof(Square**)*9);
    boxes = createBoxes();

    for (i = 0; i < SIZE_ROWS; i++)
    {
        sudoku[i] = (Square**)malloc(sizeof(Square*)*9);
        
        for (j = 0; j < SIZE_COLUMNS; j++)
        {
            sudoku[i][j] = malloc(sizeof(Square));

            sudoku[i][j] -> number = puzzle[i][j];

            sudoku[i][j] -> row = i;
            sudoku[i][j] -> column = j;

            sudoku[i][j] -> solvable = 9;

            boxes[currentBox] -> squares [ boxes[currentBox]->numbers ] = sudoku[i][j];
            sudoku[i][j] -> box = boxes[currentBox];
            boxes[currentBox] -> numbers++;

            for (x = 0; x < SIZE_ROWS; x++)
            {
                sudoku[i][j]->possible[x] = 0;
            }

            //go to the next box as the array increases 
            if (j == 2)
                currentBox++;
            
            if (j == 5)
                currentBox++;
                                     
        }
        //go back to the firstbox after comleting a loop for the first line
        currentBox -= 2;

        //on reaching the last line of the 3rd box;row;column we go to the box 4
        if (i == 2)
            currentBox = 3;

        if (i == 5)
            currentBox = 6;
                
    }

     for (i = 0; i < SIZE_ROWS; i++)
    {        
        for (j = 0; j < SIZE_COLUMNS; j++)
        {
            if (sudoku[i][j]->number != 0)
            {
                sudoku[i][j]->solvable = 0;
                updateSudoku(sudoku, i, j);
                updateBoxes(sudoku, i, j);
                UNSOLVED--;
            }
            

        }
    }

    return createSudoku(sudoku, boxes);
}

int updateSudoku(Square *** sudoku, int row, int column){

    int x;
    int number = sudoku[row][column]->number;

    for (x = 0; x < SIZE_ROWS; x++)
    {
        if (sudoku[x][column] -> possible[number - 1] == 0)
        {
            sudoku[x][column] -> solvable--;
        }

        sudoku[x][column]->possible[number - 1] = 1;
        
    }


    for (x = 0; x < SIZE_COLUMNS; x++)
    {
        if (sudoku[row][x] -> possible[number - 1] == 0)
        {
            sudoku[row][x] -> solvable--;
        }

        sudoku[row][x]->possible[number - 1] = 1;
        
    }
    return 1;    

}


int checkPuzzle(Square *** sudoku, Box ** boxes)
{
    int i, j, k;
    for (i = 0; i < SIZE_ROWS; i++)
    {        
        for (j = 0; j < SIZE_COLUMNS; j++)
        {
            if (sudoku[i][j]->solvable == 1)
            {
                solveSquare(sudoku[i][j]);
                updateSudoku(sudoku, i, j);
                updateBoxes(sudoku, i, j);

                return 1;
            }

        }
    }

    if (boxSingles(sudoku, boxes))
        return 1;

    return checkRows(sudoku, boxes);    
    
}


int ** createPuzzle() {

    int ** puzzle;
    int i, j;

    puzzle = malloc(sizeof(int*) * 9);
    for (i = 0; i < 9; i++)
        puzzle[i] = calloc(9, sizeof(int));

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    int row = 0, col = 0;
    int ch;

    while (1) {

        clear();

        mvprintw(0, 0,
            "Arrow keys move | 1-9 enter | 0 clears | ENTER solves");

        int startY = 2;
        int startX = 4;

        mvprintw(startY, startX,
                "+---------+---------+---------+");

        for (i = 0; i < 9; i++) {

            int y = startY + 1 + i + (i / 3);

            mvprintw(y, startX, "|");

            for (j = 0; j < 9; j++) {

                int x = startX + 2 + j * 3 + (j / 3);

                if (i == row && j == col)
                    attron(A_REVERSE);

                if (puzzle[i][j] == 0)
                    mvprintw(y, x, ".");
                else
                    mvprintw(y, x, "%d", puzzle[i][j]);

                if (i == row && j == col)
                    attroff(A_REVERSE);

                if ((j + 1) % 3 == 0)
                    mvprintw(y, x + 2, "|");
            }

            if ((i + 1) % 3 == 0)
                mvprintw(y + 1, startX,
                        "+---------+---------+---------+");
        }

        refresh();
        ch = getch();

        if (ch == KEY_UP && row > 0) row--;
        else if (ch == KEY_DOWN && row < 8) row++;
        else if (ch == KEY_LEFT && col > 0) col--;
        else if (ch == KEY_RIGHT && col < 8) col++;
        else if (ch >= '1' && ch <= '9')
            puzzle[row][col] = ch - '0';
        else if (ch == '0' || ch == KEY_BACKSPACE)
            puzzle[row][col] = 0;
        else if (ch == '\n')
            break;
    }

    endwin();
    return puzzle;
}

/*
int ** createPuzzle(){
    int ** puzzle;
    int i, j;

    int array [9][9] = {

        0, 1, 9,    0, 0, 2,    0, 0, 0,
        4, 7, 0,    6, 9, 0,    0, 0, 1,
        0, 0, 0,    4, 0, 0,    0, 9, 0,

        8, 9, 4,    5, 0, 7,    0, 0, 0,
        0, 0, 0,    0, 0, 0,    0, 0, 0,
        0, 0, 0,    2, 0, 1,    9, 5, 8,

        0, 5, 0,    0, 0, 6,    0, 0, 0,
        6, 0, 0,    0, 2, 8,    0, 7, 9,
        0, 0, 0,    1, 0, 0,    8, 6, 0,

    }; 

    //alocate mem
    puzzle = (int**)malloc(sizeof(int*)*9);

    for (i = 0; i < SIZE_ROWS; i++)
    {
        puzzle[i] = malloc(sizeof(int)*9);

        for (j = 0; j < SIZE_COLUMNS; j++)
        {
            puzzle[i][j] = array[i][j];
            
        }
        
    }
    
    return puzzle;
}

*/


void printPuzzle(Square *** puzzle){
    int i, j;

    //top border
    printf("\n");
    printf("\t+---------+---------+---------+\n");

    for (i = 0; i < SIZE_ROWS; i++)
    {
        //left border
        printf("\t|");

        for (j = 0; j < SIZE_COLUMNS; j++)
        { 
           printf(" %d ", puzzle[i][j]->number);

           //border after every 3 columns
           if(((j + 1) % 3) == 0 ){
            printf("|");
           }
        }
        printf("\n");

        //border after every 3 rows
        if(((i + 1)%3) == 0 ){
            printf("\t+---------+---------+---------+\n");
           }
        
    }
    

}