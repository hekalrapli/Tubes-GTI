#include "Graphics.h"
#include "Globals.h"
#include <cmath>
#include <cstdio>
#include <vector>

void getShadowMatrix(float m[16], float planeY, float lx, float ly, float lz) {
    float A=0,B=1,C=0,D=-planeY;
    float dot=A*lx+B*ly+C*lz+D;
    m[0]=dot-lx*A; m[4]=   -lx*B; m[8] =   -lx*C; m[12]=   -lx*D;
    m[1]=   -ly*A; m[5]=dot-ly*B; m[9] =   -ly*C; m[13]=   -ly*D;
    m[2]=   -lz*A; m[6]=   -lz*B; m[10]=dot-lz*C; m[14]=   -lz*D;
    m[3]=     -A;  m[7]=     -B;  m[11]=     -C;  m[15]=dot-   D;
}

// =====================================================================
// TEXT HELPERS
// =====================================================================
void drawText2D(float x, float y, const char* text, void* font) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i]; i++)
        glutBitmapCharacter(font, text[i]);
}

void drawTextCentered(float y, const char* text, void* font) {
    int len = 0;
    for (int i = 0; text[i]; i++) len += glutBitmapWidth(font, text[i]);
    drawText2D((windowWidth - len) * 0.5f, y, text, font);
}

void begin2D() {
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
}
void end2D() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// =====================================================================
// PROJECTION, CAMERA, LIGHTING, MATERIAL
// =====================================================================
void setProjection() {
    float asp=(windowHeight==0)?1.f:(float)windowWidth/windowHeight;
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    if(usePerspective) gluPerspective(60.0,asp,1.0,200.0);
    else               glOrtho(-8.*asp,8.*asp,-2.,12.,-100.,100.);
    glMatrixMode(GL_MODELVIEW);
}

void setCamera(float extraRotY) {
    if(usePerspective){
        if(cameraMode==1) gluLookAt(0,2,15,  0,2,0, 0,1,0);
        else if(cameraMode==2) gluLookAt(12,2,12, 0,2,0, 0,1,0);
        else {
            float cx = 15.f * sinf(extraRotY * 3.14159f / 180.f);
            float cz = 15.f * cosf(extraRotY * 3.14159f / 180.f);
            gluLookAt(cx,12,cz, 0,0,0, 0,1,0);
        }
    } else {
        gluLookAt(10,10,10, 0,0,0, 0,1,0);
    }
}

void setupLighting() {
    GLfloat lp[]={20,25,-20,1}, am[]={.72f,.72f,.72f,1}, di[]={0.85f,0.85f,0.78f,1}, sp[]={0.5f,0.5f,0.5f,1};
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_POSITION,lp);
    glLightfv(GL_LIGHT0,GL_AMBIENT,am);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,di);
    glLightfv(GL_LIGHT0,GL_SPECULAR,sp);
}

// Lighting untuk mode pause: ambient penuh, tidak ada bayangan keras
void setupPauseLighting() {
    GLfloat am[]={0.9f,0.9f,0.9f,1}, di[]={0.5f,0.5f,0.45f,1}, sp[]={0.2f,0.2f,0.2f,1};
    // Posisi cahaya di atas langsung (directional dari atas)
    GLfloat lp[]={0,1,0,0};
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_POSITION,lp);
    glLightfv(GL_LIGHT0,GL_AMBIENT,am);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,di);
    glLightfv(GL_LIGHT0,GL_SPECULAR,sp);
}

void setMaterial(float r,float g,float b,float sh){
    GLfloat am[]={r*.3f,g*.3f,b*.3f,1},di[]={r,g,b,1},sp[]={.8f,.8f,.8f,1},s[]={sh};
    glMaterialfv(GL_FRONT,GL_AMBIENT,am);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,di);
    glMaterialfv(GL_FRONT,GL_SPECULAR,sp);
    glMaterialfv(GL_FRONT,GL_SHININESS,s);
}


GLuint loadBMPTexture(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Gagal membuka file BMP: %s\n", filename);
        return 0;
    }

    unsigned char header[54];

    if (fread(header, 1, 54, file) != 54) {
        printf("File BMP tidak valid: %s\n", filename);
        fclose(file);
        return 0;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        printf("Bukan file BMP: %s\n", filename);
        fclose(file);
        return 0;
    }

    int dataPos     = *(int*)&(header[0x0A]);
    int width       = *(int*)&(header[0x12]);
    int height      = *(int*)&(header[0x16]);
    short bpp       = *(short*)&(header[0x1C]);
    int compression = *(int*)&(header[0x1E]);

    if (bpp != 24 || compression != 0) {
        printf("BMP harus 24-bit dan uncompressed: %s\n", filename);
        fclose(file);
        return 0;
    }

    bool bottomUp = height > 0;
    int absHeight = height > 0 ? height : -height;

    int rowPadded = (width * 3 + 3) & (~3);

    std::vector<unsigned char> rawData(rowPadded * absHeight);
    std::vector<unsigned char> rgbData(width * absHeight * 3);

    fseek(file, dataPos, SEEK_SET);
    fread(rawData.data(), 1, rowPadded * absHeight, file);
    fclose(file);

    for (int y = 0; y < absHeight; y++) {
        int srcY = bottomUp ? (absHeight - 1 - y) : y;

        for (int x = 0; x < width; x++) {
            int srcIndex = srcY * rowPadded + x * 3;
            int dstIndex = (y * width + x) * 3;

            // BMP menyimpan warna sebagai BGR, sedangkan OpenGL butuh RGB
            rgbData[dstIndex + 0] = rawData[srcIndex + 2]; // R
            rgbData[dstIndex + 1] = rawData[srcIndex + 1]; // G
            rgbData[dstIndex + 2] = rawData[srcIndex + 0]; // B
        }
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width,
        absHeight,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        rgbData.data()
    );

    return textureID;
}