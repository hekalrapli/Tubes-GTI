#include "objek/Wheel.h"
#include "Graphics.h"
#include <GL/glut.h>
#include <cmath>

void drawDetailedWheel(float rotAngle) {
    const float R_MAJOR  = 0.30f;  
    const float R_MINOR  = 0.10f;  
    const float RIM_R    = R_MAJOR - R_MINOR * 0.3f; 
    const float HUB_R    = 0.06f;
    const float HALF_W   = R_MINOR + 0.02f; 
    const int   MAJOR_S  = 32;     
    const int   MINOR_S  = 16;    
    const int   N_SPOKES = 5;
    const float PI       = 3.14159265f;

    glPushMatrix();

    glRotatef(rotAngle, 0.0f, 0.0f, 1.0f);

   
    setMaterial(0.10f, 0.10f, 0.10f, 15.f);
    for (int i = 0; i < MAJOR_S; i++) {
        float phi0 = (float)i       / MAJOR_S * 2*PI;
        float phi1 = (float)(i + 1) / MAJOR_S * 2*PI;
        float cp0 = cosf(phi0), sp0 = sinf(phi0);
        float cp1 = cosf(phi1), sp1 = sinf(phi1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= MINOR_S; j++) {
            float theta = (float)j / MINOR_S * 2*PI;
            float ct = cosf(theta), st = sinf(theta);
            // Titik torus: pusat tabung di (R_MAJOR*cos(phi), R_MAJOR*sin(phi), 0)
            // titik permukaan = pusat + R_MINOR*(ct * radialDir + st * Z)
            // radialDir = (cos(phi), sin(phi), 0)
            float x0 = (R_MAJOR + R_MINOR * ct) * cp0;
            float y0 = (R_MAJOR + R_MINOR * ct) * sp0;
            float z0 =  R_MINOR * st;

            float x1 = (R_MAJOR + R_MINOR * ct) * cp1;
            float y1 = (R_MAJOR + R_MINOR * ct) * sp1;
            float z1 =  R_MINOR * st;

            // Normal = arah dari pusat tabung ke titik permukaan
            float nx0 = ct * cp0, ny0 = ct * sp0, nz0 = st;
            float nx1 = ct * cp1, ny1 = ct * sp1;

            glNormal3f(nx0, ny0, nz0); glVertex3f(x0, y0, z0);
            glNormal3f(nx1, ny1, nz0); glVertex3f(x1, y1, z1);
        }
        glEnd();
    }

    // ----------------------------------------------------------------
    // VELG — disk silver di kedua sisi, radius sampai tepi dalam ban
    // ----------------------------------------------------------------
    setMaterial(0.72f, 0.76f, 0.82f, 120.f);
    for (int side = -1; side <= 1; side += 2) {
        float wz = side * HALF_W * 0.55f; // sedikit masuk dari tepi ban
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0, 0, (float)side);
        glVertex3f(0, 0, wz);
        for (int s = 0; s <= MAJOR_S; s++) {
            float a = (float)s / MAJOR_S * 2*PI * (float)side; // balik arah sisi kanan
            glVertex3f(cosf(a) * RIM_R, sinf(a) * RIM_R, wz);
        }
        glEnd();
    }

    // ----------------------------------------------------------------
    // SPOKE — 5 jari-jari dari hub ke velg, dua sisi
    // ----------------------------------------------------------------
    setMaterial(0.60f, 0.64f, 0.70f, 80.f);
    for (int sp = 0; sp < N_SPOKES; sp++) {
        float ang = (float)sp / N_SPOKES * 360.0f;
        glPushMatrix();
        glRotatef(ang, 0, 0, 1);
        // Spoke: kotak pipih memanjang dari hub ke rim pada sumbu Y lokal
        float spokeLen = RIM_R - HUB_R;
        glTranslatef(0, HUB_R + spokeLen * 0.5f, 0);
        glScalef(0.055f, spokeLen, HALF_W * 1.1f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    // ----------------------------------------------------------------
    // HUB — silinder pendek di tengah roda
    // ----------------------------------------------------------------
    setMaterial(0.82f, 0.85f, 0.90f, 150.f);
    GLUquadric* q = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0, 0, -HALF_W);
    gluCylinder(q, HUB_R, HUB_R, HALF_W * 2.0f, 14, 1);
    glRotatef(180, 1, 0, 0);
    gluDisk(q, 0, HUB_R, 14, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, HALF_W);
    gluDisk(q, 0, HUB_R, 14, 1);
    glPopMatrix();
    gluDeleteQuadric(q);

    // ---- Baut hub (5 titik kecil) ----
    setMaterial(0.25f, 0.25f, 0.30f, 200.f);
    float boltR = HUB_R * 0.58f;
    for (int b = 0; b < 5; b++) {
        float ba = (float)b / 5 * 2*PI;
        float bx = cosf(ba) * boltR, by = sinf(ba) * boltR;
        glPushMatrix(); glTranslatef(bx, by, HALF_W + 0.01f); glScalef(0.025f,0.025f,0.03f); glutSolidCube(1); glPopMatrix();
        glPushMatrix(); glTranslatef(bx, by,-HALF_W - 0.01f); glScalef(0.025f,0.025f,0.03f); glutSolidCube(1); glPopMatrix();
    }

    glPopMatrix();
}
