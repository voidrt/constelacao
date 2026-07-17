#pragma once

#include <raylib.h>
#include <climits>
#include <raymath.h>

struct Utils
{
    static float GetRandomFValue(const float min, const float max)
    {
        return min + ((max - min) * static_cast<float>((GetRandomValue(1, INT_MAX)) / static_cast<float>(INT_MAX))); // NOLINT(*-narrowing-conversions)
    }

    static Vector2 VectorMod(const Vector2 vec, const Vector2 vec2)
    {
        const Vector2 mod = {vec.x - (vec2.x * floorf(vec.x / vec2.x)), vec.y - (vec2.y * floorf(vec.y / vec2.y))};
        return mod;
    }
};