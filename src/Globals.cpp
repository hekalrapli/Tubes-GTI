#include "Globals.h"

// GLOBAL STATE
// =====================================================================

std::vector<Enemy>     enemies;
std::vector<NosPickup> nosPickups;
std::vector<Tree>      trees;
std::vector<Building>  buildings;

// -- NOS --
float nosAmount        = 0.0f;
float nosMax           = 100.0f;
bool  nosActive        = false;
float nosBoostSpeed    = 0.45f;
float nosDrainRate     = 1.2f;
float nosSpawnTimer    = 0.0f;
float nosSpawnInterval = 200.0f;

int   windowWidth  = 800;
int   windowHeight = 600;

float lanePos[] = { -2.5f, 0.0f, 2.5f };
int   playerLane = 1;

float playerX       = 0.0f;
float playerTargetX = 0.0f;
float laneChangeSpeed = 0.18f;
float playerRoll    = 0.0f;
float playerZ       = 4.0f;

float baseSpeed      = 0.35f;
float currentSpeed   = 0.35f;
float speedIncrement = 0.00008f;
float maxSpeed       = 1.2f;

int   spawnCooldown    = 0;
float distanceTraveled = 0.0f;
float targetDistance   = 1000.0f;

bool gameOver       = false;
bool winGame        = false;
bool usePerspective = true;

float roadAnim   = 0.0f;
int   cameraMode = 3;

const float SHADOW_Y = -0.58f;

// =====================================================================
// ANIMASI RODA — sudut rotasi global
// =====================================================================
float wheelRotAngle      = 0.0f;   // derajat, berputar saat bermain
float wheelRotAngleEnemy = 0.0f;   // roda musuh ikut berputar

// =====================================================================
// GAME STATE – layar awal

GameState gameState = STATE_START;

float startAnimTimer = 0.0f;
// =====================================================================
// CRASH INTERACTION STATE
// Menyimpan efek interaksi saat mobil player bertabrakan dengan mobil enemy.
// =====================================================================
float crashTimer         = 0.0f;
float crashDuration      = 0.85f;
int   crashEnemyIndex    = -1;
float crashImpactX       = 0.0f;
float crashImpactZ       = 0.0f;
float crashPlayerOffsetX = 0.0f;
float crashPlayerOffsetZ = 0.0f;
float crashEnemyOffsetX  = 0.0f;
float crashEnemyOffsetZ  = 0.0f;
float crashImpulseX      = 1.0f;
float crashSpinAngle     = 0.0f;
float crashFlashAlpha    = 0.0f;

