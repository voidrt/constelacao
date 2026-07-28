#include "star.hpp"

void Star::Move()
{
    const Vector2 centripetal = Utils::VectorToroidalMod(kWorldCenter - this->position, {.x = kWorldWidth, .y = kWorldHeight});
    if (const float distanceToCenterSqr = Vector2LengthSqr(centripetal); distanceToCenterSqr > 1.0f)
    {
        this->velocity += Vector2Scale(Vector2Normalize(centripetal), (kGravityConstant * kDeltaTime * kAnchorMass) / distanceToCenterSqr);
    }

    this->position += (this->velocity * kDeltaTime);
}

void Star::MakeConstellation(const Grid& grid, const std::array<Star, kStarCount>& particles) const
{
    const int gridPositionY = std::floor(this->position.y / kConstellationRadius);
    const int gridPositionX = std::floor(this->position.x / kConstellationRadius);

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

                if (distanceSqr < (kConstellationRadius * kConstellationRadius))
                {
                    int opacity = Utils::OpacityScaling(distanceSqr);

                    DrawLineV(this->position, particles[particleId].position, {218, 218, 218, static_cast<unsigned char>(opacity)});
                }
            }
        }
    }
}
