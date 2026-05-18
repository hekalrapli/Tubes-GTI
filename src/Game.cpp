#include "Game.h"
#include "Globals.h"
#include "Graphics.h"
#include "Objects.h"
#include "UI.h"
#include "Environment.h"
#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>

void startGame() {
    enemies.clear();
    nosPickups.clear();
    playerLane       = 1;
    playerX          = lanePos[1];
    playerTargetX    = lanePos[1];
    playerRoll       = 0.0f;
    distanceTraveled = 0.0f;
    spawnCooldown    = 0;
    gameOver         = false;
    winGame          = false;
    roadAnim         = 0.0f;
    nosAmount        = 0.0f;
    nosActive        = false;
    nosSpawnTimer    = 0.0f;
    currentSpeed     = baseSpeed;
    targetDistance   = 1000.0f + (rand() % 2001) * 1.0f;
    wheelRotAngle      = 0.0f;
    wheelRotAngleEnemy = 0.0f;
    crashTimer         = 0.0f;
    crashEnemyIndex    = -1;
    crashImpactX       = 0.0f;
    crashImpactZ       = 0.0f;
    crashPlayerOffsetX = 0.0f;
    crashPlayerOffsetZ = 0.0f;
    crashEnemyOffsetX  = 0.0f;
    crashEnemyOffsetZ  = 0.0f;
    crashImpulseX      = 1.0f;
    crashSpinAngle     = 0.0f;
    crashFlashAlpha    = 0.0f;
    gameState          = STATE_PLAYING;
}

void resetToStart() {
    gameState      = STATE_START;
    startAnimTimer = 0.0f;
}

bool checkCollision(float ex, float ez) {
    return (fabs(playerX-ex)<1.3f && fabs(playerZ-ez)<2.2f);
}
bool checkNosCollision(float nx, float nz) {
    return (fabs(playerX-nx)<2.0f && fabs(playerZ-nz)<2.5f);
}

void startCrashInteraction(int enemyIndex) {
    crashTimer         = 0.0f;
    crashEnemyIndex    = enemyIndex;
    crashImpactX       = (playerX + enemies[enemyIndex].x) * 0.5f;
    crashImpactZ       = (playerZ + enemies[enemyIndex].z) * 0.5f;
    crashPlayerOffsetX = 0.0f;
    crashPlayerOffsetZ = 0.0f;
    crashEnemyOffsetX  = 0.0f;
    crashEnemyOffsetZ  = 0.0f;
    crashSpinAngle     = 0.0f;
    crashFlashAlpha    = 1.0f;

    float dx = playerX - enemies[enemyIndex].x;
    if(fabs(dx) < 0.05f) dx = (playerLane <= 1) ? -1.0f : 1.0f;
    crashImpulseX = (dx < 0.0f) ? -1.0f : 1.0f;

    nosActive = false;
    gameState = STATE_CRASH;
    startAnimTimer = 0.0f;
}

void drawCrashEffect() {
    if(gameState != STATE_CRASH) return;

    float progress = crashTimer / crashDuration;
    if(progress < 0.0f) progress = 0.0f;
    if(progress > 1.0f) progress = 1.0f;
    float alpha = 1.0f - progress;

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glTranslatef(crashImpactX, 0.75f, crashImpactZ);

    glColor4f(1.0f, 0.35f, 0.05f, 0.35f * alpha);
    glutSolidSphere(0.45f + progress * 1.6f, 18, 10);

    glLineWidth(3.0f);
    glBegin(GL_LINES);
    for(int i = 0; i < 16; i++) {
        float a = i * 3.14159f * 2.0f / 16.0f;
        float inner = 0.25f + progress * 0.25f;
        float outer = 1.00f + progress * 2.10f;
        float yOff  = ((i % 3) - 1) * 0.22f;
        glColor4f(1.0f, 0.85f, 0.1f, alpha);
        glVertex3f(cosf(a) * inner, yOff, sinf(a) * inner);
        glVertex3f(cosf(a) * outer, yOff + 0.15f, sinf(a) * outer);
    }
    glEnd();
    glLineWidth(1.0f);

    glPopMatrix();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void drawCrashOverlay() {
    begin2D();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 0.0f, 0.0f, crashFlashAlpha * 0.22f);
    glBegin(GL_QUADS);
    glVertex2f(0,0); glVertex2f(windowWidth,0);
    glVertex2f(windowWidth,windowHeight); glVertex2f(0,windowHeight);
    glEnd();

    glColor4f(1.0f, 0.82f, 0.15f, 0.85f);
    drawTextCentered(windowHeight * 0.58f, "TABRAKAN ANTAR MOBIL!", GLUT_BITMAP_TIMES_ROMAN_24);

    glDisable(GL_BLEND);
    end2D();
}

void drawScene3D(float camRotY){
    setProjection();
    setCamera(camRotY);
    setupLighting();
    drawSun(); drawRoad();

    for(int i=0;i<(int)trees.size();i++){ glPushMatrix(); glTranslatef(trees[i].x,0,trees[i].z); drawTreeShadow(trees[i]); glPopMatrix(); }
    for(int i=0;i<(int)buildings.size();i++){ glPushMatrix(); glTranslatef(buildings[i].x,0,buildings[i].z); drawBuildingShadow(buildings[i]); glPopMatrix(); }

    float playerDrawX = playerX;
    float playerDrawZ = playerZ;
    if(gameState == STATE_CRASH) {
        playerDrawX += crashPlayerOffsetX;
        playerDrawZ += crashPlayerOffsetZ;
    }
    drawCarShadow(playerDrawX, playerDrawZ);

    for(int i=0;i<(int)enemies.size();i++){
        float enemyDrawX = enemies[i].x;
        float enemyDrawZ = enemies[i].z;
        if(gameState == STATE_CRASH && i == crashEnemyIndex) {
            enemyDrawX += crashEnemyOffsetX;
            enemyDrawZ += crashEnemyOffsetZ;
        }
        drawCarShadow(enemyDrawX, enemyDrawZ);
    }

    for(int i=0;i<(int)trees.size();i++){ glPushMatrix(); glTranslatef(trees[i].x,-.6f,trees[i].z); drawTree(trees[i]); glPopMatrix(); }
    for(int i=0;i<(int)buildings.size();i++){ glPushMatrix(); glTranslatef(buildings[i].x,-.6f,buildings[i].z); drawBuilding(buildings[i]); glPopMatrix(); }

    glPushMatrix();
    glTranslatef(playerDrawX,0,playerDrawZ);
    if(gameState == STATE_CRASH) {
        glRotatef(-crashSpinAngle * 0.22f, 0, 1, 0);
        glRotatef(crashImpulseX * crashSpinAngle * 0.12f, 0, 0, 1);
    }
    drawPlayerCar();
    glPopMatrix();

    for(int i=0;i<(int)enemies.size();i++){
        float enemyDrawX = enemies[i].x;
        float enemyDrawZ = enemies[i].z;
        glPushMatrix();
        if(gameState == STATE_CRASH && i == crashEnemyIndex) {
            enemyDrawX += crashEnemyOffsetX;
            enemyDrawZ += crashEnemyOffsetZ;
            glTranslatef(enemyDrawX,0,enemyDrawZ);
            glRotatef(crashImpulseX * crashSpinAngle, 0, 1, 0);
            glRotatef(-crashImpulseX * crashSpinAngle * 0.25f, 0, 0, 1);
        } else {
            glTranslatef(enemyDrawX,0,enemyDrawZ);
        }
        drawEnemyCar();
        glPopMatrix();
    }

    drawCrashEffect();

    for(int i=0;i<(int)nosPickups.size();i++){
        if(!nosPickups[i].active) continue;
        float bobY=.3f+sinf(nosPickups[i].bobTimer)*.2f;
        glPushMatrix(); glTranslatef(nosPickups[i].x,bobY,nosPickups[i].z); drawNosShadow(); glPopMatrix();
        glPushMatrix(); glTranslatef(nosPickups[i].x,bobY,nosPickups[i].z); drawNosObject(nosPickups[i].rotAngle); glPopMatrix();
    }

    if(gameState==STATE_PLAYING || gameState==STATE_CRASH){
        float fz=playerZ-(targetDistance-distanceTraveled)/5.f;
        if(fz>-90) drawFinishLine(fz);
    }
}

// =====================================================================
// DISPLAY
// =====================================================================
void display(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if(gameState==STATE_START){
        drawScene3D(startAnimTimer * 12.0f);
        begin2D();
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0,0,0,0.25f);
        glBegin(GL_QUADS); glVertex2f(0,0); glVertex2f(windowWidth,0); glVertex2f(windowWidth,windowHeight); glVertex2f(0,windowHeight); glEnd();
        glDisable(GL_BLEND); end2D();
        drawStartScreen();
    }
    else if(gameState==STATE_PLAYING){
        drawScene3D();
        drawHUD();
    }
    else if(gameState==STATE_CRASH){
        drawScene3D();
        drawHUD();
        drawCrashOverlay();
    }
    else {
        drawScene3D();
        drawHUD();
        drawEndScreen();
    }

    glutSwapBuffers();
}

// =====================================================================
// UPDATE
// =====================================================================
void update(int value){
    startAnimTimer += 0.016f;

    if(gameState==STATE_PLAYING){
        currentSpeed=baseSpeed+distanceTraveled*speedIncrement;
        if(currentSpeed>maxSpeed) currentSpeed=maxSpeed;
        float speed=currentSpeed;
        if(nosActive&&nosAmount>0){ speed+=nosBoostSpeed; nosAmount-=nosDrainRate; if(nosAmount<0)nosAmount=0; }

        // ---- Animasi roda: kecepatan sudut sesuai kecepatan maju ----
        // Keliling roda ~2*pi*0.32 = 2.01 unit; konversi ke derajat per frame
        float wheelCircumference = 2.0f * 3.14159f * 0.32f;
        float degPerFrame = (speed / wheelCircumference) * 360.0f;
        wheelRotAngle      += degPerFrame; if(wheelRotAngle      >= 360.f) wheelRotAngle      -= 360.f;
        wheelRotAngleEnemy += degPerFrame; if(wheelRotAngleEnemy >= 360.f) wheelRotAngleEnemy -= 360.f;

        distanceTraveled+=speed*5;
        roadAnim+=speed; if(roadAnim>4)roadAnim=0;

        float dx=playerTargetX-playerX;
        playerX+=dx*laneChangeSpeed;
        if(fabs(dx)<.001f) playerX=playerTargetX;
        float rollTarget=dx*7.5f;
        if(rollTarget> 18)rollTarget= 18;
        if(rollTarget<-18)rollTarget=-18;
        playerRoll+=(rollTarget-playerRoll)*.2f;

        bool collisionTriggered = false;
        for(int i=0;i<(int)enemies.size();i++){
            enemies[i].z+=speed;
            if(checkCollision(enemies[i].x,enemies[i].z)){
                startCrashInteraction(i);
                collisionTriggered = true;
                break;
            }
        }

        if(!collisionTriggered){
            for(int i=0;i<(int)enemies.size();){ if(enemies[i].z>15) enemies.erase(enemies.begin()+i); else i++; }

            for(int i=0;i<(int)nosPickups.size();i++){
            if(!nosPickups[i].active) continue;
            nosPickups[i].rotAngle+=4.5f; if(nosPickups[i].rotAngle>=360)nosPickups[i].rotAngle-=360;
            nosPickups[i].bobTimer+=.07f;
            nosPickups[i].z+=speed;
            if(checkNosCollision(nosPickups[i].x,nosPickups[i].z)){ nosAmount+=40; if(nosAmount>nosMax)nosAmount=nosMax; nosPickups[i].active=false; }
            if(nosPickups[i].z>15)nosPickups[i].active=false;
        }
        for(int i=0;i<(int)nosPickups.size();){ if(!nosPickups[i].active)nosPickups.erase(nosPickups.begin()+i); else i++; }

        for(int i=0;i<(int)trees.size();i++){
            trees[i].z+=speed;
            if(trees[i].z>25){ trees[i].z-=180; trees[i].trunkH=1.5f+(rand()%10)*.15f; trees[i].crownR=.9f+(rand()%6)*.12f; trees[i].seed=rand()%10; }
        }
        for(int i=0;i<(int)buildings.size();i++){
            buildings[i].z+=speed;
            if(buildings[i].z>25){
                buildings[i].z-=170;
                float bc[5][3]={{.75f,.70f,.60f},{.60f,.55f,.50f},{.80f,.78f,.72f},{.55f,.60f,.65f},{.70f,.65f,.58f}};
                int ci=rand()%5; buildings[i].h=3+(rand()%8)*.7f;
                buildings[i].r=bc[ci][0];buildings[i].g=bc[ci][1];buildings[i].b=bc[ci][2];
            }
        }

        nosSpawnTimer+=speed*5; if(nosSpawnTimer>=nosSpawnInterval){ nosSpawnTimer=0; if(distanceTraveled<targetDistance-300)spawnNos(); }
            if(spawnCooldown>0) spawnCooldown--;
            else if(distanceTraveled<targetDistance-250&&rand()%25==0){ spawnEnemy(); spawnCooldown=45; }
            if(distanceTraveled>=targetDistance){ gameState=STATE_WIN; startAnimTimer=0; }
        }
    }
    else if(gameState==STATE_CRASH){
        crashTimer += 0.016f;
        float progress = crashTimer / crashDuration;
        if(progress < 0.0f) progress = 0.0f;
        if(progress > 1.0f) progress = 1.0f;
        float decay = 1.0f - progress;

        float shake = sinf(crashTimer * 70.0f) * 0.10f * decay;
        crashPlayerOffsetX = -crashImpulseX * 0.45f * progress + shake;
        crashPlayerOffsetZ =  0.28f * progress;
        crashEnemyOffsetX  =  crashImpulseX * 0.70f * progress - shake;
        crashEnemyOffsetZ  = -0.42f * progress;
        crashSpinAngle     =  60.0f * progress;
        crashFlashAlpha    =  decay;

        wheelRotAngle      += 0.8f; if(wheelRotAngle      >= 360.f) wheelRotAngle      -= 360.f;
        wheelRotAngleEnemy += 0.8f; if(wheelRotAngleEnemy >= 360.f) wheelRotAngleEnemy -= 360.f;

        if(crashTimer >= crashDuration){
            gameState = STATE_OVER;
            startAnimTimer = 0.0f;
        }
    }
    else if(gameState==STATE_START){
        // Roda berputar pelan di layar start
        wheelRotAngle      += 1.5f; if(wheelRotAngle      >= 360.f) wheelRotAngle      -= 360.f;
        wheelRotAngleEnemy += 1.5f; if(wheelRotAngleEnemy >= 360.f) wheelRotAngleEnemy -= 360.f;

        roadAnim+=0.05f; if(roadAnim>4)roadAnim=0;
        for(int i=0;i<(int)trees.size();i++){
            trees[i].z+=0.05f;
            if(trees[i].z>25){ trees[i].z-=180; trees[i].trunkH=1.5f+(rand()%10)*.15f; trees[i].crownR=.9f+(rand()%6)*.12f; trees[i].seed=rand()%10; }
        }
        for(int i=0;i<(int)buildings.size();i++){
            buildings[i].z+=0.05f;
            if(buildings[i].z>25){
                buildings[i].z-=170;
                float bc[5][3]={{.75f,.70f,.60f},{.60f,.55f,.50f},{.80f,.78f,.72f},{.55f,.60f,.65f},{.70f,.65f,.58f}};
                int ci=rand()%5; buildings[i].h=3+(rand()%8)*.7f;
                buildings[i].r=bc[ci][0];buildings[i].g=bc[ci][1];buildings[i].b=bc[ci][2];
            }
        }
    }
    else {
        // STATE_OVER / STATE_WIN — roda terus berputar pelan
        wheelRotAngle      += 0.8f; if(wheelRotAngle      >= 360.f) wheelRotAngle      -= 360.f;
        wheelRotAngleEnemy += 0.8f; if(wheelRotAngleEnemy >= 360.f) wheelRotAngleEnemy -= 360.f;
    }

    glutPostRedisplay();
    glutTimerFunc(16,update,0);
}

// =====================================================================
// INPUT
// =====================================================================
void specialKeyboard(int key,int x,int y){
    if(gameState!=STATE_PLAYING) return;
    if(key==GLUT_KEY_LEFT  && playerLane>0){ playerLane--; playerTargetX=lanePos[playerLane]; }
    if(key==GLUT_KEY_RIGHT && playerLane<2){ playerLane++; playerTargetX=lanePos[playerLane]; }
}

void keyboard(unsigned char key,int x,int y){
    if(key==' '){
        if(gameState==STATE_START||gameState==STATE_OVER||gameState==STATE_WIN)
            startGame();
        return;
    }
    if(key=='r'||key=='R'){
        resetToStart();
        return;
    }
    if(gameState!=STATE_PLAYING) return;
    if(key=='p'||key=='P') usePerspective=true;
    else if(key=='o'||key=='O') usePerspective=false;
    else if(key=='1') cameraMode=1;
    else if(key=='2') cameraMode=2;
    else if(key=='3') cameraMode=3;
    else if(key=='w'||key=='W') nosActive=true;
    else if(key==27) exit(0);
}

void keyboardUp(unsigned char key,int x,int y){
    if(key=='w'||key=='W') nosActive=false;
}

void reshape(int w,int h){
    if(h==0)h=1;
    windowWidth=w; windowHeight=h;
    glViewport(0,0,w,h);
    setProjection();
}

void init(){
    glClearColor(.45f,.7f,1,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    setupLighting();
    srand((unsigned)time(NULL));
    playerX=playerTargetX=lanePos[1];
    initEnvironment();
    gameState=STATE_START;
}
