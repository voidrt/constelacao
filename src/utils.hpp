#pragma once

#include <raylib.h>
#include <climits>
#include <raymath.h>

#include "constants.hpp"

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

    static void HandleSimulationControl(Camera2D& camera, bool& isPaused, bool& hideStars, bool& constellationActive)
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
        if (IsKeyPressed(KEY_R))
        {
            constellationActive = !constellationActive;
        }
        if (IsKeyDown(KEY_W))
        {
            camera.target.y -= 5.0f;
        }
        if (IsKeyDown(KEY_A))
        {
            camera.target.x -= 5.0f;
        }
        if (IsKeyDown(KEY_D))
        {
            camera.target.x += 5.0f;
        }
        if (IsKeyDown(KEY_S))
        {
            camera.target.y += 5.0f;
        }

    }

    static int OpacityScaling(const float distanceSqr)
    {
        int opacity = kMaximumConstellationOpacity;
        constexpr float coefficient = (kMinimumConstellationOpacity - kMaximumConstellationOpacity) / (kConstellationRadius * kConstellationRadius);
        opacity += static_cast<int>(distanceSqr * coefficient);
        return opacity;
    }
};
