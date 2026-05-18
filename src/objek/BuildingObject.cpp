#include "objek/BuildingObject.h"
#include "Graphics.h"
#include "Globals.h"
#include <GL/glut.h>

void drawBuilding(const Building& b){
    setMaterial(b.r,b.g,b.b);
    glPushMatrix(); glScalef(b.w,b.h,b.d); glTranslatef(0,.5f,0); glutSolidCube(1); glPopMatrix();
    setMaterial(b.r*.7f,b.g*.7f,b.b*.7f);
    glPushMatrix(); glTranslatef(0,b.h,0); glScalef(b.w,.2f,b.d); glTranslatef(0,.5f,0); glutSolidCube(1); glPopMatrix();
    setMaterial(.6f,.85f,1.f,80.f);
    int rows=(int)(b.h/1.2f),cols=(int)(b.w/.9f);
    for(int r=0;r<rows;r++) for(int c=0;c<cols;c++){
        float wx=-b.w*.5f+(c+.5f)*(b.w/cols),wy=.5f+r*1.2f,wz=b.d*.5f+.01f;
        glPushMatrix(); glTranslatef(wx,wy,wz); glScalef(.3f,.4f,.05f); glutSolidCube(1); glPopMatrix();
    }
}
void drawBuildingShadow(const Building& b){
    float m[16]; getShadowMatrix(m,SHADOW_Y,20,25,-20);
    glDisable(GL_LIGHTING); glColor4f(0,0,0,.3f);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix(); glMultMatrixf(m);
    glPushMatrix(); glScalef(b.w,b.h,b.d); glTranslatef(0,.5f,0); glutSolidCube(1); glPopMatrix();
    glPopMatrix(); glDisable(GL_BLEND); glEnable(GL_LIGHTING);
}
