#ifndef TYPES_H
#define TYPES_H

struct Enemy { float x, z; };

struct NosPickup {
    float x, z;
    bool  active;
    float rotAngle;
    float bobTimer;
};

struct Tree {
    float x, z;
    float trunkH, crownR;
    int   seed;
};

struct Building {
    float x, z;
    float w, d, h;
    float r, g, b;
};

enum GameState { STATE_START, STATE_PLAYING, STATE_CRASH, STATE_OVER, STATE_WIN };

#endif
