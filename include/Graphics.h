#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glut.h>

void getShadowMatrix(float m[16], float planeY, float lx, float ly, float lz);
void drawText2D(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_18);
void drawTextCentered(float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_18);
void begin2D();
void end2D();
void setProjection();
void setCamera(float extraRotY = 0.0f);
void setupLighting();
void setMaterial(float r, float g, float b, float sh = 50.0f);

#endif
