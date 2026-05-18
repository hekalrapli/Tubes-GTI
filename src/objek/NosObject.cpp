#include "objek/NosObject.h"
#include "Graphics.h"
#include "Globals.h"
#include <GL/glut.h>

void drawNosObject(float rotAngle){
    glPushMatrix();
    glRotatef(rotAngle,0,1,0); glRotatef(45,0,0,1); glTranslatef(0,-.5f,0); glRotatef(-90,1,0,0);
    setMaterial(0,.9f,1.f);
    { GLUquadric* q=gluNewQuadric(); gluCylinder(q,.35f,.35f,1,16,4); gluDeleteQuadric(q); }
    setMaterial(0,.7f,.9f);
    glPushMatrix(); glRotatef(180,1,0,0); { GLUquadric* q=gluNewQuadric(); gluDisk(q,0,.35f,16,1); gluDeleteQuadric(q); } glPopMatrix();
    glPushMatrix(); glTranslatef(0,0,1); { GLUquadric* q=gluNewQuadric(); gluDisk(q,0,.35f,16,1); gluDeleteQuadric(q); } glPopMatrix();
    setMaterial(1,.8f,0);
    glPushMatrix(); glTranslatef(0,0,1); { GLUquadric* q=gluNewQuadric(); gluCylinder(q,.2f,.05f,.25f,12,2); gluDeleteQuadric(q); } glPopMatrix();
    setMaterial(1,1,1);
    glPushMatrix(); glTranslatef(0,0,.46f); { GLUquadric* q=gluNewQuadric(); gluCylinder(q,.37f,.37f,.08f,16,1); gluDeleteQuadric(q); } glPopMatrix();
    glPopMatrix();
}
void drawNosShadow(){
    float m[16]; getShadowMatrix(m,SHADOW_Y,20,25,-20);
    glDisable(GL_LIGHTING); glColor4f(0,0,0,.25f);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix(); glMultMatrixf(m); glutSolidSphere(.4f,8,6); glPopMatrix();
    glDisable(GL_BLEND); glEnable(GL_LIGHTING);
}
