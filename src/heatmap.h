#pragma once

#include <string>
#include <algorithm>
#include <iostream>

#include "renderer.h"

static const uint8_t MAX_COLOR = 255;
static const uint8_t NO_COLOR = 0;

template<class HeatmapFunction>
struct Heatmap {
public:

    using ResultType = typename HeatmapFunction::result_type;

    Heatmap(const std::string& title, uint32_t width, uint32_t height,
            HeatmapFunction function) :
        m_width(width),
        m_height(height),
        m_function(std::move(function)),
        m_min_function_value(m_function(0, 0, 0)),
        m_max_function_value(m_function(0, 0, 0)),
        m_renderer(create_renderer(title, width, height))
    {}

    void display(uint32_t t, uint32_t resolution = 1) {
        set_min_and_max(t);
        m_renderer.lock();
        for (uint32_t outer_x = 0; outer_x < m_width / resolution; ++outer_x) {
            for (uint32_t outer_y = 0; outer_y < m_width / resolution; ++outer_y) {
                Color color = color_value(
                        m_function(outer_x, outer_y, t));
                for (uint32_t inner_x = 0; inner_x < resolution; ++inner_x) {
                    for (uint32_t inner_y = 0; inner_y < resolution; ++inner_y) {
                       uint32_t x = outer_x * resolution + inner_x;
                        uint32_t y = outer_y * resolution + inner_y;
                        m_renderer.fill_pixel(x, y, color);
                    }
                }
            }
        }
        m_renderer.unlock();
        m_renderer.display();
    }

private:
    void set_min_and_max(uint32_t t) {
        for (uint32_t i = 0; i < m_width; ++i) {
            for (uint32_t j = 0; j < m_height; ++j) {
                ResultType value = m_function(i, j, t);
                m_min_function_value = std::min(m_min_function_value, value);
                m_max_function_value = std::max(m_max_function_value, value);
            }
        }
    }

    Color color_value(ResultType value) {
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
    HeatmapFunction m_function;
    ResultType m_min_function_value;
    ResultType m_max_function_value;
    Renderer m_renderer;
};

