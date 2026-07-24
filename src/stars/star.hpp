#pragma once
#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include "../utils.hpp"
#include "../grid/grid.hpp"

struct Star
{
    Vector2 position{};
    Vector2 velocity{};
    int id{};

    void Move();
    void MakeConstellation(const Grid& grid, const std::array<Star, kStarCount>& particles) const;
};
