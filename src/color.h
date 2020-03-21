#pragma once

struct Color {
public:

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha) :
        alpha(alpha), b(b), g(g), r(r)
    {}

    operator uint32_t () {
        return *reinterpret_cast<uint32_t*>(this);
    }

    operator uint32_t () const {
        return *reinterpret_cast<const uint32_t*>(this);
    }

    uint8_t alpha;
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

