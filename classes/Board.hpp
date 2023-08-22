#pragma once

#include <vector>
#include <iostream>
#include <map>
#include <random>

#define BOMB L'＊'
#define EMPTY_SPACE L'⬛'
#define NOT_PRESSED L'⬜'
#define FLAG L'？'

class Board {
public:
    Board(int height, int width, int mine_count);
    int make_move(int row, int col, bool mine);
    int check_win() const;
    std::tuple<int, int, int> info() const;
    std::vector<std::vector<wchar_t>> get_board() const;
    void make_board_loss();
    std::map<std::pair<int, int>, bool> get_mines() const;

private:
    static std::random_device rd;
    static std::uniform_int_distribution<int> dist;
    static std::vector<std::pair<int, int>> transitions;
    static std::wstring numerals;

    int height, width, mine_count, unopened_cells;
    std::vector<std::vector<wchar_t>> board;
    std::map<std::pair<int, int>, bool> mines;

    int count_adj(int x, int y) const;
    void reveal_adjacent(int x, int y);
    void generate_board(int first_x, int first_y);
};