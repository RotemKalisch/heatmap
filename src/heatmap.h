#pragma once

#include <string>
#include <algorithm>

#include "renderer.h"

static const uint8_t MAX_COLOR = 255;
static const uint8_t NO_COLOR = 0;

template <class Function>
struct Heatmap {
public:
    Heatmap(const std::string& title, uint32_t width, uint32_t height) :
        m_width(width),
        m_height(height),
        m_min_function_value(Function::at(0, 0)),
        m_max_function_value(Function::at(0, 0)),
        m_renderer(create_renderer(title, width, height))
    {}

    void display() {
        set_min_and_max();
        for (uint32_t i = 0; i < m_width; ++i) {
            for (uint32_t j = 0; j < m_height; ++j) {
                m_renderer.fill_pixel(i, j, color_value(Function::at(i, j)));
            }
        }
        m_renderer.display();
    }

private:
    void set_min_and_max() {
        for (uint32_t i = 0; i < m_width; ++i) {
            for (uint32_t j = 0; j < m_height; ++j) {
                int32_t value = Function::at(i, j);
                m_min_function_value = std::min(m_min_function_value, value);
                m_max_function_value = std::max(m_max_function_value, value);
            }
        }
    }

    Color color_value(int32_t value) {
        return Color(
                MAX_COLOR,
                (MAX_COLOR * (m_max_function_value - value)) /
                    (m_max_function_value - m_min_function_value),
                NO_COLOR,
                MAX_COLOR
            );
    }

    uint32_t m_width;
    uint32_t m_height;
    int32_t m_min_function_value;
    int32_t m_max_function_value;
    Renderer m_renderer;
};

