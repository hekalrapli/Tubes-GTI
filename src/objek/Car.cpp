#include "objek/Car.h"
#include "objek/Wheel.h"
#include "Graphics.h"
#include "Globals.h"
#include <GL/glut.h>

static GLuint playerRearLogoTex = 0;

static void drawRearLogoDecal() {
    if (playerRearLogoTex == 0) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, playerRearLogoTex);

    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);

    float z = 1.405f;

    float x1 = -0.72f;
    float x2 =  0.72f;
    float y1 = -0.20f;
    float y2 =  0.24f;


glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, y1, z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x2, y1, z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x2, y2, z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y2, z);
glEnd();

    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void initCarTextures() {
    playerRearLogoTex = loadBMPTexture("assets/mclaren_f1.bmp");
}

void drawCarBody(
    float r, float g, float b,
    float glassR, float glassG, float glassB,
    float wRot
) {
    // =========================
    // Bodi utama mobil
    // =========================
    glPushMatrix();
    setMaterial(r, g, b);
    glScalef(1.5f, 0.5f, 2.8f);
    glutSolidCube(1);
    glPopMatrix();

    // =========================
    // Kabin mobil
    // =========================
    glPushMatrix();

    // Kabin dibuat gelap agar warna kaca terlihat jelas
    setMaterial(r, g, b);

    glTranslatef(0.0f, 0.45f, 0.2f);
    glScalef(1.0f, 0.5f, 1.3f);
    glutSolidCube(1);
    glPopMatrix();

    // =========================
    // Kaca mobil
    // =========================
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Warna kaca mengikuti parameter
    glColor4f(glassR, glassG, glassB, 0.65f);

    // Kaca depan
    glPushMatrix();
    glTranslatef(0.0f, 0.49f, 0.87f);
    glScalef(0.75f, 0.28f, 0.035f);
    glutSolidCube(1);
    glPopMatrix();

    // Kaca belakang
    glPushMatrix();
    glTranslatef(0.0f, 0.49f, -0.47f);
    glScalef(0.75f, 0.28f, 0.035f);
    glutSolidCube(1);
    glPopMatrix();

    // Kaca samping kiri
    glPushMatrix();
    glTranslatef(-0.515f, 0.49f, 0.2f);
    glScalef(0.035f, 0.28f, 0.85f);
    glutSolidCube(1);
    glPopMatrix();

    // Kaca samping kanan
    glPushMatrix();
    glTranslatef(0.515f, 0.49f, 0.2f);
    glScalef(0.035f, 0.28f, 0.85f);
    glutSolidCube(1);
    glPopMatrix();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);

    // =========================
    // Roda mobil
    // =========================

    // Roda kiri depan
    glPushMatrix();
    glTranslatef(-0.82f, -0.30f, 1.0f);
    glRotatef(-90, 0, 1, 0);
    drawDetailedWheel(wRot);
    glPopMatrix();

    // Roda kanan depan
    glPushMatrix();
    glTranslatef(0.82f, -0.30f, 1.0f);
    glRotatef(90, 0, 1, 0);
    drawDetailedWheel(wRot);
    glPopMatrix();

    // Roda kiri belakang
    glPushMatrix();
    glTranslatef(-0.82f, -0.30f, -1.0f);
    glRotatef(-90, 0, 1, 0);
    drawDetailedWheel(wRot);
    glPopMatrix();

    // Roda kanan belakang
    glPushMatrix();
    glTranslatef(0.82f, -0.30f, -1.0f);
    glRotatef(90, 0, 1, 0);
    drawDetailedWheel(wRot);
    glPopMatrix();
}

void drawEnemyCar() {
    // Mobil musuh merah, kaca merah
    drawCarBody(
        1.0f, 0.15f, 0.15f,
         0.25f, 0.75f, 1.0f,
        wheelRotAngleEnemy
    );
}

void drawPlayerCar() {
    float rollAngle = playerRoll;
    float yawAngle = -rollAngle * 0.3f;

    glRotatef(yawAngle, 0, 1, 0);
    glRotatef(rollAngle, 0, 0, 1);

    // Mobil player biru, kaca biru
    drawCarBody(
        0.0f, 0.0f, 1.0f,
        0.25f, 0.75f, 1.0f,
        wheelRotAngle
    );
    drawRearLogoDecal();

}

void drawCarShadow(float tx, float tz) {
    float m[16];

    getShadowMatrix(m, SHADOW_Y, 20, 25, -20);

    glDisable(GL_LIGHTING);
    glColor4f(0, 0, 0, 0.4f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glTranslatef(tx, 0, tz);
    glMultMatrixf(m);

    // Bayangan bodi utama
    glPushMatrix();
    glScalef(1.5f, 0.5f, 2.8f);
    glutSolidCube(1);
    glPopMatrix();

    // Bayangan kabin
    glPushMatrix();
    glTranslatef(0.0f, 0.45f, 0.2f);
    glScalef(1.0f, 0.5f, 1.3f);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

