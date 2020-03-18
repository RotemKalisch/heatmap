#pragma once

#include <string>
#include <functional>

#include "renderer.h"

struct Heatmap {
public:
    Heatmap(const std::string& title, uint32_t width, uint32_t height,
            std::function<int32_t(uint32_t, uint32_t)> function);

    void display();

private:
    void set_min_and_max();
    Color color_value(int32_t value);

    uint32_t m_width;
    uint32_t m_height;
    std::function<int32_t(uint32_t, uint32_t)> m_function;
    int32_t m_min_function_value;
    int32_t m_max_function_value;
    Renderer m_renderer;
};

