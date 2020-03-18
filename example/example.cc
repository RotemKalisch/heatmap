#include <chrono>
#include <exception>

#include "heatmap.h"

int logic() {
    const uint32_t width = 500;
    const uint32_t height = 500;
    std::string title = "f(x, y, t) = x*y + 500*t % 25000";

    std::function<int32_t(uint32_t, uint32_t, uint32_t)> heatmap_function = 
        [](uint32_t x, uint32_t y, uint32_t t) {
            return (x*y + 500*t) % 25000; 
        };

    Heatmap heatmap(title, width, height, std::move(heatmap_function));

    bool quit = false;
    SDL_Event e;

    uint32_t t = 0;
    
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    //While application is running
    while(!quit) {
        //Handle events on queue
        while(SDL_PollEvent(&e) != 0) {
        //User requests quit
            if(e.type == SDL_QUIT) {
                quit = true;
            }
        }
        heatmap.display(t, 1);
        ++t;
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>
        (end - begin).count();
    std::cout << "FPS: " << 1000 * (double)t / ms << std::endl;

    return 0;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::exception();
    }
    int retval;
    try {
        retval = logic();
    } catch (std::exception& e) {
        retval = -1;
    }
    SDL_Quit();
    return retval;
}

