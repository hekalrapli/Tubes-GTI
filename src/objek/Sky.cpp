#include "objek/Sky.h"
#include <GL/glut.h>

void drawSun(){
    glPushMatrix(); glTranslatef(20,25,-20);
    glDisable(GL_LIGHTING); glColor3f(1,.9f,0); glutSolidSphere(3,32,32);
    glEnable(GL_LIGHTING); glPopMatrix();
}
