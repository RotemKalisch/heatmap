#pragma once

#include <string>
#include <functional>
#include <algorithm>
#include <iostream>

#include "renderer.h"

static const uint8_t MAX_COLOR = 255;
static const uint8_t NO_COLOR = 0;

template<class T, class Int_t, class Time_t>
struct Heatmap {
public:
    Heatmap(const std::string& title, Int_t width, Int_t height,
            std::function<T(Int_t, Int_t, Int_t)> function) :
    m_width(width),
    m_height(height),
    m_function(std::move(function)),
    m_min_function_value(m_function(0, 0, 0)),
    m_max_function_value(m_function(0, 0, 0)),
    m_renderer(create_renderer(title, width, height))
{}

    void display(Time_t t, Int_t resolution = 1) {
        set_min_and_max(t);
        m_renderer.lock();
        for (Int_t outer_x = 0; outer_x < m_width / resolution; ++outer_x) {
            for (Int_t outer_y = 0; outer_y < m_height / resolution; ++outer_y) {
                Color color = color_value(
                        m_function(outer_x, outer_y, t));
                for (Int_t inner_x = 0; inner_x < resolution; ++inner_x) {
                    for (Int_t inner_y = 0; inner_y < resolution; ++inner_y) {
                        Int_t x = outer_x * resolution + inner_x;
                        Int_t y = outer_y * resolution + inner_y;
                        m_renderer.fill_pixel(x, y, color);
                    }
                }
            }
        }
        m_renderer.unlock();
        m_renderer.display();
    }

private:
    void set_min_and_max(Time_t t) {
        for (Int_t i = 0; i < m_width; ++i) {
            for (Int_t j = 0; j < m_height; ++j) {
                T value = m_function(i, j, t);
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

    Int_t m_width;
    Int_t m_height;
    std::function<T(Int_t, Int_t, Time_t)> m_function;
    T m_min_function_value;
    T m_max_function_value;
    Renderer m_renderer;
};

