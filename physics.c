#include <SDL2/SDL.h>
#include "scene.h"
#include "physics.h"

Object *tempPhysicsObject = NULL;
Object *tempPhysicsObjectTester = NULL;



void CreatePhysics(){

}
void UpdatePhysics(){
  unsigned long i;
  for(i = 0; i <= GetObjectCount(); i++){//map tiles should have collision, but not physics
    tempPhysicsObject = GetObject(i);//only do physics and collision detection in here. Bug ai can detect the ground stuff
    tempPhysicsObject->objectCollision.x += (int)tempPhysicsObject->objectVelX;
    tempPhysicsObject->objectCollision.y += (int)tempPhysicsObject->objectVelY;

    if(tempPhysicsObject->hasPhysics){//if any object has physics enabled
      unsigned long j;
      unsigned short collided = 0;

      for(j = 0; j < GetObjectCount(); j++){
        tempPhysicsObjectTester = GetObject(j);//get the latest object to test against

        if(j != i && tempPhysicsObjectTester->hasCollision){//if it doesnt equal itself
          if(SDL_HasIntersection(&tempPhysicsObject->objectCollision, &tempPhysicsObjectTester->objectCollision)){//test to see if there is a collision with the latest object

            collided = 1;
            SDL_Rect collisionDiff;
            SDL_IntersectRect(&tempPhysicsObject->objectCollision, &tempPhysicsObjectTester->objectCollision, &collisionDiff);

            //check where the colliding obect is, whether that's right, left, up, or down. Maybe even a corner
            if(collisionDiff.h > collisionDiff.w){//if the height is bigger, that means an x collision
              //x xollision
              //tempPhysicsObject->objectCollision.y
              if(tempPhysicsObject->objectCollision.x > tempPhysicsObjectTester->objectCollision.x){
                tempPhysicsObject->objectCollision.x += collisionDiff.w;
              } else if(tempPhysicsObject->objectCollision.x < tempPhysicsObjectTester->objectCollision.x){
                tempPhysicsObject->objectCollision.x -= collisionDiff.w;
              }


            }else{//make sure all trade velocities unless it's a map tile and only has colision
              //tempPhysicsObject->objectCollision.y = collisionDiff.y - tempPhysicsObject->objectCollision.h;
              if(tempPhysicsObject->objectCollision.y > tempPhysicsObjectTester->objectCollision.y){
                tempPhysicsObject->objectCollision.y += collisionDiff.h;
              } else if(tempPhysicsObject->objectCollision.y < tempPhysicsObjectTester->objectCollision.y){
                tempPhysicsObject->objectCollision.y -= collisionDiff.h;
              }
            }

            //===========================================================================================================
            if(tempPhysicsObjectTester->hasPhysics){//switch velocities, because that's what things do when they collide
              float tempVelX = tempPhysicsObjectTester->objectVelX;
              float tempVelY = tempPhysicsObjectTester->objectVelY;

              tempPhysicsObjectTester->objectVelX = tempPhysicsObject->objectVelX;
              tempPhysicsObjectTester->objectVelY = tempPhysicsObject->objectVelY;

              tempPhysicsObject->objectVelX = tempVelX;
              tempPhysicsObject->objectVelY = tempVelY;


            } else{//only apply it to the current physics object then :(
              tempPhysicsObject->objectVelX = tempPhysicsObjectTester->objectVelX;//if it's a map, it's 0
              tempPhysicsObject->objectVelY = tempPhysicsObjectTester->objectVelY;//if it's a map, it's 0
            }








            //printf("lol: []%d %d %d %d] player %d %d\n", collisionDiff.x, collisionDiff.y, collisionDiff.w, collisionDiff.h, tempPhysicsObject->objectCollision.x, tempPhysicsObject->objectCollision.y);
          }
        }
        if(tempPhysicsObject->objectCollision.x < 0){
          tempPhysicsObject->objectCollision.x = 0;
        }
        if(tempPhysicsObject->objectCollision.y < 0){
          tempPhysicsObject->objectCollision.y = 0;
        }
        if(tempPhysicsObject->objectCollision.x + tempPhysicsObject->objectCollision.w > global.levelWidth * 15){
          tempPhysicsObject->objectCollision.x = global.levelWidth * 15 - tempPhysicsObject->objectCollision.w;
        }
        if(tempPhysicsObject->objectCollision.y + tempPhysicsObject->objectCollision.h > global.levelHeight * 15){
          tempPhysicsObject->objectCollision.y = global.levelHeight * 15 - tempPhysicsObject->objectCollision.h;
        }


      }
      if(!collided){
        tempPhysicsObject->objectVelY += global.gravity;
        if(tempPhysicsObject->type == TYPE_PLAYER){
          global.playerCollided = 0;

        }
      }else{
        if(tempPhysicsObject->type == TYPE_PLAYER){
          global.playerCollided = 1;

        }
      }

    }
    //tempPhysicsObject.objectCollision.x
  }
}
