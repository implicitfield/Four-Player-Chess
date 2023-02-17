#include "GUI.h"
#include "SDL.h"
#include "library.h"
#include <iostream>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized!\nSDL_Error: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("FPC",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        GUI::window_width, GUI::window_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    if (!window) {
        std::cout << "Window could not be created!\nSDL_Error: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cout << "Renderer could not be created!\nSDL_Error: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool quit = false;

    FPC::GameState game;
    GUI::Painter painter(game, renderer);
    painter.draw_board();

    int pixel_width = 0;
    int pixel_height = 0;
    SDL_GL_GetDrawableSize(window, &pixel_width, &pixel_height);
    bool draw_positions = false;
    FPC::Point square {};

    while (!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.motion.state & SDL_BUTTON_LMASK) {
                    // On macOS with retina displays, the position returned from event.motion.* is different compared to
                    // the internal coordinates used by SDL, so we use scaled coordinates to interface with the library.
                    int x_scaled = event.motion.x * (pixel_width / GUI::window_width);
                    int y_scaled = event.motion.y * (pixel_height / GUI::window_height);
                    std::cout << "X: " << x_scaled << " Y: " << y_scaled << '\n';
                    auto square_or_empty = GUI::get_square_from_pixel({x_scaled, y_scaled});
                    if (!square_or_empty.has_value() || (!game.point_is_of_color(square_or_empty.value(), game.get_current_player()) && !draw_positions)) {
                        draw_positions = false;
                        break;
                    }
                    if (draw_positions) {
                        if(game.move_piece_to(square, square_or_empty.value()))
                            game.advance_turn();
                        draw_positions = false;
                    } else
                        draw_positions = true;
                    square = square_or_empty.value();
                    std::cout << "X: " << square.x << " Y: " << square.y << '\n';
                }

                break;
        }
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        painter.draw_board();
        if (draw_positions)
            painter.draw_valid_positions(square, game.get_current_player());
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
