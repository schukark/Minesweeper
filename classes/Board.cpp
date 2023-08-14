#include "Board.hpp"

std::random_device Board::rd;
std::uniform_int_distribution<int> Board::dist;
std::vector<std::pair<int, int>> Board::transitions;


Board::Board(int height, int width, int mine_count) : height(height), width(width), mine_count(mine_count) {
    dist = std::uniform_int_distribution<int>(0, height * width - 1);
    transitions = std::vector<std::pair<int, int>>({ {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}, {-1, -1} });

    int count = 0;
    while (count < mine_count) {
        int new_mines = dist(rd);
        int x = new_mines / width;
        int y = new_mines - x * width;

        if (mines.count(std::make_pair(x, y)) > 0) continue;
        mines[std::make_pair(x, y)] = true;
        count++;
    }

    board = std::move(std::vector<std::vector<wchar_t>>(height, std::vector<wchar_t>(width, L'⬛')));
}

void Board::print_board() const {
    for (const auto& row : board) {
        for (const auto& elem : row) {
            std::wcout << elem;
        }
        std::wcout << std::endl;
    }
}

int Board::make_move(int row, int col, bool mine) {
    row--;
    col--;

    if (row >= height || col >= width || row < 0 || col < 0) {
        std::wcout << L"Incorrect move" << std::endl;
        return 1;
    }

    if (mine && mines.count(std::make_pair(row, col)) > 0) {
        print_board_loss();
    }

    int count_adjacent = count_adj(row, col);

    if (count_adjacent && mine) {
        board[row][col] = std::to_wstring(count_adjacent)[0];
        return 0;
    }

    if (mine) {
        board[row][col] = L'⬜';
    }
    else {
        board[row][col] = L'⚑';
    }

    return 0;
}

int Board::count_adj(int x, int y) const {
    int count = 0;

    for (const auto& [dx, dy] : transitions) {
        int new_x = x + dx;
        int new_y = y + dy;

        if (new_x < 0 || new_x >= height || new_y < 0 || new_y >= width) continue;
        count += mines.count(std::make_pair(new_x, new_y)) > 0;
    }

    return count;
}

void Board::print_board_loss() {
    for (const auto& [key, value] : mines) {
        board[key.first][key.second] = L'*';
    }

    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            int count = count_adj(x, y);

            if (count) std::wcout << std::to_wstring(count);
            else std::wcout << board[x][y];
        }
        std::wcout << std::endl;
    }
}
