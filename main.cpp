#include <iostream>
#include "raylib.h"
#include <climits>
#include "constants.hpp"
#include <iostream>
#include <array>

struct Particle
{
    Vector2 position;
    Vector2 velocity;
};

Particle particles[1000];

float GetRandomFValue(float min, float max)
{
    return min + ((max - min) * static_cast<float>((GetRandomValue(1, INT_MAX)) / static_cast<float>(INT_MAX)));
}

int InitParticles()
{
    float positionX{};
    float positionY{};
    float velocityX{};
    float velocityY{};

    for (size_t i{}; i < 1000; ++i)
    {
        positionX = (GetRandomFValue(0, kWindowWidth));
        positionY = (GetRandomFValue(0, kWindowHeight));
        particles[i].position = {positionX,positionY};
    }

    return 0;
}

int main()
{
    InitWindow(kWindowWidth, kWindowHeight, "Constellations");

    InitParticles();

    Camera2D camera = { 0 };
    camera.target = {kWindowWidth/2.0f,kWindowHeight/2.0f};

    camera.offset = {kWindowWidth/2.0f,kWindowHeight/2.0f};
    camera.zoom=.8f;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
        for (const Particle particle : particles)
        {
            DrawPixelV(particle.position, RAYWHITE);
        }
        EndDrawing();
        EndMode2D();
    }

    CloseWindow();

    return 0;
}
