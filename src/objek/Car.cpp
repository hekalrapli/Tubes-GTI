#include "objek/Car.h"
#include "objek/Wheel.h"
#include "Graphics.h"
#include "Globals.h"
#include <GL/glut.h>

void drawCarBody(float r, float g, float b, float wRot) {
    // Bodi utama
    glPushMatrix(); setMaterial(r,g,b); glScalef(1.5f,.5f,2.8f); glutSolidCube(1); glPopMatrix();
    // Kabin
    glPushMatrix(); setMaterial(1.0f, 0.0f, 0.0f); glTranslatef(0,.45f,.2f); glScalef(1,.5f,1.3f); glutSolidCube(1); glPopMatrix();

    // Roda kiri: rotasi -90Y agar bidang roda menghadap samping
    glPushMatrix(); glTranslatef(-.82f,-.30f, 1.0f); glRotatef(-90,0,1,0); drawDetailedWheel(wRot); glPopMatrix();
    glPushMatrix(); glTranslatef( .82f,-.30f, 1.0f); glRotatef( 90,0,1,0); drawDetailedWheel(wRot); glPopMatrix();
    glPushMatrix(); glTranslatef(-.82f,-.30f,-1.0f); glRotatef(-90,0,1,0); drawDetailedWheel(wRot); glPopMatrix();
    glPushMatrix(); glTranslatef( .82f,-.30f,-1.0f); glRotatef( 90,0,1,0); drawDetailedWheel(wRot); glPopMatrix();
}

void drawEnemyCar() { drawCarBody(1.f, .15f, .15f, wheelRotAngleEnemy); }

void drawPlayerCar() {
    float rollAngle = playerRoll, yawAngle = -rollAngle * .3f;
    glRotatef(yawAngle, 0,1,0);
    glRotatef(rollAngle, 0,0,1);
    drawCarBody(0.0f, 255.0f, 0.0f, wheelRotAngle);
}

void drawCarShadow(float tx,float tz){
    float m[16]; getShadowMatrix(m,SHADOW_Y,20,25,-20);
    glDisable(GL_LIGHTING); glColor4f(0,0,0,.4f);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix(); glTranslatef(tx,0,tz); glMultMatrixf(m);
    glPushMatrix(); glScalef(1.5f,.5f,2.8f); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(0,.45f,.2f); glScalef(1,.5f,1.3f); glutSolidCube(1); glPopMatrix();
    glPopMatrix(); glDisable(GL_BLEND); glEnable(GL_LIGHTING);
}
