#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <memory>
#include <SDL2\SDL.h>

#include "classes\Menu.cpp"
#include "classes\Board.cpp"

#define HEIGHT 480
#define WIDTH 640

int main(int argc, char **argv) {
    _setmode(_fileno(stdout), _O_U8TEXT);
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::wcout << "Failed to initialize SDL" << "\n";
        std::wcout << "SDL2 Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Minesweeper", 
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, HEIGHT, WIDTH, 0);
    if (!window) {
        std::wcout << "Couldn't create SDL window" << "\n";
        std::wcout << "SDL2 Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    if(!window_surface) {
        std::wcout << "Failed to get the surface from the window\n";
        std::wcout << "SDL2 Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    bool keep_window_open = true;
    while(keep_window_open) {
        SDL_Event event;

        while (SDL_PollEvent(&event) > 0) {
            switch(event.type) {
                case SDL_QUIT: 
                    keep_window_open = false;
                    break;
                
                case SDL_MOUSEBUTTONDOWN: 
                    std::wcout << event.motion.x << " " << event.motion.y << std::endl;
                    break;
            }
            
            SDL_UpdateWindowSurface(window);
        }
    }

    //std::unique_ptr<Menu> game(new Menu);
    //game->init_game();

    return 0;
}