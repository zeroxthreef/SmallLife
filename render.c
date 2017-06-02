#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "render.h"
#include "util.h"
#include "scene.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *mapCache = NULL;

SDL_Rect camera;

unsigned short mapBeenCached = 0;


//SDL_Rect tempCollisionRect, tempTransformRect;
Object *tempAccessObject = NULL;

unsigned long i;

void InitRender(){
  window = SDL_CreateWindow("Small Life by Foxe and 0x3F", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, global.windowWidth, global.windowHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);//this killed the timelapse. Oh well
  //init util with these variables
  initUtil(&renderer);
  //camera.x = 0;
  //camera.y = 0;
  //camera.w = 1280/10;
  //camera.h = 720/10;
  global.cameraX = 0;
  global.cameraY = 0;
  global.cameraWidth = global.windowWidth/global.cameraZoom;
  global.cameraHeight = global.windowHeight/global.cameraZoom;




}
void Render(){
  //tempAccessObject = GetBaseObject();
  for(i = 0; i <= GetObjectCount(); i++){//^^^^^^^^^^^^^^^^^^^^^^
    updateCamera(i);
     ParallaxBackground(i);
    //printf("yeet: %lu %u\n", i, tempAccessObject->visible);
    if(tempAccessObject->type == TYPE_TILE || tempAccessObject->type == TYPE_BACKGROUND){//TODO ONLY CALCULATE COLLISION FOR WHATS VISIBLE tempAccessObject->clipVisible || global.isDebug
        if(tempAccessObject->visible){
          if(tempAccessObject->texture != NULL){
            if(SDL_HasIntersection(&tempAccessObject->objectCollision, &camera)){
              if(global.isDebug){
                SDL_RenderCopy(renderer, tempAccessObject->texture, &tempAccessObject->textureSelect, &tempAccessObject->objectCollision);
              } else{
                SDL_RenderCopy(renderer, tempAccessObject->texture, &tempAccessObject->textureSelect, &tempAccessObject->objectTransform);
              }
              //SDL_RenderCopy(renderer, tempAccessObject->texture, &tempAccessObject->textureSelect, &tempAccessObject->objectCollision);
            }

          }
        }


      if(global.isDebug){
        if(tempAccessObject->hasCollision){
          SDL_SetRenderDrawColor(renderer, 6,230,60,255);
          SDL_RenderDrawRect(renderer, &tempAccessObject->objectCollision);

        } else if(tempAccessObject->hasPhysics){

          SDL_SetRenderDrawColor(renderer, 140,47,60,255);
          SDL_RenderDrawRect(renderer, &tempAccessObject->objectCollision);

        } else{

          SDL_SetRenderDrawColor(renderer, 255,155,0,255);
          SDL_RenderDrawRect(renderer, &tempAccessObject->objectCollision);

        }

        SDL_SetRenderDrawColor(renderer, 56,155,238,255);
        SDL_RenderDrawRect(renderer, &camera);
      }

    }

  }//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  for(i = 0; i <= GetObjectCount(); i++){//^^^^^^^^^^^^^^^^^^^^^^
    updateCamera(i);
    tempAccessObject = GetObject(i);
    //this render pass is #1, so we need to modify the hue here
    if(tempAccessObject->texture != NULL){
      SDL_SetTextureColorMod(tempAccessObject->texture, global.hueR, global.hueG, global.hueB);
    }



    //printf("yeet: %lu %u\n", i, tempAccessObject->visible);
    if(tempAccessObject->type != TYPE_TILE && tempAccessObject->type != TYPE_BACKGROUND){//TODO ONLY CALCULATE COLLISION FOR WHATS VISIBLE || global.isDebug
        if(tempAccessObject->visible){
          if(tempAccessObject->texture != NULL){
             if(SDL_HasIntersection(&tempAccessObject->objectCollision, &camera)){
               SDL_RendererFlip flip;
               if(tempAccessObject->isFlipped){
                 flip = SDL_FLIP_HORIZONTAL;
               } else{
                 flip = SDL_FLIP_NONE;
               }
              if(global.isDebug){
                SDL_RenderCopyEx(renderer, tempAccessObject->texture, &tempAccessObject->textureSelect, &tempAccessObject->objectCollision, tempAccessObject->objectRotation, NULL, flip);
              } else{
                SDL_RenderCopyEx(renderer, tempAccessObject->texture, &tempAccessObject->textureSelect, &tempAccessObject->objectTransform, tempAccessObject->objectRotation, NULL, flip);
              }
              //SDL_RenderCopy(renderer, tempAccessObject->texture, &tempAccessObject->textureSelect, &tempAccessObject->objectCollision);
            }

          }
        }

      //printf("hhhh%d\n", tempAccessObject[i].objectTransform.x);
      if(global.isDebug){
        if(tempAccessObject->hasCollision){

          SDL_SetRenderDrawColor(renderer, 6,230,60,255);
          SDL_RenderDrawRect(renderer, &tempAccessObject->objectCollision);

        } else if(tempAccessObject->hasPhysics){

          SDL_SetRenderDrawColor(renderer, 140,47,60,255);
          SDL_RenderDrawRect(renderer, &tempAccessObject->objectCollision);

        } else{

          SDL_SetRenderDrawColor(renderer, 255,155,0,255);
          SDL_RenderDrawRect(renderer, &tempAccessObject->objectCollision);

        }

        SDL_SetRenderDrawColor(renderer, 56,155,238,255);
        SDL_RenderDrawRect(renderer, &camera);
      }

    }

  }


  //render text stuff
  //==========================================================================
  //==========================================================================
  //==========================================================================
  //==========================================================================
  //==========================================================================


  if(global.state > 0){
    SDL_SetRenderDrawColor(renderer, 180,190,255,255);
  } else{
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
  }
  SDL_RenderPresent(renderer);
  SDL_RenderClear(renderer);
}
void DestroyRender(){
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  //destroy the target texture
}

void ParallaxBackground(unsigned long i){//this isnt proper. I wish it could be dynamic
  tempAccessObject = GetObject(i);
  if(tempAccessObject->type == TYPE_BACKGROUND){
    float xMove, yMove;
    xMove = (float)(global.levelHeight * 16) / (float)tempAccessObject->objectCollision.w;
    yMove = (float)(global.levelWidth * 16) / (float)tempAccessObject->objectCollision.h;
    //xMove = tempAccessObject->objectCollision.w - (global.levelHeight * 16);
    //yMove = tempAccessObject->objectCollision.h - (global.levelWidth * 16);
    //printf("%f %f %d %f gg\n", xMove, yMove, tempAccessObject->objectCollision.x, global.cameraX);
    //tempAccessObject->objectCollision.x = global.cameraX + xMove;
    //tempAccessObject->objectCollision.y = global.cameraY + yMove;
    //tempAccessObject->objectCollision.x += xMove;
    //tempAccessObject->objectCollision.y += yMove;
    //subtract from larger mapsize to find how much it needs to scroll
    tempAccessObject->objectCollision.x = global.cameraX * xMove / yMove;//3.8;
    tempAccessObject->objectCollision.y = global.cameraY * yMove / xMove / 20;//18.8;
  }
}

void updateCamera(unsigned long i){
  camera.x = global.cameraX;
  camera.y = global.cameraY;
  camera.w = (global.windowWidth)/global.cameraZoom;
  camera.h = (global.windowHeight)/global.cameraZoom;
  global.cameraWidth = camera.w;
  global.cameraHeight = camera.h;



  tempAccessObject = GetObject(i);
  tempAccessObject->objectTransform.x = tempAccessObject->objectCollision.x  * global.cameraZoom -global.cameraX * global.cameraZoom;
  tempAccessObject->objectTransform.y = tempAccessObject->objectCollision.y  * global.cameraZoom -global.cameraY * global.cameraZoom;

  //tempAccessObject->objectTransform.x-= camera.w;
  //tempAccessObject->objectTransform.y-= camera.h;

  tempAccessObject->objectTransform.w = tempAccessObject->objectCollision.w * global.cameraZoom;
  tempAccessObject->objectTransform.h = tempAccessObject->objectCollision.h * global.cameraZoom;

  if(global.isDebug){
    SDL_SetRenderDrawColor(renderer, 56,155,238,255);
    SDL_RenderDrawRect(renderer, &tempAccessObject->objectTransform);
  }
  if(global.state > 0){
    if(global.cameraX < 0){
      global.cameraX = 0;
    }
    if(global.cameraY < 0){
      global.cameraY = 0;
    }
    if(global.cameraX + global.cameraWidth > global.levelWidth * 15){
      global.cameraX = global.levelWidth * 15 - global.cameraWidth;
    }
    if(global.cameraY + global.cameraHeight > global.levelHeight * 15){
      global.cameraY = global.levelHeight * 15 - global.cameraHeight;
    }
  }

  if(tempAccessObject->type == TYPE_STRING_CHARACTER){
    //tempAccessObject->objectCollision.x = global.cameraX;
    //tempAccessObject->objectCollision.y = global.cameraY;
  }




}

void RenderText(int x, int y, const char *string){//look for \n and make a black background. maybe color?
  unsigned int i, backdropID;
  SDL_Rect character;
  static char *lastString = NULL;

  if(lastString == NULL || lastString != string){
    printf("Rendering New Text\n");
    character.x = x;
    character.y = y;
    character.w = 8/2;
    character.h = 12/2;
    //backdropID = AddObject(TYPE_STRING_CHARACTER, backdrop, bckSrc, "data/neededforengine/backd.0xdeadaf");
    for (i = 0; i < strlen(string); i++) {
      if(string[i] != '\n'){
        backdropID = AddObject(TYPE_STRING_CHARACTER, character, (SDL_Rect)findSheet(256, 96, 8, 32, (unsigned long)string[i]-1), "data/neededforengine/font.0xdeadaf");
        tempAccessObject = GetObject(backdropID);
        //SDL_SetTextureColorMod(tempAccessObject->texture, 0, 0, 0);
        character.x+= character.w + 2;
      } else{
        character.x = x;
        character.y+= character.h + 1;
      }

      //backdrop.w = (xCount / character.w) * character.w;
      //backdrop.h = (yCount /character.h) * character.h;

      //tempAccessObject->objectCollision = backdrop;
    }
    lastString = string;
  }
}

void ClearAllText(){
  unsigned long i;
  for (i = 0; i <= GetObjectCount(); i++) {
    tempAccessObject = GetObject(i);
    if(tempAccessObject->type == TYPE_STRING_CHARACTER){
      tempAccessObject->visible = 0;
    }
  }
}
