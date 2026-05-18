#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include "Types.h"

extern std::vector<Enemy> enemies;
extern std::vector<NosPickup> nosPickups;
extern std::vector<Tree> trees;
extern std::vector<Building> buildings;

extern float nosAmount;
extern float nosMax;
extern bool  nosActive;
extern float nosBoostSpeed;
extern float nosDrainRate;
extern float nosSpawnTimer;
extern float nosSpawnInterval;

extern int windowWidth;
extern int windowHeight;

extern float lanePos[3];
extern int playerLane;
extern float playerX;
extern float playerTargetX;
extern float laneChangeSpeed;
extern float playerRoll;
extern float playerZ;

extern float baseSpeed;
extern float currentSpeed;
extern float speedIncrement;
extern float maxSpeed;

extern int spawnCooldown;
extern float distanceTraveled;
extern float targetDistance;

extern bool gameOver;
extern bool winGame;
extern bool usePerspective;

extern float roadAnim;
extern int cameraMode;

extern const float SHADOW_Y;

extern float wheelRotAngle;
extern float wheelRotAngleEnemy;

extern GameState gameState;
extern float startAnimTimer;

extern float crashTimer;
extern float crashDuration;
extern int   crashEnemyIndex;
extern float crashImpactX;
extern float crashImpactZ;
extern float crashPlayerOffsetX;
extern float crashPlayerOffsetZ;
extern float crashEnemyOffsetX;
extern float crashEnemyOffsetZ;
extern float crashImpulseX;
extern float crashSpinAngle;
extern float crashFlashAlpha;

#endif
