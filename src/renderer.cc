#include <exception>

#include "renderer_exceptions.h"

#include "renderer.h"

const static int DEFAULT_RENDERING_DRIVER = -1;

Renderer::Renderer(
    const uint32_t width,
    const uint32_t height,
    SDL_Window* window,
    SDL_Renderer* renderer
) :
    m_width(width),
    m_height(height),
    m_window(window),
    m_renderer(renderer)
{} 

Renderer::~Renderer() {
    SDL_DestroyRenderer(m_renderer); 
    SDL_DestroyWindow(m_window);
}

void Renderer::fill_pixel(const uint32_t x, const uint32_t y,
        const Color& color) {
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.alpha); 
    SDL_RenderDrawPoint(m_renderer, x, m_width - y);
}

void Renderer::display() {
    SDL_RenderPresent(m_renderer);
}

Renderer create_renderer(
    const std::string& title,
    const uint32_t width,
    const uint32_t height
) {
    SDL_Window* window(SDL_CreateWindow(
                title.data(),
                SDL_WINDOWPOS_UNDEFINED, // as of now i dont care about initial
                SDL_WINDOWPOS_UNDEFINED, // position of the window.
                width,
                height,
                SDL_WINDOW_SHOWN
                )
            );

    if (!window) {
        throw RendererException(std::move("SDL_CreateWindow failed"));
    }

    SDL_Renderer* renderer(SDL_CreateRenderer(
                window,
                DEFAULT_RENDERING_DRIVER,
                SDL_RENDERER_ACCELERATED
                )
            );

    if (!renderer) {
        SDL_DestroyWindow(window);
        throw RendererException(std::move("SDL_CreateRenderer failed"));
    }

    return Renderer(width, height, window, renderer);
}

