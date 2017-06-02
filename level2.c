#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "scene.h"
#include "menu.h"
#include "level2.h"
#include "util.h"
#include "render.h"
#include "player.h"
#include "physics.h"
#include "map.h"

SDL_Rect playerWorldRect;
SDL_Rect playerSheetSelect, crate, crateSelect, nextStateRect, drinkPotionRect;
unsigned long playerID;
unsigned long testThing;
unsigned short cinematicPlayed = 0, cinematicOver = 0;
Mix_Music *l2mus = NULL;

Object *tempObjectl2 = NULL;
//==============================================================================
void CreateLevel2(){
  printf("Creating Level 2\n");


  l2mus = Mix_LoadMUS("data/global/music1.mp3");
  Mix_PlayMusic(l2mus, -1);

  //CreateScene();//DO NOT CALL THIS IN A STATE
  CreatePlayer(532, 102, 26, 26, "data/global/Jim-main-walk-animation.png");//make player 26x26
  DoLevel2BlacklistGeneration();
  //CreateMap(15, 15, 64, 64, 8, 8, "data/badmap.tmx", "data/levelMap.png", "data/font.png");
  //CreateMap(16, 16, 320, 320, 20, 20, "data/testformain.tmx", "data/insidecave-tileset.png", "data/font.png");//later
  CreateMap(16, 16, 320, 320, 20, 20, "data/level2/firstcaveentrance.tmx", "data/global/insidecave-tileset.png", NULL);
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
  nextStateRect.y = 182 - 9*16;
  nextStateRect. w = 16;
  nextStateRect.h = 7*16;



  drinkPotionRect.x = 130;
  drinkPotionRect.y = 86 - 4 * 16;
  drinkPotionRect.w = 1 * 16;
  drinkPotionRect.h = 7 * 16;

  AddObject(TYPE_RANDOMOBJECT, nextStateRect, nextStateRect, NULL);
  AddObject(TYPE_RANDOMOBJECT, drinkPotionRect, drinkPotionRect, NULL);

  //unsigned long tempcrateID = 0;

    //tempcrateID = AddObject(TYPE_RANDOMOBJECT, crate, crateSelect, "data/global/crate.png");
    //tempObjectl2 = GetObject(tempcrateID);
    //tempObjectl2->hasPhysics = 1;
    //tempObjectl2->hasCollision = 1;
    //crate.y+= crate.h;
  //UpdatePlayer();
  //RenderText(532 - 3 * 16, 102 + 2*16, "WHAT A BEUTIFUL\nTOTALLY NORMAL\nRELAXING\nABSOLUTELY STANDARD DAY.");
  //RenderText(120 - 3 *16, 182 + 2*16, "OH. THATS NEW.\nHOW DO I GET INTO THE CAVE?");


  global.hueR = 0;
  global.hueG = 0;
  global.hueB = 0;
  global.cameraZoom = 7.0f;





}
//==============================================================================
void EventLevel2(){

}
void LogicLevel2(){//play intro sequence
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
    tempObjectl2 = GetObject(j);
    if(tempObjectl2->type == TYPE_PLAYER){
      if(SDL_HasIntersection(&tempObjectl2->objectCollision, &nextStateRect) && cinematicPlayed){
        global.state++;
      } else if(SDL_HasIntersection(&tempObjectl2->objectCollision, &drinkPotionRect) && !cinematicOver){
        static int i = 0;
        if(i < 500){//this is fine
          tempObjectl2->objectVelX = 0;
          RenderText(5 * 16, 2*16, "WOW. I NEVER KNEW WHAT WAS\nINSIDE THIS CAVE 20FT AWAY\nFROM MY HOUSE!");
        }
        if( i > 500 && i < 502){
          ClearAllText();
        }
        if( i > 502 && i < 700){
          RenderText(5 * 16, 2*16, "SCORE!\nFREE SODA IN\nA WEIRD LOOKING BOTTLE.");
        }
        if( i > 700 && i < 702){
          ClearAllText();
        }
        if( i > 702 && i < 900){
          static int rrun = 0;
          RenderText(7 * 16, 2*16, "YUM");
          if(!rrun){
            tempObjectl2->textureSelect = findSheet(192, 96, 3, 6, 13);
            rrun = 1;
          }
        }
        if( i > 902 && i < 904){
          tempObjectl2->textureSelect = findSheet(192, 96, 3, 6, 0);
          ClearAllText();
        }
        if( i > 980 && i < 1200){
          RenderText(5 * 16, 2*16, "THAT WASNT SODA...");
        }
        if( i > 1202 && i < 1204){
          ClearAllText();
          Mix_PauseMusic();
        }
        if( i > 1204 && i < 1500){
          RenderText(5 * 16, 6*16, "AAAAHHHHHHHHH!!!!");
          tempObjectl2->objectRotation += 30.3;
          if(tempObjectl2->objectCollision.w > 8){
            tempObjectl2->objectCollision.w-= 1;
            tempObjectl2->objectCollision.h-= 1;
          }
          if(global.hueR != 0){
            global.hueR--;
          }
          if(global.hueG != 0){
            global.hueG--;
          }
          if(global.hueB != 0){
            global.hueB--;
          }
          global.cameraZoom+= 0.1;
        }
        if( i > 1500 && i < 1502){
          ClearAllText();
          Mix_ResumeMusic();
        }
        if( i > 1600 && i < 1900){
          tempObjectl2->objectRotation = 0;
          global.cameraZoom = 10.0f;
          global.hueR = 180;
          global.hueG = 180;
          global.hueB = 200;
          RenderText(6 * 16, 5*16, "OH GEEZ...");
        }
        if(i>1900 && i < 1903){//end
          ClearAllText();
          cinematicPlayed = 1;
          cinematicOver = 1;
          global.inCinematic = 0;
        } else{
          cinematicOver = 0;
          global.inCinematic = 1;
        }
        //printf("yo %d\n", i);
        i++;

      }
    }
  }


  //update everything else
  //check for collisions at the end
  UpdatePhysics();
}
void RenderLevel2(){
  Render();
}
void DestroyLevel2(){
  printf("Destroying Level 2\n");
  Mix_FreeMusic(l2mus);
  DestroyScene();
  DestroyMap();
}
void DoLevel2BlacklistGeneration(){
  AddCollisionWhiteList(366);
  AddCollisionWhiteList(367);
  AddCollisionWhiteList(368);
  /*
  AddCollisionWhiteList(381);
  AddCollisionWhiteList(382);
  AddCollisionWhiteList(383);
  AddCollisionWhiteList(384);
  AddCollisionWhiteList(385);
  AddCollisionWhiteList(361);
  AddCollisionWhiteList(362);
  AddCollisionWhiteList(363);
  AddCollisionWhiteList(364);
  AddCollisionWhiteList(365);
  */
}
