#pragma once

struct Color {
public:

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha) :
        r(r), g(g), b(b), alpha(alpha)
    {}

    uint32_t encode_color_rgba8888() const {
        uint32_t encoding = r;
        encoding <<= 8;
        encoding |= g;
        encoding <<= 8;
        encoding |= b;
        encoding <<= 8;
        encoding |= alpha;
        return encoding;
    }

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t alpha;
};

