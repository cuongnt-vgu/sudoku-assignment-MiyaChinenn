#include "hidden_pairs.h"

bool check_pair_in_cell(Cell **cells, int val1, int val2)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if ((cells[i]->candidates[val1 - 1] == 1 && cells[i]->candidates[val2 - 1] != 1) ||
            (cells[i]->candidates[val2 - 1] == 1 && cells[i]->candidates[val1 - 1] != 1))
        {
            return false;
        }
    }
    return true;
}

int find_hidden_pair_values(Cell **cells, int *pair_values)
{
    int count = 0;
    int temp_array[BOARD_SIZE] = {0};

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (cells[i]->num_candidates > 1)
        {
            int *candidates = get_candidates(cells[i]);
            for (int j = 0; j < cells[i]->num_candidates; j++)
            {
                temp_array[candidates[j] - 1]++;
            }
            free(candidates);
        }
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (temp_array[i] == 2)
        {
            pair_values[count++] = i + 1;
        }
    }
    return count;
}

void find_hidden_pairs_in_cells(Cell **cells, HiddenPair *hidden_pairs, int *counter)
{
    int pair_values[BOARD_SIZE];
    int count = find_hidden_pair_values(cells, pair_values);
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (check_pair_in_cell(cells, pair_values[i], pair_values[j]))
            {
                for (int k = 0; k < BOARD_SIZE - 1; k++)
                {
                    for (int l = k + 1; l < BOARD_SIZE; l++)
                    {
                        if (cells[k]->candidates[pair_values[i] - 1] == 1 &&
                            cells[k]->candidates[pair_values[j] - 1] == 1 &&
                            cells[l]->candidates[pair_values[i] - 1] == 1 &&
                            cells[l]->candidates[pair_values[j] - 1] == 1)
                        {
                            hidden_pairs[(*counter)++] = (HiddenPair){cells[k], cells[l], pair_values[i], pair_values[j]};
                        }
                    }
                }
            }
        }
    }
}

int hidden_pairs(SudokuBoard *board) {
    int overlap_count = 0;
    int pair_count = 0;
    HiddenPair hidden_pairs[BOARD_SIZE * BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++)
    {   
        find_hidden_pairs_in_cells(board->p_rows[i], hidden_pairs, &pair_count);
        find_hidden_pairs_in_cells(board->p_cols[i], hidden_pairs, &pair_count);
        find_hidden_pairs_in_cells(board->p_boxes[i], hidden_pairs, &pair_count);
    }

    for (int i = 0; i < pair_count; i++)
    {
        if (hidden_pairs[i].p_cell1->num_candidates == 2 && hidden_pairs[i].p_cell2->num_candidates == 2)
        {
            overlap_count++;
        }
        
        int num1 = hidden_pairs[i].p_cell1->num_candidates;
        int num2 = hidden_pairs[i].p_cell2->num_candidates;
        int *candidates1 = get_candidates(hidden_pairs[i].p_cell1);
        int *candidates2 = get_candidates(hidden_pairs[i].p_cell2);

        for (int j = 0; j < num1; j++)
        {
            if (candidates1[j] != hidden_pairs[i].value1 && candidates1[j] != hidden_pairs[i].value2)
            {
                unset_candidate(hidden_pairs[i].p_cell1, candidates1[j]);
            }
        }

        for (int j = 0; j < num2; j++)
        {
            if (candidates2[j] != hidden_pairs[i].value1 && candidates2[j] != hidden_pairs[i].value2)
            {
                unset_candidate(hidden_pairs[i].p_cell2, candidates2[j]);
            }
        }

        free(candidates1);
        free(candidates2);
    }

    return pair_count - overlap_count;
}