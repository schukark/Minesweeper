#include "Menu.hpp"
#include <iostream>
#include <string>

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

int Menu::make_move(int row, int col, bool mine) {
    return board->make_move(row, col, mine);
}

int Menu::check_win() const {
    return board->check_win();
}

void Menu::print_board(SDL_Renderer* renderer, SDL_Color Empty, SDL_Color Unopened, int grid_size, TTF_Font* font, bool loss) const {
    if (!font) {
        std::wcout << L"Error loading font" << std::endl;
    }

    SDL_Surface *image = NULL;
    SDL_Texture *bomb = NULL;
    SDL_Surface *image2 = SDL_LoadBMP("flag.bmp");
    SDL_Texture *flag = SDL_CreateTextureFromSurface(renderer, image2);
    SDL_FreeSurface(image2);

    std::wstring numerals_wide = L"０１２３４５６７８９";

    if (loss) {
        board->make_board_loss();
        image = SDL_LoadBMP("bomb.bmp");
        bomb = SDL_CreateTextureFromSurface(renderer, image);
        SDL_FreeSurface(image);
    }

    for (int i = 0; i < board->get_board()[0].size(); i++) {
        for (int j = 0; j < board->get_board().size(); j++) {

            if (numerals_wide.find(board->get_board()[j][i]) == std::wstring::npos) {

                SDL_Rect rect = {.x = grid_size * j + 1, .y = grid_size * i + 1, .w = grid_size - 2, .h = grid_size - 2};

                if (loss && board->get_mines().count(std::make_pair(j, i)) > 0) {
                    SDL_RenderCopy(renderer, bomb, NULL, &rect);
                    continue;
                }

                if (board->get_board()[j][i] == EMPTY_SPACE) {
                    SDL_SetRenderDrawColor(renderer, Empty.r,
                        Empty.g, Empty.b,
                        Empty.a);
                }

                else if (board->get_board()[j][i] == NOT_PRESSED) {                    
                    SDL_SetRenderDrawColor(renderer, Unopened.r,
                        Unopened.g, Unopened.b,
                        Unopened.a);
                }
                else {
                    SDL_RenderCopy(renderer, flag, NULL, &rect);
                    continue;
                }
                
                SDL_RenderFillRect(renderer, &rect);
            }

            else {
                int count = board->get_board()[j][i] - numerals_wide[0];
                SDL_Rect rect;
                SDL_Texture *text = create_text_window(renderer, grid_size * j + 1, grid_size * i + 1, 
                                                    std::to_string(count).c_str(), font, &rect, grid_size);

                SDL_RenderCopy(renderer, text, NULL, &rect);
                SDL_DestroyTexture(text);
            }
        }
    }

    if (loss) {
        SDL_DestroyTexture(bomb);
    }

    SDL_DestroyTexture(flag);
    SDL_RenderPresent(renderer);
}

SDL_Texture* Menu::create_text_window(SDL_Renderer* renderer, int x, int y, const char* text, 
                                        TTF_Font* font, SDL_Rect* rect, int grid_cell_size) {
    SDL_Color textColor = {255, 255, 255, 0};

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int text_width = surface->w;
    int text_height = surface->h;

    SDL_FreeSurface(surface);

    rect->x = x;
    rect->y = y;
    rect->w = std::min(text_width, grid_cell_size - 2);
    rect->h = std::min(text_height, grid_cell_size - 2);

    return texture;
}
