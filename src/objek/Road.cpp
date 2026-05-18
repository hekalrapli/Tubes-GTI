#include "objek/Road.h"
#include "Graphics.h"
#include "Globals.h"
#include <GL/glut.h>

void drawRoad(){
    glPushMatrix(); setMaterial(.2f,.2f,.2f);
    glBegin(GL_QUADS); glNormal3f(0,1,0);
    glVertex3f(-4.5f,-.6f,-80); glVertex3f(4.5f,-.6f,-80); glVertex3f(4.5f,-.6f,20); glVertex3f(-4.5f,-.6f,20);
    glEnd(); glPopMatrix();
    glPushMatrix(); setMaterial(.15f,.45f,.1f);
    glBegin(GL_QUADS); glNormal3f(0,1,0);
    glVertex3f(-14,-.61f,-80); glVertex3f(-4.5f,-.61f,-80); glVertex3f(-4.5f,-.61f,20); glVertex3f(-14,-.61f,20);
    glEnd(); glPopMatrix();
    glPushMatrix(); setMaterial(.15f,.45f,.1f);
    glBegin(GL_QUADS); glNormal3f(0,1,0);
    glVertex3f(4.5f,-.61f,-80); glVertex3f(14,-.61f,-80); glVertex3f(14,-.61f,20); glVertex3f(4.5f,-.61f,20);
    glEnd(); glPopMatrix();
    glPushMatrix(); setMaterial(1,1,1);
    glBegin(GL_QUADS); glNormal3f(0,1,0);
    glVertex3f(-4.2f,-.59f,-80); glVertex3f(-4,-.59f,-80); glVertex3f(-4,-.59f,20); glVertex3f(-4.2f,-.59f,20);
    glEnd(); glPopMatrix();
    glPushMatrix(); setMaterial(1,1,1);
    glBegin(GL_QUADS); glNormal3f(0,1,0);
    glVertex3f(4,-.59f,-80); glVertex3f(4.2f,-.59f,-80); glVertex3f(4.2f,-.59f,20); glVertex3f(4,-.59f,20);
    glEnd(); glPopMatrix();
    for(int i=0;i<25;i++){
        float z=-80.f+i*4.f+roadAnim;
        glPushMatrix(); setMaterial(1,1,0);
        glBegin(GL_QUADS); glNormal3f(0,1,0);
        glVertex3f(-.15f,-.58f,z); glVertex3f(.15f,-.58f,z); glVertex3f(.15f,-.58f,z+2); glVertex3f(-.15f,-.58f,z+2);
        glEnd(); glPopMatrix();
    }
}
void drawFinishLine(float zStart){
    glPushMatrix(); glTranslatef(0,-.57f,zStart);
    glTranslatef(-4.5f,0,0);
    for(int r=0;r<4;r++) for(int c=0;c<18;c++){
        if((r+c)%2==0) setMaterial(1,1,1); else setMaterial(.1f,.1f,.1f);
        float x0=c*.5f,z0=r*.5f;
        glBegin(GL_QUADS); glNormal3f(0,1,0);
        glVertex3f(x0,0,z0); glVertex3f(x0+.5f,0,z0); glVertex3f(x0+.5f,0,z0+.5f); glVertex3f(x0,0,z0+.5f);
        glEnd();
    }
    glPopMatrix();
}
