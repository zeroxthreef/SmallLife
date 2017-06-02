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
SDL_Rect playerSheetSelect, crate, crateSelect, nextStateRect, keyRect, gunRect;
unsigned long playerID;
unsigned long testThing;
Mix_Music *l3mus = NULL;
Mix_Music *l3musEnd = NULL;
Object *tempObjectl3 = NULL;
Mix_Chunk *keySound = NULL;
Mix_Chunk *thatsAllFolks = NULL;

unsigned short hasKey = 0, playedEnd = 0;
//==============================================================================
void CreateLevel3(){
  printf("Creating Level 3\n");
  global.playerHasGun = 0;

  l3mus = Mix_LoadMUS("data/global/music1.mp3");
  Mix_PlayMusic(l3mus, -1);
  l3musEnd = Mix_LoadMUS("data/global/music.mp3");

  keySound = Mix_LoadWAV("data/global/Pickup_key.wav");
  thatsAllFolks = Mix_LoadWAV("data/global/die_bugdeath.wav");



  //CreateScene();//DO NOT CALL THIS IN A STATE
  CreatePlayer(36, 101, 26, 26, "data/global/Jim-main-walk-animation.png");//make player 26x26
  unsigned long i;
  for(i = 0; i <= GetObjectCount(); i++){
    tempObjectl3 = GetObject(i);
    if(tempObjectl3->type == TYPE_PLAYER){
      tempObjectl3->objectCollision.w = 8;
      tempObjectl3->objectCollision.h = 8;
    }
  }
  DoLevel3BlacklistGeneration();
  //CreateMap(15, 15, 64, 64, 8, 8, "data/badmap.tmx", "data/levelMap.png", "data/font.png");
  //CreateMap(16, 16, 320, 320, 20, 20, "data/testformain.tmx", "data/insidecave-tileset.png", "data/font.png");//later
  CreateMap(16, 16, 320, 320, 20, 20, "data/level3/firstlevel.tmx", "data/global/insidecave-tileset.png", NULL);
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

  nextStateRect.x = 520;
  nextStateRect.y = 127-3*16;
  nextStateRect. w = 16;
  nextStateRect.h = 3*16;


  keyRect.x = 349;
  keyRect.y = 40 - 1*16;
  keyRect.w = 2*16;
  keyRect.h = 2*16;

  gunRect.x = 408;
  gunRect.y = 104 - 1*16;
  gunRect.w = 2*16;
  gunRect.h = 2*16;


  AddObject(TYPE_RANDOMOBJECT, nextStateRect, nextStateRect, NULL);
  AddObject(TYPE_RANDOMOBJECT, keyRect, keyRect, NULL);
  AddObject(TYPE_RANDOMOBJECT, gunRect, gunRect, NULL);

  //unsigned long tempcrateID = 0, r;


  //UpdatePlayer();




  global.hueR = 0;
  global.hueG = 0;
  global.hueB = 0;

  global.cameraZoom = 10.0f;



}
//==============================================================================
void EventLevel3(){

}
void LogicLevel3(){//play intro sequence
  UpdatePlayer();
  static float i = 0.0f;
  if(i < 200.0f){
    global.hueB= (Uint8)i;
    if(i < 180){
      global.hueG= (Uint8)i;//slight reddish tone to make warm atmosphere
      global.hueR= (Uint8)i;
    }
    i +=0.6f;
    global.inCinematic = 1;
  } else{
    global.inCinematic = 0;
  }

  //put detection code for searching if a bug type and the player is touching here. No need to modify collision code
  unsigned long j;
  for(j = 0; j <= GetObjectCount(); j++){
    tempObjectl3 = GetObject(j);
    if(tempObjectl3->type == TYPE_PLAYER){
      if(SDL_HasIntersection(&tempObjectl3->objectCollision, &nextStateRect) && hasKey && global.playerHasGun && !playedEnd){
        ClearAllText();
        RenderText(450, 90, "THE GAME WASNT\nCOMPLETED IN TIME.\nTHE END! :)");
        playedEnd = 1;
        //global.state++;
        Mix_PlayChannel(-1, thatsAllFolks, 0);
        Mix_PlayMusic(l3musEnd, -1);
        //ClearAllText();
      } else if(SDL_HasIntersection(&tempObjectl3->objectCollision, &nextStateRect) && !playedEnd){
        RenderText(450, 110, "I NEED A GUN AND\nA KEY");
        //ClearAllText();
      }
      if(SDL_HasIntersection(&tempObjectl3->objectCollision, &keyRect) && !hasKey){
        hasKey = 1;
        Mix_PlayChannel(-1, keySound, 0);
      }
      if(SDL_HasIntersection(&tempObjectl3->objectCollision, &gunRect) && !global.playerHasGun){
        global.playerHasGun = 1;
        Mix_PlayChannel(-1, keySound, 0);
      }
    }
  }


  //update everything else
  //check for collisions at the end
  UpdatePhysics();
}
void RenderLevel3(){
  Render();
}
void DestroyLevel3(){
  printf("Destroying Level 3\n");
  Mix_FreeMusic(l3mus);
  Mix_FreeMusic(l3musEnd);
  Mix_FreeChunk(thatsAllFolks);
  Mix_FreeChunk(keySound);
  DestroyScene();
  DestroyMap();
}
void DoLevel3BlacklistGeneration(){
  AddCollisionWhiteList(366);
  AddCollisionWhiteList(367);
  AddCollisionWhiteList(368);
  AddCollisionWhiteList(123);
  AddCollisionWhiteList(143);
  AddCollisionWhiteList(163);
}
