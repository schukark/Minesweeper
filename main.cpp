#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <memory>

#include "classes\Menu.cpp"
#include "classes\Board.cpp"

int main(int argc, char **argv) {
    _setmode(_fileno(stdout), _O_U8TEXT);
    std::unique_ptr<Menu> game(new Menu);
    game->init_game();

    return 0;
}