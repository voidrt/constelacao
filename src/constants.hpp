#pragma once
#include <raylib.h>

//window and world size
inline constexpr int kWindowWidth = 1920;
inline constexpr int kWindowHeight = 1080;
inline constexpr int kWorldWidth =10500;
inline constexpr int kWorldHeight = 10500;

//simulation
inline constexpr int kSimulationSteps = 1;
inline constexpr int kStarCount = 13804;
inline constexpr int kColumns = kWorldWidth / 70;
inline constexpr int kRows = kWorldHeight / 70;
inline constexpr float kAnchorMass = 8000.0f;
inline constexpr float kGravityConstant = 25.7f;
inline constexpr float kDeltaTime = 0.016f;
inline constexpr Vector3 kWorldCenter = {.x = kWorldWidth / 2.0f, .y = kWorldHeight / 2.0f, .z=0.0f};

//visuals
inline constexpr float kConstellationRadius = 70.0f;
inline constexpr float kStarSize = 3.4f;
inline constexpr int kMinimumConstellationOpacity = 10;
inline constexpr int kMaximumConstellationOpacity = 120;
inline constexpr float kAnchorRadius = 10.0f;
inline constexpr float kSpaceDepth = 45.0f;
