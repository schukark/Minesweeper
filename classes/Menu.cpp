#include "Menu.hpp"
#include <iostream>
#include <string>


void Menu::show_menu() {
    std::wcout << L"Possible commands:" << std::endl;
    std::wcout << L"init - Start game" << std::endl;
    std::wcout << L"quit - Exit game" << std::endl;

    std::wstring command;
    std::getline(std::wcin, command);

    if (command == L"init") {
        init_game();
    }
    else if (command == L"quit") {
        return;
    }
}

void Menu::init_game() {
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
            return;
        }
        else if (mines <= 0) {
            std::wcout << L"Too little bombs!" << std::endl;
            return;
        }

        board = std::move(std::make_unique<Board>(Board(height, width, mines)));
    }
    else {
        std::wcout << L"No such difficulty" << std::endl;
        return;
    }

    board->print_board();
    make_move();
}

void Menu::make_move() {
    std::wcout << L"Enter move in the following format: column, row, mine/flag" << std::endl;
    int column, row;
    std::wstring word;

    std::wcin >> column >> row >> word;

    if (word != L"flag" && word != L"mine") {
        std::wcout << L"Incorrect move" << std::endl;
        make_move();
        return;
    }

    if (board->make_move(row, column, word == L"mine")) {
        std::wcout << L"You lost" << std::endl;
        return;
    }
    board->print_board();
    if (check_win()) {
        std::wcout << L"You won!" << std::endl;
        return;
    }
    make_move();
}

int Menu::check_win() const {
    return board->check_win();
}
