#ifndef GAME_H
#define GAME_H

void startGame();
void resetToStart();
bool checkCollision(float ex, float ez);
bool checkNosCollision(float nx, float nz);
void drawScene3D(float camRotY = 0.0f);
void display();
void update(int value);
void specialKeyboard(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void mouseButton(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void reshape(int w, int h);
void init();

#endif
