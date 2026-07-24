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

    static float GetRandomFNormalized()
    {
        const float random = GetRandomFValue(-1, 1);
        return (random / abs(random));
    }

    static Vector2 VectorToroidalMod(Vector2 vector, const Vector2 worldSpace)
    {
        if (vector.x > (worldSpace.x / 2.0f))
        {
            vector.x -= worldSpace.x;
        }
        else if (vector.x < -(worldSpace.x / 2.0f))
        {
            vector.x += worldSpace.x;
        }
        if (vector.y > (worldSpace.y / 2.0f))
        {
            vector.y -= worldSpace.y;
        }
        else if (vector.y < -(worldSpace.y / 2.0f))
        {
            vector.y += worldSpace.y;
        }
        return vector;
    }

    static Vector2 VectorMod(const Vector2 vec, const Vector2 vec2)
    {
        const Vector2 mod = {.x = vec.x - (vec2.x * floorf(vec.x / vec2.x)), .y = vec.y - (vec2.y * floorf(vec.y / vec2.y))};
        return mod;
    }

    static void HandleSimulationControl(Camera2D& camera, bool& isPaused, bool& hideStars)
    {
        camera.zoom = Clamp(expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f)), 0.1f, 3.5f);
        if (IsKeyPressed(KEY_SPACE))
        {
            isPaused = !isPaused;
        }
        if (IsKeyPressed(KEY_H))
        {
            hideStars = !hideStars;
        }
    }
};
