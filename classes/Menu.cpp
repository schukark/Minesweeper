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

void Menu::print_board(SDL_Renderer* renderer, SDL_Color Empty, SDL_Color Unopened) const {
    
}
