#pragma once
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include "Game.h"

class PerlinNoise {
private:
    vector<int> m_vecSeed;
public:
    PerlinNoise() : PerlinNoise(-1) {}
    PerlinNoise(unsigned int seed) {
        std::random_device rd;
        std::mt19937 gen(seed == -1? rd() : seed);
        std::uniform_int_distribution<int> dis(10000000, 100000000);
        m_vecSeed.clear();

        for (int i = 0; i < 3; i++)
        {
            m_vecSeed.push_back(dis(gen));
        }
    }
    float perlin(float x, float y) {
        int x0 = (int)x;
        int y0 = (int)y;
        int x1 = x0 + 1;
        int y1 = y0 + 1;

        float sx = x - (float)x0;
        float sy = y - (float)y0;

        float n0 = dotGridGradient(x0, y0, x, y);
        float n1 = dotGridGradient(x1, y0, x, y);
        float ix0 = interpolate(n0, n1, sx);

        n0 = dotGridGradient(x0, y1, x, y);
        n1 = dotGridGradient(x1, y1, x, y);
        float ix1 = interpolate(n0, n1, sx);

        float value = interpolate(ix0, ix1, sy);

        return value;
    }
private:
    POSITION randomGradient(int ix, int iy) {
        const unsigned w = 8 * sizeof(unsigned);
        const unsigned s = w / 2;
        unsigned a = ix, b = iy;
        a *= m_vecSeed[0];

        b ^= a << s | a >> w - s;
        b *= m_vecSeed[1];

        a ^= b << s | b >> w - s;
        a *= m_vecSeed[2];
        float random = a * (3.14159265 / ~(~0u >> 1));

        POSITION v;
        v.x = sin(random);
        v.y = cos(random);

        return v;
    }
    float dotGridGradient(int ix, int iy, float x, float y) {
        POSITION gradient = randomGradient(ix, iy);

        float dx = x - (float)ix;
        float dy = y - (float)iy;

        return (dx * gradient.x + dy * gradient.y);
    }
    float interpolate(float a0, float a1, float w)
    {
        return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
    }

};