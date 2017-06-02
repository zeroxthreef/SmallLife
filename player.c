#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

Mix_Chunk *jump = NULL;
Mix_Chunk *die = NULL;


#include "scene.h"
#include "player.h"
#include "util.h"

Object *tempPlayerAccessObject = NULL;
Object *tempOtherAccessObject = NULL;

unsigned long playerID;
SDL_Rect bullet;

void CreatePlayer(int x, int y, int width, int height, const char *URL){
  bullet.x = 0;
  bullet.y = 0;
  bullet.w = 2;
  bullet.h = 2;
  SDL_Rect worldPlayer;
  worldPlayer.x = x;
  worldPlayer.y = y;
  worldPlayer.w = width;
  worldPlayer.h = height;
  playerID = AddObject(TYPE_PLAYER, worldPlayer, (SDL_Rect)findSheet(192, 96, 3, 6, 0), URL);//need types for collision later
  tempPlayerAccessObject = GetObject(playerID);
  tempPlayerAccessObject->hasPhysics = 1;
  tempPlayerAccessObject->hasCollision = 1;

  jump = Mix_LoadWAV("data/global/Jump.wav");
  die = Mix_LoadWAV("data/global/die_bugdeath.wav");
  //modify something?
}
void UpdatePlayer(){//dont worry about collision or physics here. Just accellerate the player
//for the font renderer, set the type to be font
static int jumped = 0;
static int delay = 0, i = 0;

tempPlayerAccessObject = GetObject(playerID);

if(global.isNoclip){//no physics enabled or collision
  tempPlayerAccessObject->hasCollision = 0;
  tempPlayerAccessObject->hasPhysics = 0;

  if(global.playerMoveLeft){
    tempPlayerAccessObject->objectVelX = -2.5;
    tempPlayerAccessObject->isFlipped = 1;
  }
  if(global.playerMoveRight){
    tempPlayerAccessObject->objectVelX = 2.5;
    tempPlayerAccessObject->isFlipped = 0;
  }
  if(global.playerJump){
    tempPlayerAccessObject->objectVelY = -2.5;
  }
  if(global.playerCrouch){
    tempPlayerAccessObject->objectVelY = 2.5;
  }
  if(global.playerJump == 0 && global.playerCrouch == 0){
    tempPlayerAccessObject->objectVelY = 0;
  } else if(global.playerJump == 1 && global.playerCrouch == 1){
    tempPlayerAccessObject->objectVelY = 0;
  }

  if(global.playerMoveLeft == 0 && global.playerMoveRight == 0){
    tempPlayerAccessObject->objectVelX = 0;
  }else if(global.playerMoveLeft == 1 && global.playerMoveRight == 1){
    tempPlayerAccessObject->objectVelX = 0;
  }

} else{
  tempPlayerAccessObject->hasCollision = 1;
  tempPlayerAccessObject->hasPhysics = 1;
  if(global.playerMoveLeft){//==================================================
    if(abs(tempPlayerAccessObject->objectVelX) <= 1.8){
      tempPlayerAccessObject->objectVelX -= 1.8;
    }
    if(global.playerHasGun){
      if(delay == 0){
        delay = 5;
        if(i < 9){

          tempPlayerAccessObject->textureSelect = findSheet(192, 96, 3, 6, i);
          i++;
        } else{
          i = 6;
        }
      } else{
        delay--;
      }
    } else{
      if(delay == 0){
        delay = 5;
        if(i < 4){

          tempPlayerAccessObject->textureSelect = findSheet(192, 96, 3, 6, i);
          i++;
        } else{
          i = 0;
        }
      } else{
        delay--;
      }
    }

    tempPlayerAccessObject->isFlipped = 1;
  }
  if(global.playerMoveRight){//==================================================
    if(abs(tempPlayerAccessObject->objectVelX) <= 1.8){
      tempPlayerAccessObject->objectVelX += 1.8;
    }
    if(global.playerHasGun){
      if(delay == 0){
        delay = 5;
        if(i < 9){

          tempPlayerAccessObject->textureSelect = findSheet(192, 96, 3, 6, i);
          i++;
        } else{
          i = 6;
        }
      } else{
        delay--;
      }
    } else{
      if(delay == 0){
        delay = 5;
        if(i < 4){

          tempPlayerAccessObject->textureSelect = findSheet(192, 96, 3, 6, i);
          i++;
        } else{
          i = 0;
        }
      } else{
        delay--;
      }
    }

    tempPlayerAccessObject->isFlipped = 0;
  }
  if(global.playerJump){//==================================================
    if(!jumped && global.playerCollided){
      tempPlayerAccessObject->objectVelY -= 8.5;
      jumped = 1;
      Mix_PlayChannel(-1, jump, 0);
      if(global.playerHasGun){
        tempPlayerAccessObject->textureSelect = findSheet(192, 96, 3, 6, 10);
      } else{
        tempPlayerAccessObject->textureSelect = findSheet(192, 96, 3, 6, 4);
      }
    } else{
      jumped = 0;
    }

    //make sure it has collided with the ground again before jumping
  }
  if(global.playerCrouch){//==================================================
    //tempPlayerAccessObject->objectVelY += 2.5;
    if(global.playerHasGun){
      tempPlayerAccessObject->textureSelect = findSheet(192, 96, 3, 6, 11);
    } else{
      tempPlayerAccessObject->textureSelect = findSheet(192, 96, 3, 6, 5);
    }
  }
  if(!global.playerCrouch && !global.playerJump && !global.playerMoveLeft && !global.playerMoveRight && !global.inCinematic){//because its at the end of the player handler
    if(global.playerHasGun){
      tempPlayerAccessObject->textureSelect = findSheet(192, 96, 3, 6, 6);
    } else{
      tempPlayerAccessObject->textureSelect = findSheet(192, 96, 3, 6, 0);
    }
  }

  if(global.playerUse && global.playerHasGun){//==================================================
    //Mix_PlayChannel(-1, die, 0);
    unsigned long tID = AddObject(TYPE_BULLET, bullet, (SDL_Rect)findSheet(16, 16, 1, 1, 0), "data/global/gunshot.png");
    tempOtherAccessObject = GetObject(tID);
    tempOtherAccessObject->hasPhysics = 1;
    tempOtherAccessObject->hasCollision = 0;

    tempOtherAccessObject->objectCollision.x = tempPlayerAccessObject->objectCollision.x;
    tempOtherAccessObject->objectCollision.y = tempPlayerAccessObject->objectCollision.y;

    tempOtherAccessObject->objectVelY -= abs(rand() % 3);
    if(tempPlayerAccessObject->isFlipped){
      tempOtherAccessObject->objectVelX = -12;
    } else{
      tempOtherAccessObject->objectVelX = 12;
    }
    //printf("pew\n");
  }

}//TODO TODO update tile animations

tempPlayerAccessObject = GetObject(playerID);


global.cameraX = (float)tempPlayerAccessObject->objectCollision.x - ((global.cameraWidth/2) - (float)(tempPlayerAccessObject->objectCollision.w/2));// / tempPlayerAccessObject->objectCollision.w;
global.cameraY = (float)tempPlayerAccessObject->objectCollision.y - ((global.cameraHeight/2) - (float)(tempPlayerAccessObject->objectCollision.h/2));// / tempPlayerAccessObject->objectCollision.h;


//tempPlayerAccessObject->objectCollision.x += (int)tempPlayerAccessObject->objectVelX;
//tempPlayerAccessObject->objectCollision.y += (int)tempPlayerAccessObject->objectVelY;

//printf("%d %f %d %f\n", tempPlayerAccessObject->objectCollision.x, tempPlayerAccessObject->objectVelX, tempPlayerAccessObject->objectCollision.y, tempPlayerAccessObject->objectVelY);





}
