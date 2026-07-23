#pragma once
#include <raylib.h>

inline constexpr int kWindowWidth = 1366;
inline constexpr int kWindowHeight = 768;
inline constexpr int kWorldWidth = 2950;
inline constexpr int kWorldHeight = 2950;
inline constexpr Vector2 kWorldCenter = {.x = kWorldWidth / 2.0f, .y = kWorldHeight / 2.0f};
inline constexpr int kParticleCount = 3370;
inline constexpr float kParticleSpeed = 17.5f;
inline constexpr float kConstellationRadiusSqr = 2500.0f; //25
inline constexpr float kStarSize = 2.0f;
inline constexpr float kBHoleRadius = 5.0f;
inline constexpr float kBHoleMass = 400.0f;
inline constexpr float kGravityConstant = 5.0f;
inline constexpr int kRows = kWorldHeight / 25;
inline constexpr int kColumns = kWorldWidth / 25;
inline constexpr float kPi = 3.1415926535f;
inline constexpr float kDeltaTime = 0.016f;
