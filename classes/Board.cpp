#include "Board.hpp"

std::random_device Board::rd;
std::uniform_int_distribution<int> Board::dist;
std::vector<std::pair<int, int>> Board::transitions;
std::wstring Board::numerals;


Board::Board(int height, int width, int mine_count) : height(height), width(width), mine_count(mine_count), unopened_cells(height * width) {
    dist = std::uniform_int_distribution<int>(0, height * width - 1);
    transitions = std::vector<std::pair<int, int>>({ {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}});
    numerals = L"０１２３４５６７８９";
    board = std::move(std::vector<std::vector<wchar_t>>(height, std::vector<wchar_t>(width, EMPTY_SPACE)));
}

int Board::make_move(int row, int col, bool mine) {
    if (row >= height || col >= width || row < 0 || col < 0) {
        std::wcout << L"Incorrect move, out of bounds" << std::endl;
        std::wcout << row << " " << col << std::endl;
        return 0;
    }

    if (mine && mines.count(std::make_pair(row, col)) > 0) {
        print_board_loss();
        return 1;
    }

    int count_adjacent = count_adj(row, col);

    if (count_adjacent && mine) {
        board[row][col] = numerals[count_adjacent];
        unopened_cells--;
        return 0;
    }

    if (mine && unopened_cells == height * width) {
        generate_board(row, col);
    }
    if (mine) {
        board[row][col] = NOT_PRESSED;
        reveal_adjacent(row, col);
    }
    else {
        board[row][col] = FLAG;
        unopened_cells--;
    }

    //std::wcout << unopened_cells << std::endl;
    return 0;
}

int Board::check_win() const {
    return unopened_cells == mine_count;
}

std::tuple<int, int, int> Board::info() const {
    return {height, width, mine_count};
}

std::vector<std::vector<wchar_t>> Board::get_board() const {
    return board;
}

int Board::count_adj(int x, int y) const {
    int count = 0;

    for (const auto& [dx, dy] : transitions) {
        int new_x = x + dx;
        int new_y = y + dy;

        if (new_x < 0 || new_x >= height || new_y < 0 || new_y >= width) continue;
        if (mines.count(std::make_pair(new_x, new_y)) > 0) {
            count++;
        }
    }

    return count;
}

void Board::reveal_adjacent(int x, int y) {
    unopened_cells--;
    board[x][y] = (count_adj(x, y) ? numerals[count_adj(x, y)] : NOT_PRESSED);
    if (count_adj(x, y)) return;

    for (const auto& [dx, dy] : transitions) {
        int new_x = x + dx;
        int new_y = y + dy;

        if (new_x < 0 || new_x >= height || new_y < 0 || new_y >= width) continue;

        if (board[new_x][new_y] == EMPTY_SPACE && !mines.count(std::make_pair(new_x, new_y))) reveal_adjacent(new_x, new_y);
    }
}

void Board::generate_board(int first_x, int first_y) {
    int count = 0;
    while (count < mine_count) {
        int new_mines = dist(rd);
        int x = new_mines / width;
        int y = new_mines - x * width;

        if (mines.count(std::make_pair(x, y)) > 0 || (x == first_x && y == first_y)) continue;
        mines[std::make_pair(x, y)] = true;
        count++;
    }
}