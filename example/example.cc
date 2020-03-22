#include <chrono>
#include <exception>
#include <immintrin.h>

#include "heatmap.h"

/*
for (uint32_t i = 0; i < N; ++i) {
    return_values[i] = ((x+i)*y + 512*t) & 0xFFFF;
}
*/
struct Function {
public:
    using result_type = uint32_t;
    static constexpr uint32_t N = 8;

    void operator()(uint32_t x, uint32_t y, uint32_t t,
        result_type* return_values) {
        __m256i* return_values_256 = reinterpret_cast<__m256i*>(return_values);
        *return_values_256 = _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0); // i values
        __m256i x_vector = _mm256_set1_epi32(x); // x values
        __m256i y_vector = _mm256_set1_epi32(y); // y values
        *return_values_256 = _mm256_add_epi32(*return_values_256, x_vector); // (x+i)
        *return_values_256 = _mm256_mullo_epi32(*return_values_256, y_vector); // (x+i)*y
        __m256i t_vector = _mm256_set1_epi32(t); // t values
        t_vector = _mm256_slli_epi32(t_vector, 9); // t << 9 = 512 * t
        *return_values_256 = _mm256_add_epi32(*return_values_256, t_vector); // (x+i)*y + 512*t
        __m256i masks_vector = _mm256_set1_epi32(0xFFFF);
        *return_values_256 = _mm256_and_si256(*return_values_256, masks_vector);
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

