#include <iostream>
#include "raylib.h"
#include <raymath.h>
#include <climits>
#include "constants.hpp"
#include <array>

float GetRandomFValue(const float min, const float max)
{
    return min + ((max - min) * static_cast<float>((GetRandomValue(1, INT_MAX)) / static_cast<float>(INT_MAX))); // NOLINT(*-narrowing-conversions)
}

Vector2 VectorMod(const Vector2 vec, const Vector2 vec2)
{
    const Vector2 mod = {vec.x - (vec2.x * floorf(vec.x / vec2.x)), vec.y - (vec2.y * floorf(vec.y / vec2.y))};
    return mod;
}

float GetRandomFNormalized()
{
    const float random = GetRandomFValue(-1, 1);
    return random / std::fabsf(random);
}

struct Particle
{
    Vector2 position;
    Vector2 velocity;
    int id;

    void Move()
    {
        constexpr Vector2 center = {kWindowWidth / 2.0f, kWindowHeight / 2.0f};
        const Vector2 centripetal = center - this->position;
        const float distanceToCenterSqr = Vector2DistanceSqr(center, this->position);

        if (distanceToCenterSqr > (5.0f))
        {
            this->velocity += (Vector2Normalize(centripetal) * kCentripetalForce) / (distanceToCenterSqr + kBHoleRadius);
        }

        this->position += (this->velocity * kDeltaTime);

        this->position = VectorMod(this->position, {kWindowWidth, kWindowHeight});
    }

    void MakeConstellation(const std::array<Particle, kParticleCount>& particles) const
    {
        for (size_t i{}; i < kParticleCount; ++i)
        {
            if (this->id == i)
            {
                continue;
            }

            Particle otherParticle = particles[i];
            float distanceSqr = Vector2DistanceSqr(this->position, otherParticle.position);

            if (distanceSqr < kLineRadius)
            {
                DrawLineEx(this->position, otherParticle.position, 1, RAYWHITE);
            }
        }
    }
};

std::array<Particle, kParticleCount> particles;


int InitParticles()
{
    float positionX{};
    float positionY{};
    float velocityX{};
    float velocityY{};

    for (size_t i{}; i < kParticleCount; ++i)
    {
        positionX = (GetRandomFValue(0, kWindowWidth));
        positionY = (GetRandomFValue(0, kWindowHeight));

        velocityX = GetRandomFValue(-kParticleSpeed, kParticleSpeed);
        velocityY = GetRandomFValue(-kParticleSpeed, kParticleSpeed);

        particles[i].position = {positionX, positionY};
        particles[i].velocity = {velocityX, velocityY};
        particles[i].id = static_cast<int>(i);
    }

    return 0;
}

void RunSimulation()
{
    for (size_t i{}; i < kParticleCount; ++i)
    {
        particles[i].MakeConstellation(particles);
    }

    if (IsKeyPressed(KEY_R))
    {
        InitParticles();
    }

    for (size_t i{}; i < kParticleCount; ++i)
    {
        particles[i].Move();
    }
}

int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(kWindowWidth, kWindowHeight, "Constellations");
    SetTargetFPS(60);
    InitParticles();


    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawFPS(15, 15);

        DrawCircleV({kWindowWidth / 2.0f, kWindowHeight / 2.0f}, kBHoleRadius, {51, 51, 37, 255});

        for (const auto particle : particles)
        {
            Rectangle star = {particle.position.x, particle.position.y, kStarSize, kStarSize};
            DrawRectanglePro(star, {kStarSize / 2.0f,kStarSize / 2.0f}, atan2f(particle.velocity.y, particle.velocity.x) * RAD2DEG, RAYWHITE);
        }
        RunSimulation();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
