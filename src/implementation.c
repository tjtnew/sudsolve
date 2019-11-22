#include <stdbool.h>
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Utils.h>

#define N 9

/********************************************************************
 * STRUCTURES
 ********************************************************************/

// int_array holds both a pointer to an array and it's length
struct int_array {
    int length;
    int *array;
};

/********************************************************************
 * HELPER FUNCTIONS
 ********************************************************************/

// Frees both the array member of int_array and the struct itself
void free_int_array(struct int_array *x) {
    Free(x->array);
    Free(x);
}

// copy vector/array to N by N matrix
void vec_to_matrix(int vec[], int matrix[N][N]) {
    int idx = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = vec[idx];
            idx++;
        }
    }
}

// copy N by N matrix to vector/array
void matrix_to_vec(int vec[], int matrix[N][N]) {
    int idx = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            vec[idx] = matrix[i][j];
            idx++;
        }
    }
}

// copy N by N matrix to another
void copy_grid(int grid[N][N], int copy[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            copy[row][col] = grid[row][col];
        }
    }
}


// Creates a new array that is the ordered intersection of the two input arrays.
// Input arrays must be must be sorted (ascending) and resulting structure must
// be freed with free_int_array.
struct int_array *int_inter(int *array1, int size1, int *array2, int size2) {
    int cnt = size1 + size2;

    int *tmp = Calloc(cnt, int);

    int i = 0;
    int j = 0;
    int idx = 0;
    while(i < size1 && j < size2) {
        if (array1[i] < array2[j]) {
            i++;
        } else if (array2[j] < array1[i]) {
            j++;
        } else {
            tmp[idx] = array1[i];
            idx++;
            i++;
            j++;
        }
    }

    struct int_array *result;
    result = Calloc(1, struct int_array);

    int *array;
    if (idx != 0) {
        array = Realloc(tmp, idx, int);
    } else {
        array = NULL;
        Free(tmp);
    }


    result->length = idx;
    result->array = array;

    return result;
}

// Creates a new array that is the ordered intersection of multiple input arrays.
// Input arrays must be must be sorted (ascending) and resulting structure mpust
// be freed with free_int_array.
//
// array =  of pointers to pointer to a sorted int array
// num_arrays = number of arrays pointed to
// lengths = array of lengths of the individual arrays
struct int_array *int_inter_loop(int **array, int num_arrays, int *lengths) {

    struct int_array *inter;
    inter = int_inter(array[0], lengths[0], array[1], lengths[1]);

    for (int i = 2; i < num_arrays; i++) {
        struct int_array *tmp;
        tmp = int_inter(inter->array, inter->length, array[i], lengths[i]);
        Free(inter->array);
        inter->array = tmp->array;
        inter->length = tmp->length;
        Free(tmp);
    }

    return inter;
}

/********************************************************************
 * HUMAN METHODS - lone single
 ********************************************************************/

// returns pointer to int_array of values from 1 to 9 that are not in
// a given array
struct int_array *possible_values(int array[], int size_array) {
    int tmp[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 0; i < size_array; i++) {
        for (int j = 0; j < N; j++) {
            if (array[i] == tmp[j]) {
                tmp[j] = 0;
            }
        }
    }

    int cnt = 0;
    int *new_array = Calloc(N, int);
    for (int i = 0; i < N; i++) {
        if (tmp[i] > 0) {
            new_array[cnt] = tmp[i];
            cnt++;
        }
    }

    if (cnt > 0) {
        new_array = Realloc(new_array, cnt, int);
    } else {
        free(new_array);
        new_array = NULL;
    }


    struct int_array *possible;
    possible = Calloc(1, struct int_array);
    possible->array = new_array;
    possible->length = cnt;
    return possible;
}

// returns pointer to int_array of values from 1 to 9 that are not in
// a given row of the grid
struct int_array *possible_row_values(int grid[N][N], int row_num) {
    struct int_array *x = possible_values(grid[row_num], N);
    return x;
}

// returns pointer to int_array of values from 1 to 9 that are not in
// a given column of the grid
struct int_array *possible_col_values(int grid[N][N], int col_num) {
    int col[N] = {grid[0][col_num],
                  grid[1][col_num],
                  grid[2][col_num],
                  grid[3][col_num],
                  grid[4][col_num],
                  grid[5][col_num],
                  grid[6][col_num],
                  grid[7][col_num],
                  grid[8][col_num]};
    struct int_array *x = possible_values(col, N);
    return x;
}

// returns pointer to int_array of values from 1 to 9 that are not in
// a given square of the grid.  Will currently only work for 9 by 9
// sudokus
struct int_array *possible_square_values(int grid[N][N], int row, int col) {
    int square_row = (row / 3) * 3;
    int square_col = (col / 3) * 3;

    int square[N] = {grid[square_row][square_col],
                     grid[square_row][square_col + 1],
                     grid[square_row][square_col + 2],
                     grid[square_row + 1][square_col],
                     grid[square_row + 1][square_col + 1],
                     grid[square_row + 1][square_col + 2],
                     grid[square_row + 2][square_col],
                     grid[square_row + 2][square_col + 1],
                     grid[square_row + 2][square_col + 2]};

    struct int_array *x = possible_values(square, N);
    return x;
}

// returns pointer to int_array of values from 1 to 9 that can be used within
// an individual cell of the grid (i.e. not already in corresponding row,
// column or square)
struct int_array *possible_cell_values(int grid[N][N], int row, int col) {
    struct int_array *row_poss = possible_row_values(grid, row);
    struct int_array *col_poss = possible_col_values(grid, col);
    struct int_array *square_poss = possible_square_values(grid, row, col);
    int *arrays[3];
    arrays[0] = row_poss->array;
    arrays[1] = col_poss->array;
    arrays[2] = square_poss->array;
    int lengths[3];
    lengths[0] = row_poss->length;
    lengths[1] = col_poss->length;
    lengths[2] = square_poss->length;

    struct int_array *result = int_inter_loop(arrays, 3, lengths);

    free_int_array(row_poss);
    free_int_array(col_poss);
    free_int_array(square_poss);

    return result;
}

/********************************************************************
 * HUMAN METHODS - hidden singles
 ********************************************************************/

// Looks for values that only appear once within a given row and allocates
bool hidden_values_row(int grid[N][N], int row) {
    int possible[N] = {0};
    int index[N] = {0};
    struct int_array *tmp;
    bool flag = false;
    for (int j = 0; j < N; j++) {
        if (grid[row][j] == 0) {
            tmp = possible_cell_values(grid, row, j);
            int length = tmp->length;
            for (int i = 0; i < length; i++) {
                flag = true;
                possible[(tmp->array[i]) - 1] = possible[(tmp->array[i]) - 1] + 1;
                index[(tmp->array[i]) - 1] = j;
            }
        }

    }
    if (flag == true) {
        flag = false;
        for (int i = 0; i < N; i++) {
            if (possible[i] == 1) {
                flag = true;
                grid[row][index[i]] = i + 1;
            }
        }
    }
    return flag;
}

// Looks for values that only appear once within a given colum and allocates
bool hidden_values_col(int grid[N][N], int col) {
    int possible[N] = {0};
    int index[N] = {0};
    struct int_array *tmp;
    bool flag = false;
    for (int j = 0; j < N; j++) {
        if (grid[j][col] == 0) {
            tmp = possible_cell_values(grid, j, col);
            int length = tmp->length;
            for (int i = 0; i < length; i++) {
                flag = true;
                possible[(tmp->array[i]) - 1] = possible[(tmp->array[i]) - 1] + 1;
                index[(tmp->array[i]) - 1] = j;
            }
        }

    }
    if (flag == true) {
        flag = false;
        for (int i = 0; i < N; i++) {
            if (possible[i] == 1) {
                flag = true;
                grid[index[i]][col] = i + 1;
            }
        }
    }
    return flag;
}

// Looks for values that only appear once within a given square and allocates.
// Here squares are represented by the index of the upper left corner.
bool hidden_values_square(int grid[N][N], int square_row, int square_col) {
    int row;
    int col;
    int possible[N] = {0};
    int index[2][N] = {0};
    struct int_array *tmp;
    bool flag = false;
    for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 3; k++) {
            row = square_row + j;
            col = square_col + k;
            if (grid[row][col] == 0) {
                tmp = possible_cell_values(grid, row, col);
                int length = tmp->length;
                for (int i = 0; i < length; i++) {
                    flag = true;
                    possible[(tmp->array[i]) - 1] = possible[(tmp->array[i]) - 1] + 1;
                    index[0][(tmp->array[i]) - 1] = row;
                    index[1][(tmp->array[i]) - 1] = col;
                }
            }

        }
    }
    if (flag == true) {
        flag = false;
        for (int i = 0; i < N; i++) {
            if (possible[i] == 1) {
                flag = true;
                grid[index[0][i]][index[1][i]] = i + 1;
            }
        }
    }
    return flag;
}

/********************************************************************
 * HUMAN METHODS - the solver
 ********************************************************************/

void human_solver(int grid[N][N]) {
    bool flag = true;
    bool flag_tmp;

    while (flag == true) {
        flag = false;

        for (int i = 0; i < N ; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 0) {
                    struct int_array *tmp;
                    tmp = possible_cell_values(grid, i, j);
                    if (tmp->length == 1) {
                        grid[i][j] = tmp->array[0];
                        flag = true;
                    }
                    free_int_array(tmp);
                }
            }
        }

        for (int i = 0; i < N; i++) {
            flag = flag || hidden_values_col(grid, i);
            flag = flag || hidden_values_row(grid, i);
        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                flag = flag || hidden_values_square(grid, 3*i, 3*j);
            }

        }
        R_CheckUserInterrupt();
    }


}



/********************************************************************
 * BRUTE FORCE METHODS - backtracking
 * based on code on GeeksforGeeks (https://www.geeksforgeeks.org/sudoku-backtracking-7/)
 * which seems to in turn be based on the following class handout by J Zelenski
 * https://see.stanford.edu/materials/icspacs106b/H19-RecBacktrackExamples.pdf
 ********************************************************************/

// iterate across rows / cols (given via pointers) until a zero value is found
bool find_zero_value(int sudoku[N][N], int *row, int *col) {
    for (*row = 0; *row < N; (*row)++ ) {
        for (*col = 0; *col < N; (*col)++) {
            if (sudoku[*row][*col] == 0) {
                return true;
            }
        }
    }
    return false;
}

// is a value permitted in a given row
bool allowed_in_row(int sudoku[N][N], int row, int val) {
    for (int col = 0; col < N; col++) {
        if (sudoku[row][col] == val) {
            return false;
        }
    }
    return true;
}

// is a value permitted in a given column
bool allowed_in_col(int sudoku[N][N], int col, int val) {
    for (int row = 0; row < N; row++) {
        if (sudoku[row][col] == val) {
            return false;
        }
    }
    return true;
}

// is a value permitted in a given square
bool allowed_in_square(int sudoku[N][N], int row, int col, int val) {
    int square_row = 3 * (row / 3);
    int square_col = 3 * (col / 3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (sudoku[square_row + i][square_col + j] == val) {
                return false;
            }
        }
    }
    return true;
}

// is a value allowed at a particular index (i.e. row & col combination)
bool allowed(int sudoku[N][N], int row, int col, int val) {
    bool allowable = allowed_in_row(sudoku, row, val) &&
        allowed_in_col(sudoku, col, val) &&
        allowed_in_square(sudoku, row, col, val);

    return allowable;
}

/********************************************************************
 * BRUTE FORCE METHODS - solver
 ********************************************************************/
bool brute_solver(int sudoku[N][N]) {
    int row = 0;
    int col = 0;

    bool grid_complete = !find_zero_value(sudoku, &row, &col);
    if (grid_complete) {
        return true;
    }

    for(int n = 1; n <= 9; n++) {
        if (allowed(sudoku, row, col, n)) {
            sudoku[row][col] = n;
            if(brute_solver(sudoku)) {
                return true;
            }
            sudoku[row][col] = 0;
        }
    }
    return false;
}


/********************************************************************
 * SEXP WRAPPERS
 ********************************************************************/

// solver - start with human methods and then fall back to brute force if needed
SEXP call_solver(SEXP x) {
    int length = 81;
    int *arr = INTEGER(x);
    int sudoku[N][N];
    vec_to_matrix(arr, sudoku);
    human_solver(sudoku);
    if (brute_solver(sudoku)) {
        matrix_to_vec(arr, sudoku);
        SEXP ans = allocVector(INTSXP, length);
        PROTECT(ans);
        for (int i = 0; i < length; i++) {
            INTEGER(ans)[i] = arr[i];
        }
        UNPROTECT(1);
        return ans;
    } else {
        return x;
    }
}


// solver - human methods only
SEXP call_human_solver(SEXP x) {
    int length = N * N;
    int *arr = INTEGER(x);
    int sudoku[N][N];
    vec_to_matrix(arr, sudoku);
    human_solver(sudoku);
    matrix_to_vec(arr, sudoku);
    SEXP ans = allocVector(INTSXP, length);
    PROTECT(ans);
    for (int i = 0; i < length; i++) {
        INTEGER(ans)[i] = arr[i];
    }
    UNPROTECT(1);
    return ans;
}

// solver - brute force backtracking method only
SEXP call_brute_solver(SEXP x) {
    int length = 81;
    int *arr = INTEGER(x);
    int sudoku[N][N];
    vec_to_matrix(arr, sudoku);
    if (brute_solver(sudoku)) {
        matrix_to_vec(arr, sudoku);
        SEXP ans = allocVector(INTSXP, length);
        PROTECT(ans);
        for (int i = 0; i < length; i++) {
            INTEGER(ans)[i] = arr[i];
        }
        UNPROTECT(1);
        return ans;
    } else {
        return x;
    }
}





