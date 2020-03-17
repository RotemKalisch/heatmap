#include <exception>

#include "heatmap.h"

struct Function {
public:
    static int32_t at(uint32_t x, uint32_t y) {
        return x * x - y * y;
    }
};

int logic() {
    const uint32_t width = 500;
    const uint32_t height = 500;

    Heatmap<Function> heatmap("f(x, y) = x^2 - y^2", width, height);

    heatmap.display();
            
    bool quit = false;
    SDL_Event e;

    //While application is running
    while(!quit) {
        //Handle events on queue
        while(SDL_PollEvent(&e) != 0) {
        //User requests quit
            if(e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

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

