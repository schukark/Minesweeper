#include "Menu.hpp"
#include <iostream>
#include <string>


void Menu::show_menu() {
    std::cout << "Possible commands:" << "\n";
    std::cout << "init - Start game" << "\n";
    std::cout << "quit - Exit game" << std::endl;

    std::string command;
    std::cin >> command;

    if (command == "init") {
        init_game();
    }
    else if (command == "quit") {
        return;
    }
}

void Menu::init_game() {
    std::cout << "Possible difficulties:" << "\n";
    std::cout << "beginner, intermediate, hard, expert, custom" << std::endl;

    std::string diff;
    std::cin >> diff;

    if (diff == "beginner") {
        board = std::move(std::make_unique<Board>(Board(8, 8, 10)));
    }
    else if (diff == "intermediate") {
        board = std::move(std::make_unique<Board>(Board(16, 16, 40)));
    }
    else if (diff == "hard") {
        board = std::move(std::make_unique<Board>(Board(20, 20, 72)));
    }
    else if (diff == "expert") {
        board = std::move(std::make_unique<Board>(Board(30, 16, 99)));
    }
    else if (diff == "custom") {
        int height, width, mines;
        std::cout << "Enter height, width and number of mines:" << std::endl;
        std::cin >> height >> width >> mines;

        board = std::move(std::make_unique<Board>(Board(height, width, mines)));
    }
    else {
        std::cout << "No such difficulty" << std::endl;
    }
    board->print_board();
    make_move();
}

void Menu::make_move() {
    std::cout << "Enter move in the following format: column, row, mine/flag" << std::endl;
    int column, row;
    bool mine;

    std::cin >> column >> row >> mine;

    if (board->make_move(row, column, mine)) {
        std::wcout << L"You lost" << std::endl;
        return;
    }
    board->print_board();
    make_move();
}
