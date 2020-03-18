#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "color.h"

struct Renderer {
public:
    Renderer(const uint32_t width, const uint32_t height, SDL_Window* window,
            SDL_Renderer* renderer, SDL_Texture* texture);

    /// This destructor is being called to clean up sdl environment
    ~Renderer();

    /// Call this function before calling fill_pixel.
    void lock();

    /// Call this function after changing all desired pixels with fill_pixel.
    void unlock();

    /// Call this function after lock and before unlock.
    void fill_pixel(uint32_t x, uint32_t y, const Color& color);

    /// Call this function after unlock.
    void display();

private:

    uint32_t encode_color_rgba8888(const Color& color) const;

    const uint32_t m_width;
    const uint32_t m_height;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;

    /**
     * The actual pixels used by SDL.
     * The format is RGBA8888 - 32 bit per pixel.
     */
    void* m_pixels;
};

Renderer create_renderer(const std::string& title, const uint32_t width,
        const uint32_t height);

