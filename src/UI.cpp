#include "UI.h"
#include "Graphics.h"
#include "Globals.h"
#include <GL/glut.h>
#include <cmath>
#include <cstdio>

void drawHUD(){
    begin2D();

    char buf[128];
    glColor3f(1,1,1);
    sprintf(buf,"Jarak: %.0f / %.0f m",distanceTraveled,targetDistance);
    drawText2D(20,windowHeight-30,buf);

    if(usePerspective){
        char ct[64]; sprintf(ct,"Perspektif (%d-Point)",cameraMode);
        drawText2D(20,windowHeight-55,ct);
    } else drawText2D(20,windowHeight-55,"Ortografi");

    // NOS bar
    float bx=20,by=windowHeight-95,bw=180,bh=16,fr=nosAmount/nosMax;
    glColor3f(0,.9f,1); drawText2D(bx,by+bh+4,"NOS [W]",GLUT_BITMAP_HELVETICA_12);
    glColor3f(.15f,.15f,.15f);
    glBegin(GL_QUADS); glVertex2f(bx,by); glVertex2f(bx+bw,by); glVertex2f(bx+bw,by+bh); glVertex2f(bx,by+bh); glEnd();
    if(nosActive&&nosAmount>0) glColor3f(1,1,.9f); else glColor3f(0,.85f,1);
    glBegin(GL_QUADS); glVertex2f(bx,by); glVertex2f(bx+bw*fr,by); glVertex2f(bx+bw*fr,by+bh); glVertex2f(bx,by+bh); glEnd();
    glColor3f(0,.9f,1);
    glBegin(GL_LINE_LOOP); glVertex2f(bx,by); glVertex2f(bx+bw,by); glVertex2f(bx+bw,by+bh); glVertex2f(bx,by+bh); glEnd();
    if(nosActive&&nosAmount>0){ glColor3f(1,1,.2f); drawText2D(bx+bw+10,by+2,"BOOST!",GLUT_BITMAP_HELVETICA_12); }
    if(nosAmount<=0)           { glColor3f(.6f,.6f,.6f); drawText2D(bx+bw+10,by+2,"EMPTY",GLUT_BITMAP_HELVETICA_12); }

    // Speed bar
    float sx=20,sy=windowHeight-130,sw=180,sh=12;
    float ratio=(currentSpeed-baseSpeed)/(maxSpeed-baseSpeed);
    if(ratio<0)ratio=0; if(ratio>1)ratio=1;
    glColor3f(1,.65f,0); drawText2D(sx,sy+sh+4,"SPEED",GLUT_BITMAP_HELVETICA_12);
    char spbuf[32]; sprintf(spbuf,"%d km/h",(int)(80+ratio*140));
    glColor3f(1,1,1); drawText2D(sx+sw+8,sy+2,spbuf,GLUT_BITMAP_HELVETICA_12);
    glColor3f(.15f,.15f,.15f);
    glBegin(GL_QUADS); glVertex2f(sx,sy); glVertex2f(sx+sw,sy); glVertex2f(sx+sw,sy+sh); glVertex2f(sx,sy+sh); glEnd();
    float sr=ratio*2.f; if(sr>1)sr=1;
    float sg=2*(1-ratio); if(sg>1)sg=1;
    glColor3f(sr,sg,0);
    glBegin(GL_QUADS); glVertex2f(sx,sy); glVertex2f(sx+sw*ratio,sy); glVertex2f(sx+sw*ratio,sy+sh); glVertex2f(sx,sy+sh); glEnd();
    glColor3f(1,.65f,0);
    glBegin(GL_LINE_LOOP); glVertex2f(sx,sy); glVertex2f(sx+sw,sy); glVertex2f(sx+sw,sy+sh); glVertex2f(sx,sy+sh); glEnd();

    glColor3f(.8f,.8f,.8f);
    drawText2D(20,50,"Arrow L/R = Pindah Lane   W = NOS   P/O = Proyeksi   1/2/3 = Kamera",GLUT_BITMAP_HELVETICA_12);
    drawText2D(20,32,"R = Restart ke Menu   ESC = Keluar",GLUT_BITMAP_HELVETICA_12);

    end2D();
}

// =====================================================================
// LAYAR START
// =====================================================================
void drawStartScreen(){
    begin2D();
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    float pw=460,ph=340;
    float px=(windowWidth-pw)*.5f, py=(windowHeight-ph)*.5f;

    glColor4f(0,0,0,0.45f);
    glBegin(GL_QUADS);
    glVertex2f(px+6,py-6); glVertex2f(px+pw+6,py-6);
    glVertex2f(px+pw+6,py+ph-6); glVertex2f(px+6,py+ph-6);
    glEnd();

    glColor4f(0.04f,0.06f,0.14f,0.88f);
    glBegin(GL_QUADS);
    glVertex2f(px,py); glVertex2f(px+pw,py); glVertex2f(px+pw,py+ph); glVertex2f(px,py+ph);
    glEnd();

    glLineWidth(2.5f);
    glColor4f(0.0f,0.85f,1.0f,0.9f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(px,py); glVertex2f(px+pw,py); glVertex2f(px+pw,py+ph); glVertex2f(px,py+ph);
    glEnd();
    glLineWidth(1.0f);
    glColor4f(0.0f,0.5f,0.7f,0.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(px+5,py+5); glVertex2f(px+pw-5,py+5); glVertex2f(px+pw-5,py+ph-5); glVertex2f(px+5,py+ph-5);
    glEnd();

    float lineY = py+ph-72;
    glLineWidth(1.5f);
    glColor4f(0.0f,0.85f,1.0f,0.6f);
    glBegin(GL_LINES);
    glVertex2f(px+20,lineY); glVertex2f(px+pw-20,lineY);
    glEnd();
    glLineWidth(1.0f);

    glColor3f(0.0f,0.92f,1.0f);
    drawTextCentered(py+ph-55,"3D DODGE CAR", GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(1.0f,0.85f,0.0f);
    drawTextCentered(py+ph-80,"NOS  EDITION", GLUT_BITMAP_HELVETICA_18);

    float colX = px + 30;
    float rowY  = py + ph - 118;
    float rowGap = 26;

    glColor3f(0.6f,0.9f,1.0f);
    drawText2D(colX, rowY,            "KONTROL :", GLUT_BITMAP_HELVETICA_12);
    glColor3f(1,1,1);
    drawText2D(colX, rowY-rowGap*1,   "[Panah Kiri / Kanan]  Pindah Lane", GLUT_BITMAP_HELVETICA_12);
    drawText2D(colX, rowY-rowGap*2,   "[W]                   Aktifkan NOS Boost", GLUT_BITMAP_HELVETICA_12);
    drawText2D(colX, rowY-rowGap*3,   "[P / O]               Perspektif / Ortografi", GLUT_BITMAP_HELVETICA_12);
    drawText2D(colX, rowY-rowGap*4,   "[1 / 2 / 3]           Ganti Sudut Kamera", GLUT_BITMAP_HELVETICA_12);
    drawText2D(colX, rowY-rowGap*5,   "[R]                   Restart ke Menu", GLUT_BITMAP_HELVETICA_12);
    drawText2D(colX, rowY-rowGap*6,   "[ESC]                 Keluar", GLUT_BITMAP_HELVETICA_12);

    float line2Y = py + 54;
    glLineWidth(1.5f);
    glColor4f(0.0f,0.85f,1.0f,0.4f);
    glBegin(GL_LINES);
    glVertex2f(px+20,line2Y); glVertex2f(px+pw-20,line2Y);
    glEnd();
    glLineWidth(1.0f);

    float blink = sinf(startAnimTimer * 3.5f);
    if(blink > 0){
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
void drawEndScreen(){
    begin2D();
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    float pw=340, ph=180;
    float px=(windowWidth-pw)*.5f, py=(windowHeight-ph)*.5f;

    glColor4f(0.04f,0.06f,0.14f,0.88f);
    glBegin(GL_QUADS);
    glVertex2f(px,py); glVertex2f(px+pw,py); glVertex2f(px+pw,py+ph); glVertex2f(px,py+ph);
    glEnd();

    glLineWidth(2.5f);
    if(gameState==STATE_OVER) glColor4f(1.0f,0.2f,0.2f,1.0f);
    else                      glColor4f(0.2f,1.0f,0.4f,1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(px,py); glVertex2f(px+pw,py); glVertex2f(px+pw,py+ph); glVertex2f(px,py+ph);
    glEnd();
    glLineWidth(1.0f);

    if(gameState==STATE_OVER){
        glColor3f(1.0f,0.25f,0.25f);
        drawTextCentered(py+ph-50,"GAME  OVER !",GLUT_BITMAP_TIMES_ROMAN_24);
        glColor3f(.9f,.9f,.9f);
        drawTextCentered(py+ph-85,"Mobil kamu bertabrakan !",GLUT_BITMAP_HELVETICA_18);
    } else {
        glColor3f(0.2f,1.0f,0.4f);
        drawTextCentered(py+ph-50,"YOU  WIN !",GLUT_BITMAP_TIMES_ROMAN_24);
        glColor3f(.9f,.9f,.9f);
        drawTextCentered(py+ph-85,"Finish line tercapai !",GLUT_BITMAP_HELVETICA_18);
    }

    char sbuf[64];
    sprintf(sbuf,"Jarak tempuh: %.0f m",distanceTraveled);
    glColor3f(.7f,.9f,1.f);
    drawTextCentered(py+ph-118,sbuf,GLUT_BITMAP_HELVETICA_12);

    float blink=sinf(startAnimTimer*3.5f);
    if(blink>0){
        float alpha=.55f+blink*.45f;
        glColor4f(1,1,.2f,alpha);
        drawTextCentered(py+28,"[ SPACE = Main Lagi ]   [ R = Menu ]",GLUT_BITMAP_HELVETICA_12);
    }

    glDisable(GL_BLEND);
    end2D();
}

