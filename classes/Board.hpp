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
    void print_board() const;
    int make_move(int row, int col, bool mine);
    int check_win() const;

private:
    static std::random_device rd;
    static std::uniform_int_distribution<int> dist;
    static std::vector<std::pair<int, int>> transitions;
    static std::wstring numerals;

    int height, width, mine_count, unopened_cells;
    std::vector<std::vector<wchar_t>> board;
    std::map<std::pair<int, int>, bool> mines;

    void print_board_loss();
    int count_adj(int x, int y) const;
    void reveal_adjacent(int x, int y);
    void generate_board(int first_x, int first_y);
};