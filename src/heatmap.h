#pragma once

#include <string>
#include <algorithm>
#include <iostream>

#include "renderer.h"

template<class HeatmapFunction>
struct Heatmap {
public:

    using ResultType = typename HeatmapFunction::result_type;

    /**
     * range_changing determines if for all t the heatmap needs to calculate
     * the function's new min and max or if it is static for all t.
     */
    Heatmap(
        const std::string& title,
        uint32_t width,
        uint32_t height,
        HeatmapFunction function,
        ResultType min,
        ResultType max,
        bool range_changing
    ) :
        m_width(width),
        m_height(height),
        m_function(std::move(function)),
        m_min_function_value(min),
        m_max_function_value(max),
        m_renderer(create_renderer(title, width, height)),
        m_range_changing(range_changing)
    {}

    void display(uint32_t t) {
        if (m_range_changing) {
            set_min_and_max(t);
        }
        m_renderer.lock();
        for (uint32_t x = 0; x < m_width; ++x) {
            for (uint32_t y = 0; y < m_height; ++y) {
                Color color = color_value(
                        m_function(x, y, t));
                        m_renderer.fill_pixels<1>(x, y, &color);
            }
        }
        m_renderer.unlock();
        m_renderer.display();
    }

private:

    static const uint8_t MAX_COLOR = 255;
    static const uint8_t NO_COLOR = 0;

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
    bool m_range_changing;
};

