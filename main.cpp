#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <memory>

#include "SDL2\SDL.h"
#include "SDL2\SDL_ttf.h"
#include "classes\Board.cpp"
#include "classes\Menu.cpp"

#define FPS 60
#define WIDTH 480
#define HEIGHT 640

int main(int argc, char** argv) {
    _setmode(_fileno(stdout), _O_U8TEXT);

    int grid_width, grid_height, mine_count;
    std::unique_ptr<Menu> gameboard(new Menu());
    std::tie(grid_width, grid_height, mine_count) = gameboard->init_game();

    if (grid_width < 0) {
        std::wcout << "Gameboard initialization failed" << std::endl;
        return -1;
    }

    std::wcout << "Gameboard initialized" << std::endl;

    int grid_cell_size = std::max(36, std::min(WIDTH / grid_width, HEIGHT / grid_height));
    int window_width = (grid_width * grid_cell_size) + 1;
    int window_height = (grid_height * grid_cell_size) + 1;

    // Place the grid cursor in the middle of the screen
    SDL_Rect grid_cursor = {
        .x = (grid_width - 1) / 2 * grid_cell_size,
        .y = (grid_height - 1) / 2 * grid_cell_size,
        .w = grid_cell_size,
        .h = grid_cell_size,
    };

    // Ghost cursor that will be displayed under mouse cursor
    SDL_Rect grid_cursor_ghost = { grid_cursor.x, grid_cursor.y, grid_cell_size,
                                grid_cell_size };

    // All the colors
    SDL_Color grid_background = { 0, 0, 0, 255 }; // Black
    SDL_Color grid_line_color = { 44, 44, 44, 255 }; // Dark grey
    SDL_Color grid_cursor_ghost_color = { 44, 44, 44, 255 };
    SDL_Color grid_cursor_color = { 255, 255, 255, 255 }; // White

    // All the inits
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s",
            SDL_GetError());
        return EXIT_FAILURE;
    }

    if (TTF_Init() < 0) {
        return EXIT_FAILURE;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    if (SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window,
        &renderer) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Create window and renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_SetWindowTitle(window, "Minesweeper");

    // Various flags
    SDL_bool quit = SDL_FALSE;
    SDL_bool mouse_active = SDL_FALSE;
    SDL_bool mouse_hover = SDL_FALSE;
    SDL_bool not_first_click = SDL_FALSE;

    // Load font beforehand, to optimize
    TTF_Font* font = TTF_OpenFont("AovelSansRounded-rdDL.ttf", 24);

    while (!quit) {
        SDL_Event event;

        if (!SDL_PollEvent(&event)) continue;

        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                grid_cursor.x = (event.motion.x / grid_cell_size) * grid_cell_size;
                grid_cursor.y = (event.motion.y / grid_cell_size) * grid_cell_size;

                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (gameboard->make_move(grid_cursor.x / grid_cell_size, grid_cursor.y / grid_cell_size, true)) {
                        std::wcout << "You lost!" << std::endl;
                        gameboard->print_board(renderer, grid_cursor_color, grid_background, grid_cell_size, font, true);
                        quit = SDL_TRUE;
                        SDL_Delay(5000);
                    }

                    not_first_click = SDL_TRUE;
                }

                else if (event.button.button == SDL_BUTTON_RIGHT) {
                    if (gameboard->make_move(grid_cursor.x / grid_cell_size, grid_cursor.y / grid_cell_size, false)) {
                        std::wcout << "Incorrect move!" << std::endl;
                        gameboard->print_board(renderer, grid_cursor_color, grid_background, grid_cell_size, font, false);
                    }
                }

                break;

            case SDL_MOUSEMOTION:
                grid_cursor_ghost.x = (event.motion.x / grid_cell_size) * grid_cell_size;
                grid_cursor_ghost.y = (event.motion.y / grid_cell_size) * grid_cell_size;

                mouse_active = SDL_TRUE;
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover)
                    mouse_hover = SDL_TRUE;
                else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover)
                    mouse_hover = SDL_FALSE;
                break;

            case SDL_QUIT:
                quit = SDL_TRUE;
                break;
        }

        SDL_SetRenderDrawColor(renderer, grid_background.r, 
            grid_background.g, grid_background.b, grid_background.a);

        SDL_RenderClear(renderer);

        // Draw grid lines
        SDL_SetRenderDrawColor(renderer, grid_line_color.r, grid_line_color.g,
            grid_line_color.b, grid_line_color.a);

        for (int x = 0; x < 1 + grid_width * grid_cell_size; x += grid_cell_size) {
            SDL_RenderDrawLine(renderer, x, 0, x, window_height);
        }

        for (int y = 0; y < 1 + grid_height * grid_cell_size; y += grid_cell_size) {
            SDL_RenderDrawLine(renderer, 0, y, window_width, y);
        }

        // Draw grid ghost cursor
        if (mouse_active && mouse_hover) {
            SDL_SetRenderDrawColor(renderer, grid_cursor_ghost_color.r,
                grid_cursor_ghost_color.g,
                grid_cursor_ghost_color.b,
                grid_cursor_ghost_color.a);
            SDL_RenderFillRect(renderer, &grid_cursor_ghost);
        }

        // Draw board
        if (not_first_click && !quit) {
            gameboard->print_board(renderer, grid_background, grid_cursor_color, grid_cell_size, font, false);
        }

        // Delay, so that it won't crash
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / FPS);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;

    return 0;
}