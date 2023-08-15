#pragma once

#include "Board.hpp"
#include <SDL2\SDL.h>
#include <memory>
#include <tuple>

class Menu {
public:
    std::tuple<int, int, int> init_game();
    void make_move(int row, int col, bool mine);
    int check_win() const;
    void print_board(SDL_Renderer* renderer, SDL_Color Empty, SDL_Color Unopened) const;

private:
    std::unique_ptr<Board> board;
};