#include<iostream>
#include<stdio.h>
#include <ctime>
#include <optional>
#include <vector>

int hasWinner(int game_board[3][3]);
bool isValid(int game_board[3][3], int row, int col);
void printBoard(int game_board[3][3]);
int playGame();
bool isFull(int game_board[3][3]);


struct Location {
    Location() {};
    Location(int Score) : score(Score) {}
    Location(int Row, int Column) : row(Row), column(Column) {}
    int row = 0;
    int column =  0;
    int score = 0;
};

Location makeRandomGuess(int game_board[3][3]);
Location makeBetterRandomGuess(int game_board[3][3], int player);
std::optional<Location> twoInLine(int game_board[3][3], int player);
Location makeBestMove(int game_board[3][3], int player);

int main() {
    srand((unsigned)time(NULL));
    int fdsa[3][3] = { {1,2,1}, {2,0,2}, {1,2,1} };
    std::cout << hasWinner(fdsa) << std::endl;
    int counts[3] = { 0,0,0 };
    for (int i = 0; i < 100000; i++) {
        int game = playGame();
        counts[game]++;
    }
    std::cout << counts[0] << " draws" << std::endl;
    std::cout << counts[1] << " player 1" << std::endl;
    std::cout << counts[2] << " player 2" << std::endl;
    return 0;
}

int playGame() {
    int winner = 0;
    int game_board[3][3] = { {0,0,0}, {0,0,0}, {0,0,0} };
    while (hasWinner(game_board) == 0 && !isFull(game_board)) {
        Location temp = makeBestMove(game_board, 1);
        game_board[temp.row][temp.column] = 1;
        temp = makeRandomGuess(game_board);
        game_board[temp.row][temp.column] = 2;
        
        int winner = hasWinner(game_board);
    }
    return hasWinner(game_board);
}

Location makeBetterRandomGuess(int game_board[3][3], int player) {
    Location loc;
    std::optional<Location> maybe_good_spot = twoInLine(game_board, player);
    if (maybe_good_spot.has_value()) {
        return maybe_good_spot.value();
    }
    if (game_board[0][0] == 0) {
        loc.row = 0;
        loc.column = 0;
        return loc;
    }
    if (game_board[1][1] == 0) {
        loc.row = 1;
        loc.column = 1;
        return loc;
    }
    return makeRandomGuess(game_board);

}

std::optional<Location> twoInLine(int game_board[3][3], int player) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (game_board[row][col] == 0) {
                game_board[row][col] = player;
                if (hasWinner(game_board)) {
                    return std::optional<Location>({row, col});
                }
                game_board[row][col] = 0;
            }
        }
    }
    return std::optional<Location>();
}

Location makeRandomGuess(int game_board[3][3]) {
    Location random_move;
    random_move.row = rand() % 3;
    random_move.column = rand() % 3;
    bool not_valid = true;
    if (!isFull(game_board)) {
        if (isValid(game_board, random_move.row, random_move.column)) {
            not_valid = false;
        }
        else {
            while (not_valid) {
                random_move.row = rand() % 3;
                random_move.column = rand() % 3;
                if (isValid(game_board, random_move.row, random_move.column)) {
                    not_valid = false;
                }
            }
        }
    }
    return random_move;
}

Location makeBestMove(int game_board[3][3], int player) {

    int win = hasWinner(game_board);
    if (win == 1) {
        return Location(10);
    }
    else if (win == 2) {
        return Location(-10);
    }
    else if (win == 0) {
        return Location(0);
    }
    
    std::vector<Location> moves;

    for (int y = 0; y < sizeof(game_board); y++) {
        for (int x = 0; x < sizeof(game_board[0]); x++) {
            if (game_board[x][y] == 0) {
                Location temp;
                temp.row = x;
                temp.column = y;
                game_board[x][y] = player;
                if (player == 1) {
                    temp.score = makeBestMove(game_board, 1).score;
                }
                else {
                    temp.score = makeBestMove(game_board, 2).score;
                }
                moves.push_back(temp);
                game_board[x][y] = 0;
            }
        }
    }
    int best_move = 0;
    if (player == 2) {
        int best_score = -10000000;
        for (int i = 0; i < moves.size(); i++) {
            if (moves[i].score > best_score) {
                best_move = i;
                best_score = moves[i].score;
            }
        }
    }
    else {
        int best_score = 10000000;
        for (int i = 0; i < moves.size(); i++) {
            if (moves[i].score < best_score) {
                best_move = i;
                best_score = moves[i].score;
            }
        }
    }
    return moves[best_move];
}


bool isFull(int game_board[3][3]) {
    bool isFull = true;
    for (int idx = 0; idx < 3; idx++) {
        for (int jdx = 0; jdx < 3; jdx++) {
            if (game_board[idx][jdx] == 0) {
                isFull = false;
            }
        }
    }
    if (isFull) {
        return true;;
    }
    return false;
}

void printBoard(int game_board[3][3]) {
    for (int idx = 0; idx < 3; idx++) {
        for (int jdx = 0; jdx < 3; jdx++) {
            std::cout << game_board[idx][jdx] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "------" << std::endl;
}

bool isValid(int game_board[3][3], int row, int col) {
    return (game_board[row][col] == 0);
}

int hasWinner(int game_board[3][3]) {

    for (int player = 1; player < 3; player++) {

        for (int row = 0; row < 3; row++) {

            if (game_board[row][0] == player && game_board[row][1] == player && game_board[row][2] == player) {
                return player;
            }
            if (game_board[0][row] == player && game_board[1][row] == player && game_board[2][row] == player) {
                return player;
            }
        }
        if (game_board[0][0] == player && game_board[1][1] == player && game_board[2][2] == player) {
            return player;
        }
        if (game_board[0][2] == player && game_board[1][1] == player && game_board[2][0] == player) {
            return player;
        }
    }

    return 0;
}
