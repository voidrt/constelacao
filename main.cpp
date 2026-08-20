#include "src/grid/grid.hpp"
#include "src/stars/star.hpp"
#include "src/constants.hpp"
#include "src/utils.hpp"

static bool isPaused = true;
static bool constellationActive = true;
static bool hideStars = false;
static std::array<Star, kStarCount> stars;
static Grid grid{};
static Camera2D camera = {
    .offset = {.x = kWindowWidth / 2.0f, .y = kWindowHeight / 2.0f},
    .target = {.x = kWorldWidth / 2.0f, .y = kWorldHeight / 2.0f},
    .rotation = 0,
    .zoom = 1.0f
};

static void AllocateVectorSpace()
{
    for (size_t i{}; i < kRows; ++i)
    {
        for (size_t j{}; j < kColumns; ++j)
        {
            grid[i][j].reserve(kStarCount / (kRows * kColumns));
        }
    }
}

static Vector3 GenerateRandomPosition()
{
    const float radiusRandom = Utils::GetRandomFValue(0.0f, 1.0f);
    const float angleRandom = Utils::GetRandomFValue(0.0f, 1.0f);
    const float depth = Utils::GetRandomFValue(-kSpaceDepth, kSpaceDepth);
    const float positionR = (5 * kAnchorRadius) + (sqrt(radiusRandom) * ((kWorldWidth / 2.0f) - (20 * kAnchorRadius)));
    const float positionTheta = 2 * PI * angleRandom;


    const Vector3 positionGenerated = {
        .x = kWorldCenter.x + positionR * cosf(positionTheta), .y = kWorldCenter.y + positionR * sinf(positionTheta), .z = depth
    };

    return positionGenerated;
}

static Vector3 GenerateTangentVelocity(const Vector3& position)
{
    const float signMultiplier = Utils::GetRandomFNormalized();

    const Vector3 centripetal = (kWorldCenter - position);
    const Vector3 tangent = {.x = -centripetal.y, .y = centripetal.x, .z = centripetal.z};
    const float distance = Vector3Length(tangent);

    const Vector3 velocityTan = Vector3Normalize(tangent)* signMultiplier * 
            sqrt((kGravityConstant * kAnchorMass) / (distance));
    return velocityTan;
}

static int InitStars()
{
    int gridPositionX{};
    int gridPositionY{};

    for (size_t i{}; i < kStarCount; ++i)
    {
        const Vector3 position = GenerateRandomPosition();
        const Vector3 velocity = GenerateTangentVelocity(position);

        stars[i].id = static_cast<int>(i);
        stars[i].position = position;
        stars[i].velocity = velocity;

        gridPositionX = std::floor(position.x / kConstellationRadius);
        gridPositionY = std::floor(position.y / kConstellationRadius);
        gridPositionX = std::clamp(gridPositionX, 0, kColumns - 1);
        gridPositionY = std::clamp(gridPositionY, 0, kRows - 1);

        grid[gridPositionY][gridPositionX].push_back(static_cast<int>(i));
    }

    return 0;
}

static void InitSimulation()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);

    InitWindow(kWindowWidth, kWindowHeight, "Constellations");
    SetWindowState(FLAG_FULLSCREEN_MODE);

    InitStars();
    AllocateVectorSpace();
}

static void RunSimulation()
{
    for (Star& star : stars)
    {
        star.Move();
    }

    for (size_t i{}; i < kRows; ++i)
    {
        for (size_t j{}; j < kColumns; ++j)
        {
            grid[i][j].clear();
        }
    }
    for (const Star& star : stars)
    {
        int gridPositionY = std::floor(star.position.y / kConstellationRadius);
        int gridPositionX = std::floor(star.position.x / kConstellationRadius);

        grid[gridPositionY][gridPositionX].push_back(static_cast<int>(star.id));
    }

    if (constellationActive) { for (Star& star : stars) { star.MakeConstellation(grid, stars); } }
}

int main()
{
    InitSimulation();

    while (!WindowShouldClose())
    {
        Utils::HandleSimulationControl(camera, isPaused, hideStars, constellationActive);

        BeginDrawing();
        ClearBackground({.r = 24, .g = 24, .b = 22, .a = 255});

        BeginMode2D(camera);

        DrawCircleV({kWorldCenter.x - (kAnchorRadius / 2.0f), kWorldCenter.y - (kAnchorRadius / 2.0f)}, kAnchorRadius, RAYWHITE);
        DrawRectangleLines(0.0f, 0.0f, kWorldWidth, kWorldHeight,DARKGRAY);

        if (!hideStars)
        {
            for (const Star star : stars)
            {
                star.DrawStar();
            }
        }

        if (!isPaused)
        {
            for (int i{}; i < kSimulationSteps; ++i)
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
