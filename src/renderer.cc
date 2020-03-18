#include <exception>

#include "renderer_exceptions.h"

#include "renderer.h"

const static int DEFAULT_RENDERING_DRIVER = -1;

Renderer::Renderer(
    const uint32_t width,
    const uint32_t height,
    SDL_Window* window,
    SDL_Renderer* renderer,
    SDL_Texture* texture
) :
    m_width(width),
    m_height(height),
    m_window(window),
    m_renderer(renderer),
    m_texture(texture),
    m_pixels(nullptr)
{} 

Renderer::~Renderer() {
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer); 
    SDL_DestroyWindow(m_window);
}

void Renderer::lock() {
    int pitch;
    int result = SDL_LockTexture(
            m_texture,
            nullptr, // we lock the entire texture
            &m_pixels,
            &pitch /* pitch will be stored here. as of now - it is height,
                     therefore it's not used */
        );
    if (result < 0 || !m_pixels) {
        throw RendererException(std::move("SDL_LockTexture failed"));
    }
}

void Renderer::unlock() {
    if (!m_pixels) {
        throw RendererException(std::move("SDL_UnlockTexture failed because lock was not invoked"));
    }
    SDL_UnlockTexture(m_texture);
    m_pixels = nullptr;
}

void Renderer::fill_pixel(uint32_t x, uint32_t y,
        const Color& color) {
    /*
     * Transforming from SDL coordinate system (x left to right, y up to down)
     * to the normal one (x left to right, y down to up)
     */
    y = m_height - y;
    static_cast<uint32_t*>(m_pixels)[y * m_height + x] = encode_color_rgba8888(color);
}

void Renderer::display() {
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
    SDL_RenderClear(m_renderer);
}

uint32_t Renderer::encode_color_rgba8888(const Color& color) const {
    uint32_t encoding = color.r;
    encoding <<= 8;
    encoding |= color.g;
    encoding <<= 8;
    encoding |= color.b;
    encoding <<= 8;
    encoding |= color.alpha;
    return encoding;
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
    
    SDL_Texture* texture (SDL_CreateTexture(
                renderer, 
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_STREAMING,
                width,
                height
                )
            );
   
    if (!texture) {
        SDL_DestroyRenderer(renderer); 
        SDL_DestroyWindow(window);
        throw RendererException(std::move("SDL_CreateTexture failed"));
    }

    return Renderer(width, height, window, renderer, texture);

}

