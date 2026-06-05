#include "objek/Road.h"
#include "Graphics.h"
#include "Globals.h"
#include <GL/glut.h>

// drawRoad sekarang menerima offset Z (posisi player) agar jalan
// selalu ter-render di sekitar kamera, bukan di koordinat absolut.
// Tapi karena drawRoad() dipanggil dari dalam matrix yg sudah di-push
// di scene normal (koordinat kamera = 0), kita cukup extend jauh ke belakang.
// Untuk pause mode, road juga masih benar karena jalan di-render relatif ke world.

void drawRoad(){
    // Aspal — sangat panjang ke segala arah
    glPushMatrix(); setMaterial(.2f,.2f,.2f);
    glBegin(GL_QUADS); glNormal3f(0,1,0);
    glVertex3f(-4.5f,-.6f,-500); glVertex3f(4.5f,-.6f,-500);
    glVertex3f(4.5f,-.6f, 100); glVertex3f(-4.5f,-.6f, 100);
    glEnd(); glPopMatrix();

    // Rumput kiri
    glPushMatrix(); setMaterial(.15f,.45f,.1f);
    glBegin(GL_QUADS); glNormal3f(0,1,0);
    glVertex3f(-500,-.61f,-500); glVertex3f(-4.5f,-.61f,-500);
    glVertex3f(-4.5f,-.61f, 100); glVertex3f(-500,-.61f, 100);
    glEnd(); glPopMatrix();

    // Rumput kanan
    glPushMatrix(); setMaterial(.15f,.45f,.1f);
    glBegin(GL_QUADS); glNormal3f(0,1,0);
    glVertex3f(4.5f,-.61f,-500); glVertex3f(500,-.61f,-500);
    glVertex3f(500,-.61f, 100); glVertex3f(4.5f,-.61f, 100);
    glEnd(); glPopMatrix();

    // Marka tepi kiri
    glPushMatrix(); setMaterial(1,1,1);
    glBegin(GL_QUADS); glNormal3f(0,1,0);
    glVertex3f(-4.2f,-.59f,-500); glVertex3f(-4,-.59f,-500);
    glVertex3f(-4,-.59f, 100); glVertex3f(-4.2f,-.59f, 100);
    glEnd(); glPopMatrix();

    // Marka tepi kanan
    glPushMatrix(); setMaterial(1,1,1);
    glBegin(GL_QUADS); glNormal3f(0,1,0);
    glVertex3f(4,-.59f,-500); glVertex3f(4.2f,-.59f,-500);
    glVertex3f(4.2f,-.59f, 100); glVertex3f(4,-.59f, 100);
    glEnd(); glPopMatrix();

    // Marka tengah putus-putus kuning — tile animasi
    for(int i=0;i<25;i++){
        float z=-80.f+i*4.f+roadAnim;
        glPushMatrix(); setMaterial(1,1,0);
        glBegin(GL_QUADS); glNormal3f(0,1,0);
        glVertex3f(-.15f,-.58f,z); glVertex3f(.15f,-.58f,z);
        glVertex3f(.15f,-.58f,z+2); glVertex3f(-.15f,-.58f,z+2);
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
        glVertex3f(x0,0,z0); glVertex3f(x0+.5f,0,z0);
        glVertex3f(x0+.5f,0,z0+.5f); glVertex3f(x0,0,z0+.5f);
        glEnd();
    }
    glPopMatrix();
}
