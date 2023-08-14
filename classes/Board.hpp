#pragma once

#include <vector>
#include <iostream>
#include <map>
#include <random>

class Board {
public:
    Board(int height, int width, int mine_count);
    void print_board() const;
    int make_move(int row, int col, bool mine);

private:
    static std::random_device rd;
    static std::uniform_int_distribution<int> dist;
    static std::vector<std::pair<int, int>> transitions;

    int height, width, mine_count;
    std::vector<std::vector<wchar_t>> board;
    std::map<std::pair<int, int>, bool> mines;

    void print_board_loss();
    int count_adj(int x, int y) const;
};