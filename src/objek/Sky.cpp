#include "objek/Sky.h"
#include "Globals.h"
#include <GL/glut.h>
#include <cmath>

void drawSun(){
    glPushMatrix(); glTranslatef(20,25,-20);
    glDisable(GL_LIGHTING); glColor3f(1,.9f,0); glutSolidSphere(3,32,32);
    glEnable(GL_LIGHTING); glPopMatrix();
}

// Skybox: gradient langit sebagai fullscreen quad 2D (tanpa garis tepi),
// lalu ground plane lebar mengikuti kamera untuk nutup horizon.
void drawSkybox(float camX, float camZ) {
    // --- Langit: fullscreen quad 2D gradient, tidak terpengaruh matriks 3D ---
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    glOrtho(0,1,0,1,-1,1);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    // gradient: bawah = biru horizon, atas = biru gelap
    glBegin(GL_QUADS);
    glColor3f(0.55f, 0.82f, 1.00f); glVertex2f(0,0);   // kiri bawah
    glColor3f(0.55f, 0.82f, 1.00f); glVertex2f(1,0);   // kanan bawah
    glColor3f(0.18f, 0.48f, 0.88f); glVertex2f(1,1);   // kanan atas
    glColor3f(0.18f, 0.48f, 0.88f); glVertex2f(0,1);   // kiri atas
    glEnd();

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  glPopMatrix();

    // --- Ground plane: quad besar mengikuti kamera untuk tutup horizon ---
    glDisable(GL_LIGHTING);
    float R = 185.0f;
    glBegin(GL_QUADS);
    glColor3f(0.15f, 0.44f, 0.10f);
    glVertex3f(camX-R, -0.62f, camZ-R);
    glVertex3f(camX+R, -0.62f, camZ-R);
    glVertex3f(camX+R, -0.62f, camZ+R);
    glVertex3f(camX-R, -0.62f, camZ+R);
    glEnd();
    glEnable(GL_LIGHTING);
}
