#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "scene.h"
#include "util.h"
unsigned short firstCalled = 1;


Object *objects = NULL;
unsigned long objectAmmount = 0;

unsigned long AddObject(unsigned long type, SDL_Rect collisionRect, SDL_Rect selectTexture, const char *textureUrl){

  if(firstCalled){//for first time calling

    objects = (Object *)malloc(sizeof(Object));//this is fine

    if(objects == NULL){

      printf("Critical memory allocation error\n");

    } else{
      printf("Allocated memory\n");
    }

  } else{
    objectAmmount++;

    Object *tempObjects = (Object *)realloc(objects, (objectAmmount + 1) * sizeof(Object));//have to do 1+ to make room for more||sizeof(Object)

    if(tempObjects == NULL){

      printf("CRITICAL*** Out of memory/memory error\n");
      exit(1);

    } else{

      //printf("Reallocation successfull\n");

      objects = tempObjects;
      tempObjects = NULL;

    }
  }




  //============================================================================
  if(textureUrl != NULL){
    createTex(&objects[objectAmmount].texture, textureUrl);
  } else{
    objects[objectAmmount].texture = NULL;
  }



  if(objects[objectAmmount].texture == NULL){
    printf("No texture\n");
  }


  objects[objectAmmount].textureSelect = selectTexture;
  objects[objectAmmount].objectCollision = collisionRect;//this doesnt have the accuracy of floats, so that might take more work than I want. Use this to get world coordinates
  objects[objectAmmount].objectTransform.x = 0;
  objects[objectAmmount].objectTransform.y = 0;
  objects[objectAmmount].objectTransform.w = 0;
  objects[objectAmmount].objectTransform.h = 0;
  objects[objectAmmount].objectVelX = 0.0f;
  objects[objectAmmount].objectVelY = 0.0f;
  objects[objectAmmount].objectX = 0.0f;//do I need this? I want floats, but it needs to share its value with an SDL_Rect for collision. Not going to use these after decision
  objects[objectAmmount].objectY = 0.0f;//do I need this? I want floats, but it needs to share its value with an SDL_Rect for collision
  objects[objectAmmount].objectRotation = 0.0f;
  objects[objectAmmount].isFlipped = 0;
  objects[objectAmmount].hasCollision = 0;
  objects[objectAmmount].hasPhysics = 0;
  objects[objectAmmount].used = 1;
  objects[objectAmmount].visible = 1;
  objects[objectAmmount].clipVisible = 1;
  objects[objectAmmount].type = type;


  //============================================================================



  if(firstCalled){//for first time calling
    //just give the function that called this this number because this is the object ID;
    firstCalled = 0;
    return objectAmmount;
  } else{
    return objectAmmount;//I could make it go back through and clean up objects and put new ones in their place, but until I run out of storage, this isnt a priority
  }
}
int DeleteObject(unsigned long ID){
  objects[ID].used = 0;
  return 0;
}
Object *GetObject(unsigned long ID){
  return &objects[ID];
}
Object *GetBaseObject(){
  return objects;
}
unsigned long GetObjectCount(){
  return objectAmmount;
}
int CreateScene(){//only call this once

  global.state = 0;
  global.isDebug = 0;
  global.isNoclip = 0;
  global.health = 100;
  global.points = 0;
  global.gravity = 0.38f;
  global.playerMoveLeft = 0;
  global.playerMoveRight = 0;
  global.playerJump = 0;
  global.playerCrouch = 0;
  global.playerCollided = 0;
  global.playerUse= 0;
  global.playerHasGun = 0;
  global.inCinematic = 0;
  global.canExit = 0;
  global.isPaused = 0;
  global.cameraZoom = 7.0f;
  global.cameraX = 0.0f;
  global.cameraY = 0.0f;
  global.cameraWidth = 0.0f;
  global.cameraHeight = 0.0f;
  global.windowWidth = 1280;
  global.windowHeight = 720;
  global.levelWidth = 0;
  global.levelHeight = 0;
  global.hueR = 255;
  global.hueG = 255;
  global.hueB = 255;

  return 0;
}
int DestroyScene(){
  printf("Destroying scene\n");
  firstCalled = 1;
  if(objects != NULL){
    unsigned long i;
    for(i = 0; i < objectAmmount; i++){
      if(objects[i].texture != NULL){
        SDL_DestroyTexture(objects[i].texture);
      }
    }
    free(objects);
    objects = NULL;
  } else{
    printf("Nothing to free\n");
  }

  objectAmmount = 0;
  return 0;
}
