#pragma once

#include "Board.hpp"
#include <memory>

class Menu {
public:
    void show_menu();
    void init_game();
    void make_move();
    int check_win() const;

private:
    std::unique_ptr<Board> board;
};