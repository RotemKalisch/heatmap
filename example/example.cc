#include <chrono>
#include <exception>

#include "heatmap.h"

struct Function {
public:
    using result_type = int32_t;

    result_type operator()(uint32_t x, uint32_t y, uint32_t t) {
        return (x*y + 512*t) & 0x0000FFFF; 
    }
    
};

int logic() {
    constexpr uint32_t width = 1920 / 2;
    constexpr uint32_t height = 984 / 2;

    std::string title = "f(x, y, t) = x*y + (2^9)*t % 2^16";

    Heatmap heatmap(title, width, height, Function(), 0, 0x0000FFFF - 1, false);

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
        heatmap.display(t);
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

