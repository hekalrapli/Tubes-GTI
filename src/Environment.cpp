#include "Environment.h"
#include "Globals.h"
#include <cstdlib>

void initEnvironment(){
    trees.clear(); buildings.clear();
    for(int i=0;i<20;i++){
        float z=-75.f+i*9.f;
        { Tree t; t.x=-6.f-(float)(rand()%3)*1.2f; t.z=z+(rand()%4)-2; t.trunkH=1.5f+(rand()%10)*.15f; t.crownR=.9f+(rand()%6)*.12f; t.seed=rand()%10; trees.push_back(t); }
        { Tree t; t.x= 6.f+(float)(rand()%3)*1.2f; t.z=z+(rand()%4)-2; t.trunkH=1.5f+(rand()%10)*.15f; t.crownR=.9f+(rand()%6)*.12f; t.seed=rand()%10; trees.push_back(t); }
    }
    float bc[5][3]={{.75f,.70f,.60f},{.60f,.55f,.50f},{.80f,.78f,.72f},{.55f,.60f,.65f},{.70f,.65f,.58f}};
    for(int i=0;i<8;i++){
        float z=-70.f+i*17.f; int ci=rand()%5;
        { Building b; b.x=-11;b.z=z;  b.w=2.5f+(rand()%3)*.5f;b.d=2+(rand()%3)*.4f;b.h=3+(rand()%8)*.7f;b.r=bc[ci][0];b.g=bc[ci][1];b.b=bc[ci][2]; buildings.push_back(b); }
        { ci=rand()%5; Building b; b.x=11;b.z=z+5;b.w=2.5f+(rand()%3)*.5f;b.d=2+(rand()%3)*.4f;b.h=3+(rand()%8)*.7f;b.r=bc[ci][0];b.g=bc[ci][1];b.b=bc[ci][2]; buildings.push_back(b); }
    }
}

void spawnEnemy(){ Enemy e; e.x=lanePos[rand()%3]; e.z=-70; enemies.push_back(e); }
void spawnNos()  { NosPickup n; n.x=lanePos[rand()%3]; n.z=-65; n.active=true; n.rotAngle=0; n.bobTimer=0; nosPickups.push_back(n); }
