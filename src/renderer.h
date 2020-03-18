#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "color.h"

struct Renderer {
public:
    Renderer(const uint32_t width, const uint32_t height, SDL_Window* window,
            SDL_Renderer* renderer);

    /// This destructor is being called to clean up sdl environment
    ~Renderer();

    void fill_pixel(const uint32_t x, const uint32_t y, const Color& color);

    void display();

private:
    const uint32_t m_width;
    const uint32_t m_height;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
};

Renderer create_renderer(const std::string& title, const uint32_t width,
        const uint32_t height);

