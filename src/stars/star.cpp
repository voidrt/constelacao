#include "star.hpp"

void Star::DrawStar() const
{
    const float side = kStarSize + (this->position.z / 15.0f);
    const Vector2 starMiddle = {.x = side / 2.0f, .y = side / 2.0f};
    const Color starColor = {.r = 218, .g = 218, .b = 218, .a = 255};
    const Rectangle starBody = {.x = this->position.x, .y = this->position.y, .width = side, .height = side};
    const float headingAngle = atan2f(this->velocity.y, this->velocity.x) * RAD2DEG;


    DrawRectanglePro(starBody, starMiddle, headingAngle, {.r = 218, .g = 218, .b = 218, .a = 255});
    // Vector2 vec1 = Vector2Add({0.0f, side / 4.0f}, {this->position.x, this->position.y});
    // Vector2 vec2 = Vector2Add({side / 4.0f, -side / 4.0f}, {this->position.x, this->position.y});
    // Vector2 vec3 = Vector2Add({-side / 4.0f, -side / 4.0f}, {this->position.x, this->position.y});
    //
    // Vector2 vec4 = Vector2Add({0.0f, -1.0f * side}, {this->position.x, this->position.y});
    // Vector2 vec5 = Vector2Add({-1.0f * side * fabsf(cosf(7 * (PI/6))), 1.0f * side * fabsf(sinf(7 * (PI/6)))}, {this->position.x, this->position.y});
    // Vector2 vec6 = Vector2Add({1.0f * side * fabsf(cosf(-(PI/6))), 1.0f * side * fabsf(sinf(-(PI/6)))}, {this->position.x, this->position.y});
    //
    //
    // DrawTriangle(vec2, vec3, vec1, starColor); //middle
    // DrawTriangle(vec3, vec2, vec4, starColor); //top
    // DrawTriangle(vec5, vec1, vec3, starColor); //left
    // DrawTriangle(vec1, vec6, vec2, starColor); //right
}

void Star::Move()
{
    const Vector3 centripetal = kWorldCenter - this->position;
    if (const float distanceToCenterSqr = Vector3LengthSqr(centripetal); distanceToCenterSqr > 1.0f)
    {
        this->velocity += Vector3Scale(Vector3Normalize(centripetal), (kGravityConstant * kDeltaTime * kAnchorMass) / distanceToCenterSqr);
    }

    this->position += (this->velocity * kDeltaTime);
}

void Star::MakeConstellation(const Grid& grid, const std::array<Star, kStarCount>& stars) const
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

            for (const auto starId : grid[gridRow][gridColumn])
            {
                if (this->id == starId) { continue; }

                const float distanceSqr = Vector3DistanceSqr(this->position, stars[starId].position);

                if (distanceSqr < (kConstellationRadius * kConstellationRadius))
                {
                    int opacity = Utils::OpacityScaling(distanceSqr);
                    Vector2 position2D = {this->position.x, this->position.y};
                    Vector2 otherStarPosition2D = {stars[starId].position.x, stars[starId].position.y};
                    DrawLineV(position2D, otherStarPosition2D, {218, 218, 218, static_cast<unsigned char>(opacity)});
                }
            }
        }
    }
}
