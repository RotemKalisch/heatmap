#pragma once

struct Color {
public:

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha) :
        r(r), g(g), b(b), alpha(alpha)
    {}

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t alpha;
};

