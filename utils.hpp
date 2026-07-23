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

    static float GetRandomFNormalized()
    {
        const float random = GetRandomFValue(-1, 1);
        return (random / abs(random));
    }

    static void HandleCameraControl(Camera2D& camera)
    {
        camera.zoom = Clamp(expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f)), 0.1f, 2.5f);
    }
};
