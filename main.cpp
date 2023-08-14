#include <iostream>
#include "classes\Menu.cpp"
#include "classes\Board.cpp"
#include <memory>

int main(int argc, char **argv) {
    std::unique_ptr<Menu> game(new Menu);
    game->init_game();

    return 0;
}