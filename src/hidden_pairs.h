#pragma once

#include "sudoku.h"

typedef struct 
{
    Cell *p_cell1;
    Cell *p_cell2;  
    int value1;    
    int value2;    
} HiddenPair;

bool check_pair_in_cell(Cell **cells, int val1, int val2);

int find_hidden_pair_values(Cell **cells, int *pair_values);

void find_hidden_pairs_in_cells(Cell **cells, HiddenPair *hidden_pairs, int *counter);

int hidden_pairs(SudokuBoard *board);






