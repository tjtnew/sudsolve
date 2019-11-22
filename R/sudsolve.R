## usethis namespace: start
#' @useDynLib sudsolve, .registration = TRUE
## usethis namespace: end
NULL


#' Sudoku solver
#'
#' Solves a given sudoku, first trying human methods (currently just lone and
#' hidden singles) and then useing a brute force backtracking approach.
#'
#' @param sudoku the puzzle to be solved
#' @export
solveGameQuick <- function(sudoku) {
    tmp <- as.integer(t(sudoku))
    if (anyNA(tmp)) {
        tmp[is.na(tmp)] <- 0L
    }
    tmp <- .Call(call_solver, tmp)
    tmp <- t(matrix(tmp, nrow = 9, ncol = 9))

    if (requireNamespace("sudokuAlt", quietly = TRUE)) {
        # for compatibility with sudokuAlt
        is.na(sudoku[!(sudoku %in% 1:9)]) <- TRUE
        storage.mode(sudoku) <- "character"
        storage.mode(tmp) <- "character"
        structure(tmp, game = sudoku, class = "sudoku")
    } else {
        tmp
    }
}


#' Sudoku solver (human methods only) - NOT EXPORTED
#'
#' Attempts to solve a sudoku using human methods (currently just lone and
#' hidden singles)
#'
#' @param sudoku the puzzle to be solved
solveHuman <- function(sudoku) {
    tmp <- as.integer(t(sudoku))
    if (anyNA(tmp)) {
        tmp[is.na(tmp)] <- 0L
    }

    tmp <- .Call(call_human_solver, tmp)
    tmp <- t(matrix(tmp, nrow = 9, ncol = 9))
    if (requireNamespace("sudokuAlt", quietly = TRUE)) {
        # for compatibility with sudokuAlt
        is.na(sudoku[!(sudoku %in% 1:9)]) <- TRUE
        storage.mode(sudoku) <- "character"
        storage.mode(tmp) <- "character"
        structure(tmp, game = sudoku, class = "sudoku")
    } else {
        tmp
    }
}


#' Sudoku solver (brute force only) - NOT EXPORTED
#'
#' Solves a given sudoku using the brute-force backtracing method
#'
#' @param sudoku the puzzle to be solved
#' @export
solveBrute <- function(sudoku) {
    tmp <- as.integer(t(sudoku))
    if (anyNA(tmp)) {
        tmp[is.na(tmp)] <- 0L
    }

    tmp <- .Call(call_brute_solver, tmp)
    tmp <- t(matrix(tmp, nrow = 9, ncol = 9))
    if (requireNamespace("sudokuAlt", quietly = TRUE)) {
        # for compatibility with sudokuAlt
        is.na(sudoku[!(sudoku %in% 1:9)]) <- TRUE
        storage.mode(sudoku) <- "character"
        storage.mode(tmp) <- "character"
        structure(tmp, game = sudoku, class = "sudoku")
    } else {
        tmp
    }
}
