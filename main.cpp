#include <algorithm>

#include "constants.hpp"
#include "utils.hpp"
#include <array>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>


using Grid = std::array<std::array<std::vector<int>, kColumns>, kRows>;

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

        this->position = Utils::VectorMod(this->position, {kWindowWidth, kWindowHeight});
    }

    void MakeConstellation(const Grid& grid, const std::array<Particle, kParticleCount>& particles) const
    {
        int gridPositionY = std::floor(this->position.y / 25.0f);
        int gridPositionX = std::floor(this->position.x / 25.0f);


        for (int i = -1; i <= 1; ++i)
        {
            for (int j = -1; j <= 1; ++j)
            {
                int gridRow = gridPositionY + j;
                int gridColumn = gridPositionX + i;
                gridRow = std::clamp(gridRow, 0, kRows - 1);
                gridColumn = std::clamp(gridColumn, 0, kColumns - 1);

                if (gridColumn < 0 || gridColumn > 21) std::cout << gridColumn << std::endl;
                if (gridRow < 0 || gridRow > 27) std::cout << gridRow << std::endl;

                for (const auto particleId : grid[gridRow][gridColumn])
                {
                    if (this->id == particleId) { continue; }

                    const float distanceSqr = Vector2DistanceSqr(this->position, particles[particleId].position);

                    if (distanceSqr < kConstellationRadiusSqr && distanceSqr > 1)
                    {
                        DrawLineEx(this->position, particles[particleId].position, 1, RAYWHITE);
                    }
                }
            }
        }
    }
};

bool isPaused = false;
std::array<Particle, kParticleCount> particles;
Grid grid{};

void AllocateVectorSpace()
{
    for (size_t i{}; i < kRows; ++i)
    {
        for (size_t j{}; j < kColumns; ++j)
        {
            grid[i][j].reserve(kParticleCount / (kRows * kColumns));
        }
    }
}

int InitParticles()
{
    float positionX{};
    float positionY{};
    float velocityX{};
    float velocityY{};
    int gridPositionX{};
    int gridPositionY{};

    for (size_t i{}; i < kParticleCount; ++i)
    {
        positionX = (Utils::GetRandomFValue(0, kWindowWidth));
        positionY = (Utils::GetRandomFValue(0, kWindowHeight));

        velocityX = Utils::GetRandomFValue(-kParticleSpeed, kParticleSpeed);
        velocityY = Utils::GetRandomFValue(-kParticleSpeed, kParticleSpeed);

        particles[i].position = {positionX, positionY};
        particles[i].velocity = {velocityX, velocityY};
        particles[i].id = static_cast<int>(i);

        gridPositionX = std::floor(positionX / 25.0f);
        gridPositionY = std::floor(positionY / 25.0f);

        grid[gridPositionY][gridPositionX].push_back(static_cast<int>(i));
    }

    return 0;
}

void RunSimulation()
{
    if (IsKeyPressed(KEY_R))
    {
        InitParticles();
    }

    for (size_t i{}; i < kParticleCount; ++i)
    {
        particles[i].Move();
    }

    for (size_t i{}; i < kRows; ++i)
    {
        for (size_t j{}; j < kColumns; ++j)
        {
            grid[i][j].clear();
        }
    }
    for (size_t i{}; i < kParticleCount; ++i)
    {
        int gridPositionY = std::floor(particles[i].position.y / 25.0f);
        int gridPositionX = std::floor(particles[i].position.x / 25.0f);
        gridPositionY = std::clamp(gridPositionY, 0, kRows - 1);
        gridPositionX = std::clamp(gridPositionX, 0, kRows - 1);

        if (gridPositionY < 0 || gridPositionY > 27 || gridPositionX < 0 || gridPositionX > 21)
        {
            std::cout << gridPositionY << " " << gridPositionX << std::endl;
        }
        grid[gridPositionY][gridPositionX].push_back(static_cast<int>(i));
    }

    for (auto particle : particles) { particle.MakeConstellation(grid, particles); }
}

int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(kWindowWidth, kWindowHeight, "Constellations");
    SetTargetFPS(60);
    InitParticles();

    AllocateVectorSpace();

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            isPaused = !isPaused;
        }


        BeginDrawing();
        ClearBackground({8,8,10,255});


        for (const auto particle : particles)
        {
            const Rectangle star = {particle.position.x, particle.position.y, kStarSize, kStarSize};
            DrawRectanglePro(star, {kStarSize / 2.0f, kStarSize / 2.0f}, atan2f(particle.velocity.y, particle.velocity.x) * RAD2DEG, {152,152,152,255});
        }

        if (isPaused)
        {
            RunSimulation();
        }
        DrawFPS(15, 15);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
