#include "hidden_singles.h"

int find_hidden_single_values(Cell **cells, int *hidden_single_values) {
    int unique_candidates = 0;
    int candidate_occurrences[BOARD_SIZE] = {0};

    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (cells[i]->num_candidates > 1) {
            int *candidates = get_candidates(cells[i]);
            for (int j = 0; j < cells[i]->num_candidates; ++j) {
                int candidate = candidates[j];
                candidate_occurrences[candidate - 1]++;
            }
            free(candidates);
        }
    }

    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (candidate_occurrences[i] == 1) {
            hidden_single_values[unique_candidates++] = i + 1;
        }
    }

    return unique_candidates;
}

void find_hidden_single_cells(Cell **cells, HiddenSingle *hidden_singles, int *counter) {
    int hidden_single_values[BOARD_SIZE];
    for (int i = 0; i < find_hidden_single_values(cells, hidden_single_values); ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (is_candidate(cells[j], hidden_single_values[i])) {
                HiddenSingle hidden_singles_object;
                hidden_singles_object.value = hidden_single_values[i];
                hidden_singles_object.p_cell = cells[j];
                hidden_singles[(*counter)++] = hidden_singles_object;
            }
        }
    }
}

int overlapped_candidates(HiddenSingle *hidden_singles, int *counter) {
    int overlap = *counter;

    for (int i = 0; i < *counter; ++i) {
        int not_overlap = 0;
        Cell *hidden_single_cell = hidden_singles[i].p_cell;
        int *candidates = get_candidates(hidden_single_cell);
        int num_candidates = hidden_single_cell->num_candidates;

        for (int j = 0; j < num_candidates; ++j) {
            if (candidates[j] != hidden_singles[i].value) {
                unset_candidate(hidden_single_cell, candidates[j]);
                not_overlap = 1;
            }
        }
        overlap -= not_overlap;
        free(candidates);
    }
    return overlap;
}

int hidden_singles(SudokuBoard *board) {
    HiddenSingle hidden_singles[BOARD_SIZE * BOARD_SIZE];
    int counter = 0;
    int overlap = 0;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        find_hidden_single_cells(board->p_rows[i], hidden_singles, &counter);
        find_hidden_single_cells(board->p_cols[i], hidden_singles, &counter);
        find_hidden_single_cells(board->p_boxes[i], hidden_singles, &counter);
    }

    overlap = overlapped_candidates(hidden_singles, &counter);

    return counter - overlap;
}
