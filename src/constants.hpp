#pragma once
#include <raylib.h>

//window and world size
inline constexpr int kWindowWidth = 1366;
inline constexpr int kWindowHeight = 768;
inline constexpr int kWorldWidth =5500;
inline constexpr int kWorldHeight = 5500;

//simulation
inline constexpr int kSimulationSteps = 1;
inline constexpr int kStarCount = 9804;
inline constexpr int kColumns = kWorldWidth / 50;
inline constexpr int kRows = kWorldHeight / 50;
inline constexpr float kAnchorMass = 4000.0f;
inline constexpr float kGravityConstant = 15.7f;
inline constexpr float kDeltaTime = 0.016f;
inline constexpr Vector3 kWorldCenter = {.x = kWorldWidth / 2.0f, .y = kWorldHeight / 2.0f, .z=0.0f};

//visuals
inline constexpr float kConstellationRadius = 50.0f;
inline constexpr float kStarSize = 2.4f;
inline constexpr int kMinimumConstellationOpacity = 5;
inline constexpr int kMaximumConstellationOpacity = 60;
inline constexpr float kAnchorRadius = 8.0f;
inline constexpr float kSpaceDepth = 20.0f;