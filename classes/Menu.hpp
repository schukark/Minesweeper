#pragma once

#include "Board.hpp"
#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>
#include <memory>
#include <tuple>

class Menu {
public:
    std::tuple<int, int, int> init_game();
    void make_move(int row, int col, bool mine);
    int check_win() const;
    void print_board(SDL_Renderer* renderer, SDL_Color Empty, SDL_Color Unopened, int grid_size) const;

private:
    static SDL_Texture* create_text_window(SDL_Renderer* renderer, int x, int y, const char* text, 
                                    TTF_Font* font, SDL_Rect* rect, int grid_cell_size);
    std::unique_ptr<Board> board;
};