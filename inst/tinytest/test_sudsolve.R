# test sudokuAlt gives identical results ----------------------------------

# test 1: puzzle 1
if (requireNamespace("sudokuAlt", quietly = TRUE)) {
    r1 <- c(0, 0, 0, 7, 0, 0, 0, 0, 0)
    r2 <- c(1, 0, 0, 0, 0, 0, 0, 0, 0)
    r3 <- c(0, 0, 0, 4, 3, 0, 2, 0, 0)
    r4 <- c(0, 0, 0, 0, 0, 0, 0, 0, 6)
    r5 <- c(0, 0, 0, 5, 0, 9, 0, 0, 0)
    r6 <- c(0, 0, 0, 0, 0, 0, 4, 1, 8)
    r7 <- c(0, 0, 0, 0, 8, 1, 0, 0, 0)
    r8 <- c(0, 0, 2, 0, 0, 0, 0, 5, 0)
    r9 <- c(0, 4, 0, 0, 0, 0, 3, 0, 0)

    game <- rbind(r1,r2,r3,r4,r5,r6,r7,r8,r9)
    result <- sudsolve(game)
    expected <- sudokuAlt::solveGame(game)
    expect_identical(result, expected, "testing puzzle 1 solution matches sudokuAlt")
}


# test 2: puzzle 2
if (requireNamespace("sudokuAlt", quietly = TRUE)) {
    r1 <- c(4, 0, 0, 0, 0, 0, 0, 0, 7)
    r2 <- c(0, 0, 0, 8, 5, 7, 6, 0, 0)
    r3 <- c(0, 0, 0, 0, 6, 0, 9, 0, 0)
    r4 <- c(0, 9, 0, 0, 0, 0, 0, 0, 5)
    r5 <- c(1, 0, 5, 6, 0, 8, 4, 0, 3)
    r6 <- c(8, 0, 0, 0, 0, 0, 0, 1, 0)
    r7 <- c(0, 0, 7, 0, 8, 0, 0, 0, 0)
    r8 <- c(0, 0, 3, 4, 7, 1, 0, 0, 0)
    r9 <- c(6, 0, 0, 0, 0, 0, 0, 0, 9)

    game <- rbind(r1,r2,r3,r4,r5,r6,r7,r8,r9)
    result <- sudsolve(game)
    expected <- sudokuAlt::solveGame(game)
    expect_identical(result, expected, "testing puzzle 2 solution matches sudokuAlt")
}



# test 3: puzzle 3
if (requireNamespace("sudokuAlt", quietly = TRUE)) {
    r1 <- c(8, 0, 0, 0, 0, 0, 0, 0, 0)
    r2 <- c(0, 0, 3, 6, 0, 0, 0, 0, 0)
    r3 <- c(0, 7, 0, 0, 9, 0, 2, 0, 0)
    r4 <- c(0, 5, 0, 0, 0, 7, 0, 0, 0)
    r5 <- c(0, 0, 0, 0, 4, 5, 7, 0, 0)
    r6 <- c(0, 0, 0, 1, 0, 0, 0, 3, 0)
    r7 <- c(0, 0, 1, 0, 0, 0, 0, 6, 8)
    r8 <- c(0, 0, 8, 5, 0, 0, 0, 1, 0)
    r9 <- c(0, 9, 0, 0, 0, 0, 4, 0, 0)

    game <- rbind(r1,r2,r3,r4,r5,r6,r7,r8,r9)
    result <- sudsolve(game)
    expected <- sudokuAlt::solveGame(game)
    expect_identical(result, expected, "testing puzzle 3 solution matches sudokuAlt")
}



# test 4: brute force solver matches combined solver ----------------------
r1 <- c(8, 0, 0, 0, 0, 0, 0, 0, 0)
r2 <- c(0, 0, 3, 6, 0, 0, 0, 0, 0)
r3 <- c(0, 7, 0, 0, 9, 0, 2, 0, 0)
r4 <- c(0, 5, 0, 0, 0, 7, 0, 0, 0)
r5 <- c(0, 0, 0, 0, 4, 5, 7, 0, 0)
r6 <- c(0, 0, 0, 1, 0, 0, 0, 3, 0)
r7 <- c(0, 0, 1, 0, 0, 0, 0, 6, 8)
r8 <- c(0, 0, 8, 5, 0, 0, 0, 1, 0)
r9 <- c(0, 9, 0, 0, 0, 0, 4, 0, 0)

game <- rbind(r1,r2,r3,r4,r5,r6,r7,r8,r9)
expected <- sudsolve(game)
result <- sudsolve_brute(game)
expect_identical(result, expected, "testing combined solver matches brute solver")
