#include "Menu.hpp"
#include <iostream>
#include <string>
#include <cstdint>

std::tuple<int, int, int> Menu::init_game() {
    std::wcout << L"Possible difficulties:" << std::endl;
    std::wcout << L"beginner, intermediate, hard, expert, custom" << std::endl;

    std::wstring diff;
    std::getline(std::wcin, diff);

    if (diff == L"beginner") {
        board = std::move(std::make_unique<Board>(Board(8, 8, 10)));
    }
    else if (diff == L"intermediate") {
        board = std::move(std::make_unique<Board>(Board(16, 16, 40)));
    }
    else if (diff == L"hard") {
        board = std::move(std::make_unique<Board>(Board(20, 20, 72)));
    }
    else if (diff == L"expert") {
        board = std::move(std::make_unique<Board>(Board(30, 16, 99)));
    }
    else if (diff == L"custom") {
        int height, width, mines;
        std::wcout << L"Enter height, width and number of mines:" << std::endl;
        std::wcin >> height >> width >> mines;

        if (mines >= height * width) {
            std::wcout << L"Too much bombs!" << std::endl;
            return {-1, -1, -1};
        }
        else if (mines <= 0) {
            std::wcout << L"Too little bombs!" << std::endl;
            return {-1, -1, -1};
        }

        board = std::move(std::make_unique<Board>(Board(height, width, mines)));
    }
    else {
        std::wcout << L"No such difficulty" << std::endl;
        return {-1, -1, -1};
    }

    return board->info();
}

void Menu::make_move(int row, int col, bool mine) {
    if (board->make_move(row, col, mine)) {
        std::wcout << L"You lost" << std::endl;
        return;
    }

    if (check_win()) {
        std::wcout << L"You won!" << std::endl;
        return;
    }
}

int Menu::check_win() const {
    return board->check_win();
}

void Menu::print_board(SDL_Renderer* renderer, SDL_Color Empty, SDL_Color Unopened, int grid_size) const {
    TTF_Font* font = TTF_OpenFont("FreeSans.ttf", 24);
    std::wstring numerals_wide = L"０１２３４５６７８９";

    for (uint32_t i = 0; i < board->get_board().size(); i++) {
        for (uint32_t j = 0; j < board->get_board()[0].size(); i++) {

            if (numerals_wide.find(board->get_board()[i][j]) == std::wstring::npos) {

                if (board->get_board()[i][j] == EMPTY_SPACE) {
                    SDL_Rect rect = {.x = grid_size * i, .y = grid_size * j, .w = grid_size, .h = grid_size};
                    
                    SDL_SetRenderDrawColor(renderer, Empty.r,
                        Empty.g, Empty.b,
                        Empty.a);
                    SDL_RenderFillRect(renderer, &rect);
                }

                else {
                    SDL_Rect rect = {.x = grid_size * i, .y = grid_size * j, .w = grid_size, .h = grid_size};
                    
                    SDL_SetRenderDrawColor(renderer, Unopened.r,
                        Unopened.g, Unopened.b,
                        Unopened.a);
                    SDL_RenderFillRect(renderer, &rect);
                }
            }

            else {
                int count = board->get_board()[i][j] - numerals_wide[0];
                SDL_Rect rect;
                SDL_Texture *text = create_text_window(renderer, grid_size * i, grid_size * j, 
                                                    std::to_string(count).c_str(), font, &rect);

                SDL_RenderCopy(renderer, text, NULL, &rect);
                SDL_RenderPresent(renderer);
            }
        }
    }
}

SDL_Texture* Menu::create_text_window(SDL_Renderer* renderer, int x, int y, const char* text, 
                                        TTF_Font* font, SDL_Rect* rect) {
    SDL_Color textColor = {255, 255, 255, 0};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int text_width = surface->w;
    int text_height = surface->h;

    SDL_FreeSurface(surface);

    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;

    return texture;
}
