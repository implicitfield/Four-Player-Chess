#include "GUI.h"
#include "SDL.h"
#include "SDL_image.h"
#include "library.h"
#include <array>
#include <iostream>

static int resizingEventWatcher(void* data, SDL_Event* event) {
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        GUI::Painter* painter = reinterpret_cast<GUI::Painter*>(data);
        painter->refresh_surface();
        int height = 0;
        int width = 0;
        SDL_GetWindowSize(SDL_GetWindowFromID(event->window.windowID), &width, &height);
        painter->update_window_size(height, width);
        painter->draw_board();
        SDL_UpdateWindowSurface(SDL_GetWindowFromID(event->window.windowID));
    }
    return 0;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized!\nSDL_Error: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("FPC",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1024, 768,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

    if (!window) {
        std::cout << "Window could not be created!\nSDL_Error: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    bool quit = false;

    FPC::GameState game;
    GUI::Painter painter(game, window, 768, 1024);
    SDL_AddEventWatch(resizingEventWatcher, &painter);
    painter.draw_board();

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
                    auto square_or_empty = painter.get_square_from_pixel({event.motion.x, event.motion.y});

                    if (!square_or_empty.has_value() || (!draw_positions && !game.point_is_of_color(square_or_empty.value(), game.get_current_player()) && !promotion_dialog_active)) {
                        draw_positions = false;
                        break;
                    }

                    if (promotion_dialog_active) {
                        update_square_value = false;
                        const int cell_size = painter.get_cell_size();
                        int cell_x = square_or_empty.value().x * cell_size + (painter.get_window_width() - 14 * cell_size) / 2;
                        int cell_y = square_or_empty.value().y * cell_size + (painter.get_window_height() - 14 * cell_size) / 2;
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
                        if (game.move_piece_to(square, square_or_empty.value(), true)) {
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
        painter.draw_board();
        if (promotion_dialog_active)
            promotion_selection = painter.draw_promotion_dialog(square, game.get_current_player());
        if (draw_positions) {
            if (!painter.draw_valid_positions(square, game.get_current_player()))
                draw_positions = false;
        }
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
