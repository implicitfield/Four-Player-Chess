#include "GUI.h"
#include "SDL.h"
#include "library.h"
#include <array>
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
    bool promotion_dialog_active = false;
    FPC::Point square {};
    std::array<SDL_Rect, 4> promotion_selection {};

    while (!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.motion.state & SDL_BUTTON_LMASK) {
                    bool update_square_value = true;
                    // On macOS with retina displays, the position returned from event.motion.* differs from the
                    // internal coordinates used by SDL, so we use scaled coordinates to interface with the library.
                    int x_scaled = event.motion.x * (pixel_width / GUI::window_width);
                    int y_scaled = event.motion.y * (pixel_height / GUI::window_height);
                    auto square_or_empty = GUI::get_square_from_pixel({x_scaled, y_scaled});

                    const bool square_exists = square_or_empty.has_value();
                    const bool point_is_owned_by_player = game.point_is_of_color(square_or_empty.value(), game.get_current_player());

                    if (!square_exists || (!draw_positions && !point_is_owned_by_player && !promotion_dialog_active)) {
                        draw_positions = false;
                        break;
                    }

                    if (promotion_dialog_active) {
                        update_square_value = false;
                        const int cell_size = GUI::get_cell_size();
                        int cell_x = square_or_empty.value().x * cell_size + (GUI::window_width / 4) + 12;
                        int cell_y = square_or_empty.value().y * cell_size + 12;
                        SDL_Rect current_square {cell_x, cell_y, cell_size, cell_size};
                        auto is_equal = [](SDL_Rect first, SDL_Rect second) -> bool {
                            if (first.w == second.w && first.h == second.h && first.x == second.x && first.y == second.y)
                                return true;
                            return false;
                        };
                        for (int i = 0; i < 4; ++i) {
                            if (is_equal(promotion_selection[i], current_square)) {
                                game.get_board()[square.x][square.y].piece = static_cast<FPC::Piece>(i);
                                promotion_dialog_active = false;
                                game.advance_turn();
                            }
                        }
                    } else if (draw_positions) {
                        if (game.move_piece_to(square, square_or_empty.value())) {
                            if (game.may_promote(square_or_empty.value(), game.get_current_player()))
                                promotion_dialog_active = true;
                            else
                                game.advance_turn();
                        }
                        draw_positions = false;
                    } else
                        draw_positions = true;
                    if (update_square_value)
                        square = square_or_empty.value();
                }

                break;
        }
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        painter.draw_board();
        if (promotion_dialog_active)
            promotion_selection = painter.draw_promotion_dialog(square, game.get_current_player());
        if (draw_positions) {
            if (!painter.draw_valid_positions(square, game.get_current_player()))
                draw_positions = false;
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
