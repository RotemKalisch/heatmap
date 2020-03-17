#pragma once

struct Color {
public:

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
        r(r), g(g), b(b), a(a)
    {}

    uint8_t r;
    uint8_t g;
    uint8_t b;
    /// This is the alpha value that determines opacity
    uint8_t a;
};

