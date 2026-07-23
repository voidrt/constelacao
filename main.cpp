#include <algorithm>
#include "constants.hpp"
#include "utils.hpp"
#include <array>
#include <vector>


using Grid = std::array<std::array<std::vector<int>, kColumns>, kRows>;

struct Particle
{
    Vector2 position;
    Vector2 velocity;
    int id;

    void Move()
    {
        const Vector2 centripetal = Utils::VectorToroidalMod(kWorldCenter - this->position, {.x = kWorldWidth, .y = kWorldHeight});
        const float distanceToCenterSqr = Vector2LengthSqr(centripetal);

        if (distanceToCenterSqr > 1.0f)
        {
            this->velocity += Vector2Scale(Vector2Normalize(centripetal), (kGravityConstant * kBHoleMass) / distanceToCenterSqr) * kDeltaTime;
        }

        this->position += (this->velocity * kDeltaTime);
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

                for (const auto particleId : grid[gridRow][gridColumn])
                {
                    if (this->id == particleId) { continue; }

                    const float distanceSqr = Vector2DistanceSqr(this->position, particles[particleId].position);

                    if (distanceSqr < kConstellationRadiusSqr)
                    {
                        DrawLineV(this->position, particles[particleId].position, {218, 218, 218, 5});
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
    Vector2 position{};
    Vector2 velocity{};
    int gridPositionX{};
    int gridPositionY{};

    for (size_t i{}; i < kParticleCount; ++i)
    {
        float random1 = Utils::GetRandomFValue(0.0f, 1.0f);
        float random2 = Utils::GetRandomFValue(0.0f, 1.0f);

        float minRadiusSqr = (kBHoleRadius + 30.0f) * (kBHoleRadius + 30.0f);
        float maxRadiusSqr = (kWorldCenter.x - 55.0f) * (kWorldCenter.x - 55.0f);

        float positionR = std::sqrt(minRadiusSqr + (random1 * (maxRadiusSqr - minRadiusSqr)));
        float positionTheta = 2 * PI * random2;


        position = {.x = kWorldCenter.x + (positionR * cosf(positionTheta)), .y = kWorldCenter.y + (positionR * sinf(positionTheta))};
        particles[i].position = position;

        const float signMultiplier = Utils::GetRandomFNormalized();
        const Vector2 centripetal = Utils::VectorToroidalMod(kWorldCenter - position, {.x = kWorldWidth, .y = kWorldHeight});
        const Vector2 tangent = {.x = -centripetal.y, .y = centripetal.x};
        const float distance = Vector2Length(tangent);
        float random3 = Utils::GetRandomFValue(0.65f, 1.4f);

        velocity = Vector2Scale(Vector2Normalize(tangent), signMultiplier * random3 * sqrt((kGravityConstant * kBHoleMass) / distance));


        particles[i].velocity = velocity;
        particles[i].id = static_cast<int>(i);

        gridPositionX = std::floor(position.x / 25.0f);
        gridPositionY = std::floor(position.y / 25.0f);
        gridPositionY = std::clamp(gridPositionY, 0, kRows - 1);
        gridPositionX = std::clamp(gridPositionX, 0, kColumns - 1);

        grid[gridPositionY][gridPositionX].push_back(static_cast<int>(i));
    }

    return 0;
}

void RunSimulation()
{
    for (size_t i{}; i < kRows; ++i)
    {
        for (size_t j{}; j < kColumns; ++j)
        {
            grid[i][j].clear();
        }
    }

    for (size_t i{}; i < kParticleCount; ++i)
    {
        particles[i].Move();
    }

    for (size_t i{}; i < kParticleCount; ++i)
    {
        int gridPositionY = std::floor(particles[i].position.y / 25.0f);
        int gridPositionX = std::floor(particles[i].position.x / 25.0f);
        gridPositionY = std::clamp(gridPositionY, 0, kRows - 1);
        gridPositionX = std::clamp(gridPositionX, 0, kColumns - 1);

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
    Camera2D camera{};
    camera.offset = {.x = kWindowWidth / 2.0f, .y = kWindowHeight / 2.0f};
    camera.target = {.x = kWorldWidth / 2.0f, .y = kWorldHeight / 2.0f};
    camera.zoom = 1.0f;

    AllocateVectorSpace();

    while (!WindowShouldClose())
    {
        Utils::HandleCameraControl(camera);
        if (IsKeyPressed(KEY_SPACE))
        {
            isPaused = !isPaused;
        }
        if (IsKeyPressed(KEY_R))
        {
            InitParticles();
        }

        BeginDrawing();
        ClearBackground({.r = 24, .g = 24, .b = 22, .a = 255});

        BeginMode2D(camera);

        DrawRectangleV({kWorldCenter.x - (kBHoleRadius / 2.0f), kWorldCenter.y - (kBHoleRadius / 2.0f)}, {kBHoleRadius, kBHoleRadius}, RAYWHITE);
        DrawRectangleLines(0.0f, 0.0f, kWorldWidth, kWorldHeight, DARKGRAY);

        for (const auto particle : particles)
        {
            const Rectangle star = {particle.position.x, particle.position.y, kStarSize, kStarSize};
            constexpr Vector2 starMiddle = {kStarSize / 2.0f, kStarSize / 2.0f};
            const float headingAngle = atan2f(particle.velocity.y, particle.velocity.x) * RAD2DEG;

            DrawRectanglePro(star, starMiddle, headingAngle, {218, 218, 218, 255});
        }

        if (isPaused)
        {
            for (int i{}; i < 6; ++i)
            {
                RunSimulation();
            }
        }


        EndMode2D();
        DrawFPS(15, 15);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
