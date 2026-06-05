#include "../include/UI.h"
#include "Graphics.h"
#include "Globals.h"
#include <GL/glut.h>
#include <cmath>
#include <cstdio>

// =====================================================================
// HELPER UI
// =====================================================================

static float clamp01(float v) {
    if (v < 0.0f) return 0.0f;
    if (v > 1.0f) return 1.0f;
    return v;
}

static void drawRect(float x, float y, float w, float h, float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
    glVertex2f(x,     y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x,     y + h);
    glEnd();
}

static void drawRectOutline(float x, float y, float w, float h, float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x,     y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x,     y + h);
    glEnd();
}

static void drawLabel(float x, float y, const char* text, float r, float g, float b) {
    glColor4f(r, g, b, 1.0f);
    drawText2D(x, y, text, GLUT_BITMAP_HELVETICA_12);
}

static void drawProgressBar(
    float x, float y, float w, float h,
    float ratio,
    float r1, float g1, float b1,
    float r2, float g2, float b2
) {
    ratio = clamp01(ratio);

    // Background bar
    drawRect(x, y, w, h, 0.03f, 0.05f, 0.10f, 0.95f);

    // Inner shadow
    drawRect(x + 2, y + 2, w - 4, h - 4, 0.0f, 0.0f, 0.0f, 0.35f);

    // Fill bar dengan efek gradient
    if (ratio > 0.0f) {
        float fw = (w - 4) * ratio;

        glBegin(GL_QUADS);
        glColor4f(r1, g1, b1, 1.0f);
        glVertex2f(x + 2,      y + 2);
        glVertex2f(x + 2,      y + h - 2);

        glColor4f(r2, g2, b2, 1.0f);
        glVertex2f(x + 2 + fw, y + h - 2);
        glVertex2f(x + 2 + fw, y + 2);
        glEnd();

        // Highlight bagian atas bar
        if (fw > 4.0f) {
            drawRect(x + 3, y + h - 5, fw - 2, 2, 1.0f, 1.0f, 1.0f, 0.25f);
        }
    }

    // Garis segmentasi bar
    glColor4f(1.0f, 1.0f, 1.0f, 0.12f);
    glBegin(GL_LINES);
    for (int i = 1; i < 10; i++) {
        float sx = x + (w / 10.0f) * i;
        glVertex2f(sx, y + 2);
        glVertex2f(sx, y + h - 2);
    }
    glEnd();

    // Outline
    glLineWidth(1.5f);
    drawRectOutline(x, y, w, h, 0.0f, 0.85f, 1.0f, 0.75f);
    glLineWidth(1.0f);
}

// =====================================================================
// HUD GAME
// =====================================================================

void drawHUD() {
    begin2D();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    char buf[128];

    // =================================================================
    // PANEL KIRI ATAS — diperbesar & jarak antar elemen dirapikan
    // =================================================================
    float panelX = 18.0f;
    float panelW = 320.0f;   // lebih lebar agar teks tidak mepet ke tepi
    float panelH = 165.0f;   // ditinggikan dari 142 → 165 agar tidak sesak
    float panelY = windowHeight - panelH - 18.0f;

    // Lebar bar seragam untuk semua progress bar di panel ini
    float barW = 278.0f;

    // Shadow panel
    drawRect(panelX + 5, panelY - 5, panelW, panelH, 0.0f, 0.0f, 0.0f, 0.35f);

    // Background panel
    drawRect(panelX, panelY, panelW, panelH, 0.035f, 0.055f, 0.12f, 0.88f);

    // Aksen kiri
    drawRect(panelX, panelY, 5.0f, panelH, 0.0f, 0.85f, 1.0f, 0.95f);

    // Outline panel
    glLineWidth(2.0f);
    drawRectOutline(panelX, panelY, panelW, panelH, 0.0f, 0.85f, 1.0f, 0.65f);
    glLineWidth(1.0f);

    // Header — sedikit turun dari tepi atas agar tidak terpotong
    glColor4f(0.0f, 0.9f, 1.0f, 1.0f);
    drawText2D(panelX + 18, panelY + panelH - 28, "RACE STATUS", GLUT_BITMAP_HELVETICA_18);

    // Mode kamera
    if (usePerspective) {
        sprintf(buf, "CAMERA  PERSPECTIVE %d-POINT", cameraMode);
    } else {
        sprintf(buf, "CAMERA  ORTHOGRAPHIC");
    }

    glColor4f(0.7f, 0.85f, 1.0f, 0.85f);
    drawText2D(panelX + 18, panelY + panelH - 48, buf, GLUT_BITMAP_HELVETICA_12);

    // =================================================================
    // JARAK TEMPUH
    // =================================================================
    float distRatio = (targetDistance > 0.0f)
        ? clamp01(distanceTraveled / targetDistance)
        : 0.0f;

    // Baris ini dimulai di posisi yang cukup jauh dari header
    float distanceY = panelY + 90.0f;

    // Label kiri dan nilai kanan — disejajarkan dalam satu baris
    drawLabel(panelX + 18, distanceY + 18, "JARAK TEMPUH", 1.0f, 0.85f, 0.25f);

    sprintf(buf, "%.0f / %.0f m", distanceTraveled, targetDistance);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    // Nilai di sisi kanan panel agar tidak tumpang tindih dengan label
    drawText2D(panelX + panelW - 108, distanceY + 18, buf, GLUT_BITMAP_HELVETICA_12);

    drawProgressBar(
        panelX + 18, distanceY,
        barW, 16.0f,
        distRatio,
        1.0f, 0.75f, 0.05f,
        0.0f, 0.9f, 1.0f
    );

    // =================================================================
    // NOS BAR
    // =================================================================
    float nosRatio = (nosMax > 0.0f)
        ? clamp01(nosAmount / nosMax)
        : 0.0f;

    // Baris NOS di bawah baris Jarak, dengan jarak vertikal yang cukup
    float nosY = panelY + 44.0f;

    // Label NOS dan persentase di atas bar (bukan di dalam bar)
    drawLabel(panelX + 18, nosY + 20, "NOS  [W]", 0.0f, 0.9f, 1.0f);

    sprintf(buf, "%.0f%%", nosRatio * 100.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    drawText2D(panelX + panelW - 55, nosY + 20, buf, GLUT_BITMAP_HELVETICA_12);

    if (nosActive && nosAmount > 0.0f) {
        drawProgressBar(
            panelX + 18, nosY,
            barW, 15.0f,
            nosRatio,
            1.0f, 1.0f, 0.25f,
            0.0f, 0.95f, 1.0f
        );

        // Status BOOST ACTIVE ditampilkan DI BAWAH bar, tidak di dalam bar
        glColor4f(1.0f, 1.0f, 0.25f, 1.0f);
        drawText2D(panelX + 18, nosY - 14, "> BOOST ACTIVE", GLUT_BITMAP_HELVETICA_12);
    }
    else {
        drawProgressBar(
            panelX + 18, nosY,
            barW, 15.0f,
            nosRatio,
            0.0f, 0.7f, 1.0f,
            0.0f, 1.0f, 0.9f
        );

        if (nosAmount <= 0.0f) {
            // Status NOS EMPTY juga di bawah bar
            glColor4f(0.65f, 0.65f, 0.65f, 1.0f);
            drawText2D(panelX + 18, nosY - 14, "> NOS EMPTY", GLUT_BITMAP_HELVETICA_12);
        }
    }

    // =================================================================
    // SPEED PANEL KANAN ATAS
    // =================================================================
    float speedRatio = 0.0f;

    if ((maxSpeed - baseSpeed) != 0.0f) {
        speedRatio = (currentSpeed - baseSpeed) / (maxSpeed - baseSpeed);
    }

    speedRatio = clamp01(speedRatio);

    int speedKmh = (int)(80 + speedRatio * 140);

    float speedW = 190.0f;
    float speedH = 82.0f;
    float speedX = windowWidth - speedW - 18.0f;
    float speedY = windowHeight - speedH - 18.0f;

    // Shadow speed panel
    drawRect(speedX + 5, speedY - 5, speedW, speedH, 0.0f, 0.0f, 0.0f, 0.35f);

    // Background speed panel
    drawRect(speedX, speedY, speedW, speedH, 0.035f, 0.055f, 0.12f, 0.88f);

    // Outline speed panel
    glLineWidth(2.0f);

    if (speedRatio > 0.75f) {
        drawRectOutline(speedX, speedY, speedW, speedH, 1.0f, 0.25f, 0.1f, 0.9f);
    } else {
        drawRectOutline(speedX, speedY, speedW, speedH, 1.0f, 0.65f, 0.0f, 0.8f);
    }

    glLineWidth(1.0f);

    drawLabel(speedX + 16, speedY + speedH - 24, "SPEED", 1.0f, 0.65f, 0.0f);

    sprintf(buf, "%d", speedKmh);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    drawText2D(speedX + 16, speedY + 26, buf, GLUT_BITMAP_TIMES_ROMAN_24);

    glColor4f(0.75f, 0.85f, 1.0f, 1.0f);
    drawText2D(speedX + 72, speedY + 31, "km/h", GLUT_BITMAP_HELVETICA_18);

    drawProgressBar(
        speedX + 16, speedY + 12,
        158.0f, 12.0f,
        speedRatio,
        0.0f, 1.0f, 0.35f,
        1.0f, 0.25f, 0.0f
    );

    // =================================================================
    // PETUNJUK KONTROL BAWAH
    // =================================================================
    glColor4f(0.75f, 0.85f, 1.0f, 0.8f);
    // drawText2D(
    //     20,
    //     28,
    //     "Arrow L/R = Pindah Lane   W = NOS   P/O = Proyeksi   1/2/3 = Kamera",
    //     GLUT_BITMAP_HELVETICA_12
    // );

    glDisable(GL_BLEND);

    end2D();
}

// =====================================================================
// LAYAR START
// =====================================================================

void drawStartScreen() {
    begin2D();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float pw = 460;
    float ph = 340;
    float px = (windowWidth - pw) * 0.5f;
    float py = (windowHeight - ph) * 0.5f;

    glColor4f(0, 0, 0, 0.45f);
    glBegin(GL_QUADS);
    glVertex2f(px + 6,      py - 6);
    glVertex2f(px + pw + 6, py - 6);
    glVertex2f(px + pw + 6, py + ph - 6);
    glVertex2f(px + 6,      py + ph - 6);
    glEnd();

    glColor4f(0.04f, 0.06f, 0.14f, 0.88f);
    glBegin(GL_QUADS);
    glVertex2f(px,      py);
    glVertex2f(px + pw, py);
    glVertex2f(px + pw, py + ph);
    glVertex2f(px,      py + ph);
    glEnd();

    glLineWidth(2.5f);
    glColor4f(0.0f, 0.85f, 1.0f, 0.9f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(px,      py);
    glVertex2f(px + pw, py);
    glVertex2f(px + pw, py + ph);
    glVertex2f(px,      py + ph);
    glEnd();

    glLineWidth(1.0f);
    glColor4f(0.0f, 0.5f, 0.7f, 0.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(px + 5,      py + 5);
    glVertex2f(px + pw - 5, py + 5);
    glVertex2f(px + pw - 5, py + ph - 5);
    glVertex2f(px + 5,      py + ph - 5);
    glEnd();

    float lineY = py + ph - 72;

    glLineWidth(1.5f);
    glColor4f(0.0f, 0.85f, 1.0f, 0.6f);
    glBegin(GL_LINES);
    glVertex2f(px + 20,      lineY);
    glVertex2f(px + pw - 20, lineY);
    glEnd();

    glLineWidth(1.0f);

    glColor3f(0.0f, 0.92f, 1.0f);
    drawTextCentered(py + ph - 55, "3D DODGE CAR", GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1.0f, 0.85f, 0.0f);
    drawTextCentered(py + ph - 80, "NOS  EDITION", GLUT_BITMAP_HELVETICA_18);

    float colX = px + 30;
    float rowY = py + ph - 118;
    float rowGap = 26;

    glColor3f(0.6f, 0.9f, 1.0f);
    drawText2D(colX, rowY, "KONTROL :", GLUT_BITMAP_HELVETICA_12);

    glColor3f(1, 1, 1);
    drawText2D(colX, rowY - rowGap * 1, "[Panah Kiri / Kanan]  Pindah Lane", GLUT_BITMAP_HELVETICA_12);
    drawText2D(colX, rowY - rowGap * 2, "[W]                   Aktifkan NOS Boost", GLUT_BITMAP_HELVETICA_12);

    drawText2D(colX, rowY - rowGap * 4, "[1 / 2 / 3]           Ganti Sudut Kamera", GLUT_BITMAP_HELVETICA_12);
    drawText2D(colX, rowY - rowGap * 5, "[R]                   Restart ke Menu", GLUT_BITMAP_HELVETICA_12);
    drawText2D(colX, rowY - rowGap * 6, "[ESC]                 Keluar", GLUT_BITMAP_HELVETICA_12);

    float line2Y = py + 54;

    glLineWidth(1.5f);
    glColor4f(0.0f, 0.85f, 1.0f, 0.4f);
    glBegin(GL_LINES);
    glVertex2f(px + 20,      line2Y);
    glVertex2f(px + pw - 20, line2Y);
    glEnd();

    glLineWidth(1.0f);

    float blink = sinf(startAnimTimer * 3.5f);

    if (blink > 0) {
        float alpha = 0.55f + blink * 0.45f;
        glColor4f(1.0f, 1.0f, 0.2f, alpha);
        drawTextCentered(py + 28, "[ TEKAN  SPACE  UNTUK  MULAI ]", GLUT_BITMAP_HELVETICA_18);
    }

    glDisable(GL_BLEND);

    end2D();
}

// =====================================================================
// LAYAR GAME OVER / WIN
// =====================================================================

void drawEndScreen() {
    begin2D();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float pw = 340;
    float ph = 180;
    float px = (windowWidth - pw) * 0.5f;
    float py = (windowHeight - ph) * 0.5f;

    glColor4f(0.04f, 0.06f, 0.14f, 0.88f);
    glBegin(GL_QUADS);
    glVertex2f(px,      py);
    glVertex2f(px + pw, py);
    glVertex2f(px + pw, py + ph);
    glVertex2f(px,      py + ph);
    glEnd();

    glLineWidth(2.5f);

    if (gameState == STATE_OVER) {
        glColor4f(1.0f, 0.2f, 0.2f, 1.0f);
    } else {
        glColor4f(0.2f, 1.0f, 0.4f, 1.0f);
    }

    glBegin(GL_LINE_LOOP);
    glVertex2f(px,      py);
    glVertex2f(px + pw, py);
    glVertex2f(px + pw, py + ph);
    glVertex2f(px,      py + ph);
    glEnd();

    glLineWidth(1.0f);

    if (gameState == STATE_OVER) {
        glColor3f(1.0f, 0.25f, 0.25f);
        drawTextCentered(py + ph - 50, "GAME  OVER !", GLUT_BITMAP_TIMES_ROMAN_24);

        glColor3f(0.9f, 0.9f, 0.9f);
        drawTextCentered(py + ph - 85, "Mobil kamu bertabrakan !", GLUT_BITMAP_HELVETICA_18);
    } else {
        glColor3f(0.2f, 1.0f, 0.4f);
        drawTextCentered(py + ph - 50, "YOU  WIN !", GLUT_BITMAP_TIMES_ROMAN_24);

        glColor3f(0.9f, 0.9f, 0.9f);
        drawTextCentered(py + ph - 85, "Finish line tercapai !", GLUT_BITMAP_HELVETICA_18);
    }

    char sbuf[64];

    sprintf(sbuf, "Jarak tempuh: %.0f m", distanceTraveled);
    glColor3f(0.7f, 0.9f, 1.0f);
    drawTextCentered(py + ph - 118, sbuf, GLUT_BITMAP_HELVETICA_12);

    float blink = sinf(startAnimTimer * 3.5f);

    if (blink > 0) {
        float alpha = 0.55f + blink * 0.45f;
        glColor4f(1, 1, 0.2f, alpha);
        drawTextCentered(py + 28, "[ SPACE = Main Lagi ]   [ R = Menu ]", GLUT_BITMAP_HELVETICA_12);
    }

    glDisable(GL_BLEND);

    end2D();
}