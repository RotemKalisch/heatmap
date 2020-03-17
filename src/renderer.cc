#include <exception>

#include "renderer.h"

const static int DEFAULT_RENDERING_DRIVER = -1;

Renderer::Renderer(
    std::string title,
    const uint32_t width,
    const uint32_t height
) :
    m_width(width),
    m_height(height)
{
    m_window = SDL_CreateWindow(
            title.data(),
            SDL_WINDOWPOS_UNDEFINED, // as of now i dont care about initial
            SDL_WINDOWPOS_UNDEFINED, // position of the window.
            width,
            height,
            SDL_WINDOW_SHOWN
            );

    if (!m_window) {
        throw std::exception();
    }

    m_renderer = SDL_CreateRenderer(
                m_window,
                DEFAULT_RENDERING_DRIVER,
                SDL_RENDERER_ACCELERATED);

    if (!m_renderer) {
        SDL_DestroyWindow(m_window);
        throw std::exception();
    }
} 

Renderer::~Renderer() {
    SDL_DestroyRenderer(m_renderer); 
    SDL_DestroyWindow(m_window);
}

void Renderer::fill_pixel(const uint32_t x, const uint32_t y,
        const Color& color) {
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a); 
    SDL_RenderDrawPoint(m_renderer, x, m_width - y);
}

void Renderer::display() {
    SDL_RenderPresent(m_renderer);
}

