#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "scene.h"
#include "menu.h"
#include "util.h"
#include "render.h"
#include "player.h"
#include "physics.h"
#include "map.h"

SDL_Rect backdrop, playButton, exitButton, creditsButton, mouseRect, selectR;
Uint32 mouse;
int mx, my, isClicked = 0;
Object *menuTempObject = NULL;
unsigned long menuID;
unsigned long creditsID;
Mix_Music *menuMusic = NULL;
Mix_Chunk *button = NULL;

//==============================================================================
void CreateMenu(){
  printf("Creating menu\n");
  //CreatePlayer(0, 0, 26, 26, "data/Jim-main-walk-animation.png");//make player 26x26
  //AddCollisionBlacklist(0);
  //CreateMap(15, 15, 64, 64, 8, 8, "data/smol.tmx", "data/levelMap.png", "data/font.png");
  //CreateMap(16, 16, 320, 320, 20, 20, "data/testformain.tmx", "data/insidecave-tileset.png", "data/font.png");//later
  //CreateMap(16, 16, 246, 256, 16, 16, "data/testmap1.tmx", "data/large-world-tilemap.png", "data/font.png");
  //CreatePhysics();
  //CreatePlayer();

  menuMusic = Mix_LoadMUS("data/global/music.mp3");
  //Mix_PlayMusic(menuMusic, -1);

  selectR.x = 0;
  selectR.y = 0;
  selectR.w = global.windowWidth;
  selectR.h = global.windowHeight;

  backdrop.x = 0;
  backdrop.y = 0;
  backdrop.w = global.cameraWidth + 1;
  backdrop.h = global.cameraHeight + 1;


  menuID = AddObject(99, backdrop, selectR, "data/menu/TITLE-SCREEN-SMALL-LIFE.png");
  creditsID = AddObject(99, backdrop, selectR, "data/menu/credits-screen.png");
  menuTempObject = GetObject(creditsID);
  menuTempObject->visible = 0;

  button = Mix_LoadWAV("data/global/buttonPush.wav");




  mouseRect.x = 0;
  mouseRect.y = 0;
  mouseRect.w = 1;
  mouseRect.h = 1;

  //AddObject(99, mouseRect, mouseRect, "data/credits-screen.png");

  playButton.x = 387;
  playButton.y = 308;
  playButton.w = 130;
  playButton.h = 73;

  exitButton.x = 1183;
  exitButton.y = 19;
  exitButton.w = 84;
  exitButton.h = 46;

  creditsButton.x = 748;
  creditsButton.y = 316;
  creditsButton.w = 199;
  creditsButton.h = 61;



}
//==============================================================================
void EventMenu(){
  if(SDL_GetMouseState(&mx, &my) & SDL_BUTTON(1)){
    isClicked = 1;
  } else{
    isClicked = 0;
  }
}
static unsigned short clickedChange = 0;
void LogicMenu(){
  //printf("%d %d %d %d\n", mx, my, isClicked, menuTempObject->visible);
  mouseRect.x = mx;
  mouseRect.y = my;
  if(SDL_HasIntersection(&mouseRect, &playButton) && isClicked){
    global.state++;
    Mix_PlayChannel(-1, button, 0);
  }
  if(SDL_HasIntersection(&mouseRect, &exitButton) && isClicked){
    global.canExit = 1;
    Mix_PlayChannel(-1, button, 0);
  }
  if(SDL_HasIntersection(&mouseRect, &creditsButton) && isClicked){
    menuTempObject = GetObject(creditsID);
    menuTempObject->visible = 1;

    menuTempObject = GetObject(menuID);
    menuTempObject->visible = 0;
    //createTex(&menuTempObject->texture, "data/menu/credits-screen.png");
    if(!clickedChange){
      Mix_PlayChannel(-1, button, 0);
    }
    clickedChange = 1;
  } else{
    //printf("Should draw this\n");
    menuTempObject = GetObject(menuID);
    menuTempObject->visible = 1;
    menuTempObject = GetObject(creditsID);
    menuTempObject->visible = 0;
    clickedChange = 0;
  }
}
void RenderMenu(){
  Render();
}
void DestroyMenu(){
  SDL_Delay(740);//because the sound has to play
  Mix_FreeChunk(button);
  Mix_FreeMusic(menuMusic);
  printf("Destroying Level 1\n");
  DestroyScene();
}
