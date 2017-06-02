#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "scene.h"
#include "menu.h"
#include "level1.h"
#include "util.h"
#include "render.h"
#include "player.h"
#include "physics.h"
#include "map.h"

SDL_Rect playerWorldRect;
SDL_Rect playerSheetSelect, crate, crateSelect, nextStateRect;
unsigned long playerID;
unsigned long testThing;
Mix_Music *l1mus = NULL;

Object *tempObject = NULL;
//==============================================================================
void CreateLevel1(){
  printf("Creating Level 1\n");

  l1mus = Mix_LoadMUS("data/global/music.mp3");
  Mix_PlayMusic(l1mus, -1);

  //CreateScene();//DO NOT CALL THIS IN A STATE
  CreatePlayer(524, 102, 26, 26, "data/global/Jim-main-walk-animation.png");//make player 26x26
  DoLevel1BlacklistGeneration();
  //CreateMap(15, 15, 64, 64, 8, 8, "data/badmap.tmx", "data/levelMap.png", "data/font.png");
  //CreateMap(16, 16, 320, 320, 20, 20, "data/testformain.tmx", "data/insidecave-tileset.png", "data/font.png");//later
  CreateMap(16, 16, 256, 256, 16, 16, "data/level1/levelStart.tmx", "data/global/large-world-tilemap.png", "data/global/new-pixel-bg.png");
  CreatePhysics();
  //CreatePlayer();
  global.hueR = 0;
  global.hueG = 0;//220;//slight reddish tone to make warm atmosphere
  global.hueB = 0;//220;

  crate.x = 80;
  crate.y = 0;
  crate.w = 16;
  crate.h = 16;

  crateSelect.x = 0;
  crateSelect.y = 0;
  crateSelect.w = 16;
  crateSelect.h = 16;

  nextStateRect.x = 0;
  nextStateRect.y = 182 - 3*16;
  nextStateRect. w = 16;
  nextStateRect.h = 5*16;

  AddObject(TYPE_RANDOMOBJECT, nextStateRect, nextStateRect, NULL);

  unsigned long tempcrateID = 0, r;

  for(r = 0; r < 6; r++){
    tempcrateID = AddObject(TYPE_RANDOMOBJECT, crate, crateSelect, "data/global/crate.png");
    tempObject = GetObject(tempcrateID);
    tempObject->hasPhysics = 1;
    tempObject->hasCollision = 1;
    crate.y+= crate.h;
  }
  //UpdatePlayer();
  RenderText(532 - 3 * 16, 102 + 2*16, "WHAT A BEUTIFUL\nTOTALLY NORMAL\nRELAXING\nABSOLUTELY STANDARD DAY.");
  RenderText(120 - 3 *16, 182 + 2*16, "OH. THATS NEW.\nHOW DO I GET INTO THE CAVE?");



  global.hueR = 0;
  global.hueG = 0;
  global.hueB = 0;

  global.cameraZoom = 7.0f;



}
//==============================================================================
void EventLevel1(){

}
void LogicLevel1(){//play intro sequence
  UpdatePlayer();
  static float i = 0.0f;
  if(i < 255.0f){
    global.hueR= (Uint8)i;
    if(i < 220){
      global.hueG= (Uint8)i;//slight reddish tone to make warm atmosphere
      global.hueB= (Uint8)i;
    }
    i +=0.6f;
    global.inCinematic = 1;
  } else{
    global.inCinematic = 0;
  }

  //put detection code for searching if a bug type and the player is touching here. No need to modify collision code
  unsigned long j;
  for(j = 0; j <= GetObjectCount(); j++){
    tempObject = GetObject(j);
    if(tempObject->type == TYPE_PLAYER){
      if(SDL_HasIntersection(&tempObject->objectCollision, &nextStateRect)){
        global.state++;
      }
    }
  }


  //update everything else
  //check for collisions at the end
  UpdatePhysics();
}
void RenderLevel1(){
  Render();
}
void DestroyLevel1(){
  printf("Destroying Level 1\n");
  Mix_FreeMusic(l1mus);
  DestroyScene();
  DestroyMap();
}
void DoLevel1BlacklistGeneration(){
  AddCollisionWhiteList(225);
  AddCollisionWhiteList(226);
  AddCollisionWhiteList(227);
  AddCollisionWhiteList(228);
  AddCollisionWhiteList(229);
  AddCollisionWhiteList(230);
  AddCollisionWhiteList(231);
  AddCollisionWhiteList(232);
  AddCollisionWhiteList(233);
  AddCollisionWhiteList(241);
  AddCollisionWhiteList(242);
  AddCollisionWhiteList(245);
  AddCollisionWhiteList(177);
  AddCollisionWhiteList(178);
  AddCollisionWhiteList(179);
  AddCollisionWhiteList(180);
  AddCollisionWhiteList(181);
  AddCollisionWhiteList(193);
  AddCollisionWhiteList(194);
  AddCollisionWhiteList(195);
  AddCollisionWhiteList(196);
  AddCollisionWhiteList(197);
  AddCollisionWhiteList(25);
  AddCollisionWhiteList(26);
  AddCollisionWhiteList(27);
  AddCollisionWhiteList(28);
  AddCollisionWhiteList(29);
  AddCollisionWhiteList(30);
  AddCollisionWhiteList(31);
  /*Too inefficient to do a blacklist
  AddCollisionBlacklist(0);
  AddCollisionBlacklist(1);
  AddCollisionBlacklist(2);
  AddCollisionBlacklist(3);
  AddCollisionBlacklist(17);
  AddCollisionBlacklist(18);
  AddCollisionBlacklist(19);
  AddCollisionBlacklist(33);
  AddCollisionBlacklist(34);
  AddCollisionBlacklist(35);
  AddCollisionBlacklist(41);
  AddCollisionBlacklist(42);
  AddCollisionBlacklist(43);
  AddCollisionBlacklist(44);
  AddCollisionBlacklist(45);
  AddCollisionBlacklist(46);
  AddCollisionBlacklist(47);
  AddCollisionBlacklist(48);
  AddCollisionBlacklist(49);
  AddCollisionBlacklist(50);
  AddCollisionBlacklist(51);
  AddCollisionBlacklist(57);
  AddCollisionBlacklist(58);
  AddCollisionBlacklist(59);
  AddCollisionBlacklist(60);
  AddCollisionBlacklist(61);
  AddCollisionBlacklist(62);
  AddCollisionBlacklist(63);
  AddCollisionBlacklist(64);
  AddCollisionBlacklist(65);
  AddCollisionBlacklist(66);
  AddCollisionBlacklist(67);
  AddCollisionBlacklist(73);
  AddCollisionBlacklist(74);
  AddCollisionBlacklist(75);
  AddCollisionBlacklist(76);
  AddCollisionBlacklist(77);
  AddCollisionBlacklist(78);
  AddCollisionBlacklist(79);
  AddCollisionBlacklist(63);
  AddCollisionBlacklist(79);
  AddCollisionBlacklist(63);
  AddCollisionBlacklist(243);
  AddCollisionBlacklist(244);
  AddCollisionBlacklist(209);
  AddCollisionBlacklist(210);
  AddCollisionBlacklist(211);
  AddCollisionBlacklist(157);
  AddCollisionBlacklist(158);
  AddCollisionBlacklist(159);
  */
}
