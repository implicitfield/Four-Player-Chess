#include "SDL.h"
#include "GUI.h"
#include "library.h"
#include <iostream>

int main() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized!\nSDL_Error: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("FPC",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          GUI::window_width, GUI::window_height,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    if(!window) {
        std::cout << "Window could not be created!\nSDL_Error: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(!renderer) {
        std::cout << "Renderer could not be created!\nSDL_Error: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

/*
    SDL_Rect square_rect;
    square_rect.w = 50;
    square_rect.h = 50;
    square_rect.x = window_width - (square_rect.w / 2);
    square_rect.y = window_height - (square_rect.h / 2);
*/

    bool quit = false;
    
    FPC::GameState game;
    //FPC::get_piece_name(game, 3, 0);
    
    
    while(!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT)
            quit = true;
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        GUI::Painter painter(game, renderer);
        painter.draw_board();
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
