#include "objek/TreeObject.h"
#include "Graphics.h"
#include "Globals.h"
#include <GL/glut.h>

void drawTree(const Tree& t){
    setMaterial(.45f+t.seed*.01f,.28f,.10f);
    glPushMatrix(); glRotatef(-90,1,0,0);
    GLUquadric* q=gluNewQuadric(); gluCylinder(q,.18f,.12f,t.trunkH,8,2); gluDeleteQuadric(q);
    glPopMatrix();
    float gr=.15f+(t.seed%3)*.12f,gg=.55f+(t.seed%4)*.07f,gb=.10f+(t.seed%2)*.06f;
    setMaterial(gr,gg,gb);
    glPushMatrix(); glTranslatef(0,t.trunkH,0); glutSolidSphere(t.crownR,10,8); glPopMatrix();
    glPushMatrix(); glTranslatef(0,t.trunkH+t.crownR*.5f,0); glutSolidSphere(t.crownR*.65f,10,8); glPopMatrix();
}
void drawTreeShadow(const Tree& t){
    float m[16]; getShadowMatrix(m,SHADOW_Y,20,25,-20);
    glDisable(GL_LIGHTING); glColor4f(0,0,0,.35f);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix(); glMultMatrixf(m);
    glPushMatrix(); glRotatef(-90,1,0,0);
    GLUquadric* q=gluNewQuadric(); gluCylinder(q,.18f,.12f,t.trunkH,8,2); gluDeleteQuadric(q); glPopMatrix();
    glPushMatrix(); glTranslatef(0,t.trunkH,0); glutSolidSphere(t.crownR,10,8); glPopMatrix();
    glPushMatrix(); glTranslatef(0,t.trunkH+t.crownR*.5f,0); glutSolidSphere(t.crownR*.65f,10,8); glPopMatrix();
    glPopMatrix(); glDisable(GL_BLEND); glEnable(GL_LIGHTING);
}
