#pragma once

#include "Board.hpp"
#include <memory>
#include <tuple>

class Menu {
public:
    void show_menu();
    std::tuple<int, int, int> init_game();
    void make_move();
    int check_win() const;

private:
    std::unique_ptr<Board> board;
};