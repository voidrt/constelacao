#pragma once
#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include "../utils.hpp"
#include "../grid/grid.hpp"

struct Star
{
    Vector3 position{};
    Vector3 velocity{};
    int id{};

    void Move();
    void DrawStar() const ;
    void MakeConstellation(const Grid& grid, const std::array<Star, kStarCount>& stars) const;
};
