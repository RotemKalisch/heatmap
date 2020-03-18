#pragma once

#include <string>
#include <functional>
#include <algorithm>

#include "renderer.h"

static const uint8_t MAX_COLOR = 255;
static const uint8_t NO_COLOR = 0;

template<class T, class IntType>
struct Heatmap {
public:
    Heatmap(const std::string& title, IntType width, IntType height,
            std::function<T(IntType, IntType)> function) :
    m_width(width),
    m_height(height),
    m_function(std::move(function)),
    m_min_function_value(m_function(0, 0)),
    m_max_function_value(m_function(0, 0)),
    m_renderer(create_renderer(title, width, height))
{}

    void display(IntType resolution = 1) {
        set_min_and_max();
        for (IntType outer_x = 0; outer_x < m_width / resolution; ++outer_x) {
            for (IntType outer_y = 0; outer_y < m_height / resolution; ++outer_y) {
                Color color = color_value(
                        m_function(outer_x * resolution, outer_y * resolution));
                for (IntType inner_x = 0; inner_x < resolution; ++inner_x) {
                    for (IntType inner_y = 0; inner_y < resolution; ++inner_y) {
                        IntType x = outer_x * resolution + inner_x;
                        IntType y = outer_y * resolution + inner_y;
                        m_renderer.fill_pixel(x, y, color);
                    }
                }
            }
        }
        m_renderer.display();
    }

private:
    void set_min_and_max() {
        for (IntType i = 0; i < m_width; ++i) {
            for (IntType j = 0; j < m_height; ++j) {
                T value = m_function(i, j);
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

    IntType m_width;
    IntType m_height;
    std::function<T(IntType, IntType)> m_function;
    T m_min_function_value;
    T m_max_function_value;
    Renderer m_renderer;
};

