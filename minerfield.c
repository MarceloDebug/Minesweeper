#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function prototypes
void fillBoard(int rows, int cols, int **board, char **displayBoard, int *numMines);
void showFinalBoard(int rows, int cols, int **board);
void displayBoardState(int rows, int cols, char **displayBoard);
void readCoordinates(int rows, int cols, int *x, int *y);
void revealFields(int rows, int cols, int **board, char **displayBoard, int x, int y);

int main() {
    int playAgain = 1;

    while (playAgain != 0) {
        int option;
        int rows, cols;
        int x, y, i, j;
        int hitMine = 0;
        int totalMines = 0, foundMines = 0;
        char opPlayAgain;

        // Pointer creation
        int **board;
        char **displayBoard;

        // Initialize random seed
        srand(time(0));

        printf("\n");
        printf("███╗░░░███╗██╗███╗░░██╗███████╗░██████╗░██╗░░░░░░░██╗███████╗███████╗██████╗░███████╗██████╗░\n");
        printf("████╗░████║██║████╗░██║██╔════╝██╔════╝░██║░░██╗░░██║██╔════╝██╔════╝██╔══██╗██╔════╝██╔══██╗\n");
        printf("██╔████╔██║██║██╔██╗██║█████╗░░╚█████╗░░╚██╗████╗██╔╝█████╗░░█████╗░░██████╔╝█████╗░░██████╔╝\n");
        printf("██║╚██╔╝██║██║██║╚████║██╔══╝░░░╚═══██╗░░████╔═████║░██╔══╝░░██╔══╝░░██╔═══╝░██╔══╝░░██╔══██╗\n");
        printf("██║░╚═╝░██║██║██║░╚███║███████╗██████╔╝░░╚██╔╝░╚██╔╝░███████╗███████╗██║░░░░░███████╗██║░░██║\n");
        printf("╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝╚══════╝╚═════╝░░░░╚═╝░░░╚═╝░░╚══════╝╚══════╝╚═╝░░░░░╚══════╝╚═╝░░╚═╝n\n");

        printf("Choose an option:\n");
        printf("1 - 10x10 board\n");
        printf("2 - 20x20 board\n");
        printf("3 - 30x30 board\n");

        scanf("%d", &option);

        // Choose board size
        switch (option) {
            case 1:
                rows = 10;
                cols = 10;
                break;
            case 2:
                rows = 20;
                cols = 20;
                break;
            case 3:
                rows = 30;
                cols = 30;
                break;
            default:
                printf("Invalid option!\n");
                return 1;
        }

        // Dynamic allocation
        board = (int **)malloc(rows * sizeof(int *));
        displayBoard = (char **)malloc(rows * sizeof(char *));

        for (i = 0; i < rows; i++) {
            board[i] = (int *)malloc(cols * sizeof(int));
            displayBoard[i] = (char *)malloc(cols * sizeof(char));
        }

        // Fill both boards
        fillBoard(rows, cols, board, displayBoard, &totalMines);
        displayBoardState(rows, cols, displayBoard);
        printf("\n");

        // Keep playing until a mine is hit
        do {
            int move;
            printf("1 - Place a mine marker\n");
            printf("2 - Reveal field\n");
            scanf("%d", &move);

            switch (move) {
                case 1:
                    printf("Which field do you want to mark?\n");
                    readCoordinates(rows, cols, &x, &y);

                    if (board[x][y] == -1) {
                        foundMines++;
                    }

                    if (foundMines == totalMines) {
                        printf("Congratulations, you won!!!\n");
                        showFinalBoard(rows, cols, board);
                    } else {
                        displayBoard[x][y] = '#';
                        displayBoardState(rows, cols, displayBoard);
                        printf("Remaining mines: %d\n\n", totalMines - foundMines);
                    }
                    break;

                case 2:
                    printf("Which field do you want to reveal?\n");
                    readCoordinates(rows, cols, &x, &y);

                    if (board[x][y] == -1) {
                        hitMine = -1;
                        printf("You lost!!!\n");
                        showFinalBoard(rows, cols, board);
                    } else if (board[x][y] == 0) {
                        revealFields(rows, cols, board, displayBoard, x, y);
                        displayBoardState(rows, cols, displayBoard);
                    } else {
                        displayBoard[x][y] = board[x][y] + '0';
                        displayBoardState(rows, cols, displayBoard);
                    }
                    printf("Remaining mines: %d\n\n", totalMines - foundMines);
                    break;

                default:
                    printf("Enter a valid option\n\n");
                    break;
            }

        } while (hitMine != -1);

        // Free memory
        for (i = 0; i < rows; i++) {
            free(board[i]);
            free(displayBoard[i]);
        }
        free(board);
        free(displayBoard);

        printf("Play again? (Y/N) ");
        scanf(" %c", &opPlayAgain);

        if (opPlayAgain == 'N') {
            playAgain = 0;
        }
    }

    return 0;
}

// Fill board with mines and numbers
void fillBoard(int rows, int cols, int **board, char **displayBoard, int *numMines) {
    int i, j, x, y;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (rand() % 9 == 0) {
                board[i][j] = -1;
                (*numMines)++;
            } else {
                board[i][j] = 0;
            }
        }
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (board[i][j] == -1) {
                for (x = -1; x <= 1; x++) {
                    for (y = -1; y <= 1; y++) {
                        int rowX = i + x;
                        int colY = j + y;
                        if (rowX >= 0 && rowX < rows && colY >= 0 && colY < cols && board[rowX][colY] != -1) {
                            board[rowX][colY] += 1;
                        }
                    }
                }
            }
        }
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            displayBoard[i][j] = '.';
        }
    }
}

// Show final board with mine locations
void showFinalBoard(int rows, int cols, int **board) {
    int i, j;
    printf("    ");
    for (j = 0; j < cols; j++) {
        printf("%3d", j);
    }
    printf("\n");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < rows; i++) {
        printf(" %2d |", i);
        for (j = 0; j < cols; j++) {
            if (board[i][j] == -1)
                printf("%3s", "X");
            else
                printf("%3d", board[i][j]);
        }
        printf("\n");
    }
}

// Display visible state of the board
void displayBoardState(int rows, int cols, char **displayBoard) {
    int i, j;
    printf("    ");
    for (j = 0; j < cols; j++) {
        printf("%3d", j);
    }
    printf("\n");

    for (i = 0; i < rows; i++) {
        printf("%2d |", i);
        for (j = 0; j < cols; j++) {
            printf("%3c", displayBoard[i][j]);
        }
        printf("\n");
    }
}

// Read and validate coordinates from user
void readCoordinates(int rows, int cols, int *x, int *y) {
    while (1) {
        printf("Row number: ");
        scanf("%d", x);
        printf("Column number: ");
        scanf("%d", y);

        if (*x >= rows || *x < 0 || *y >= cols || *y < 0) {
            printf("Invalid coordinates, try again\n\n");
        } else {
            break;
        }
    }
}

// Recursively reveal empty fields
void revealFields(int rows, int cols, int **board, char **displayBoard, int x, int y) {
    int dx, dy;

    if (x < 0 || y < 0 || x >= rows || y >= cols) {
        return;
    }

    if (displayBoard[x][y] != '.') {
        return;
    }

    if (board[x][y] == 0) {
        displayBoard[x][y] = '0';
    } else {
        displayBoard[x][y] = board[x][y] + '0';
        return;
    }

    for (dx = -1; dx <= 1; dx++) {
        for (dy = -1; dy <= 1; dy++) {
            if (dx != 0 || dy != 0) {
                revealFields(rows, cols, board, displayBoard, x + dx, y + dy);
            }
        }
    }
}
