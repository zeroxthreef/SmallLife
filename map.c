#include <SDL2/SDL.h>

#include "scene.h"
#include "map.h"
#include "util.h"

//have the abillity to add blacklisted collision tiles

unsigned long whitelist[900];
unsigned long whitelistAmount = 0;
Object *tempMapObject = NULL;
unsigned long tempID = 0;
SDL_Rect tileSize;



void CreateMap(int tileWidth, int tileHeight, unsigned long textureWidth, unsigned long textureHeight, unsigned long columns, unsigned long rows, const char *TMXloc, const char *tileSheet, const char *backgroundImage){//add abillity to add paralaxxed background
  //this function should be the only one to access the blacklisted tiles
  //TEMPORARY
  unsigned long dataSize = 0, tempTileNum = 0, tempObjectID = 0;
  unsigned char *data = readFile(TMXloc, &dataSize);
  unsigned char *lastPtr = data;
  unsigned long i;
  tileSize.x = 0;
  tileSize.y = 0;
  tileSize.w = tileWidth;
  tileSize.h = tileHeight;



  global.levelWidth = parseNumber("width=\"", data, NULL);
  global.levelHeight = parseNumber("height=\"", data, NULL);

  printf("Map settings found: %lu %lu\n", global.levelWidth, global.levelHeight);
  //printf("%s\n", data);

  tempMapObject = GetObject(AddObject(TYPE_BACKGROUND, tileSize, tileSize, backgroundImage));
  SDL_QueryTexture(tempMapObject->texture, NULL, NULL, &tempMapObject->textureSelect.w, &tempMapObject->textureSelect.h);
  tempMapObject->objectCollision.w = tempMapObject->textureSelect.w;
  tempMapObject->objectCollision.h = tempMapObject->textureSelect.h;




  tempTileNum = parseNumber("<data encoding=\"csv\">\n", data, &lastPtr);
  for(i = 0; i < global.levelWidth * global.levelHeight; i++){//it seems that it puts the tiles up by one in the tmx file
    if( tempTileNum != 0){
      //if putting nothing into the texture doesnt work, draw transparent rects?
      tempObjectID = AddObject(TYPE_TILE, tileSize, (SDL_Rect)findSheet(textureWidth, textureHeight, rows, columns, tempTileNum - 1), tileSheet);
      Object *objTempMap = GetObject(tempObjectID);
      if(CheckWhitelist(tempTileNum)){
        objTempMap->hasCollision = 1;
        //printf("f %u\n", objTempMap->hasCollision);
      } else{
        objTempMap->hasCollision = 0;
        //printf("h %u\n", objTempMap->hasCollision);
      }
      



    }

    if(i < (global.levelWidth * global.levelHeight) - 1){
      if(*(lastPtr + 1) == '\n'){//increment
        lastPtr++;
        tempTileNum = parseNumber("\n", lastPtr, &lastPtr);//usually, look for ",", but if it's a \n, then it wont skip the next line's beginning numer
      } else{
        tempTileNum = parseNumber(",", lastPtr, &lastPtr);//usually, look for ",", but if it's a \n, then it wont skip the next line's beginning numer
      }
    }



    tileSize.x+= tileSize.w;

    if(tileSize.x >= global.levelWidth * tileSize.w){
      tileSize.y+= tileSize.h;
      tileSize.x = 0;
    }
    //tempTileNum = parseNumber(",", lastPtr, &lastPtr);//usually, look for ",", but if it's a \n, then it wont skip the next line's beginning numer
  }

  free(data);


}
//DO THIS FUNCTION BEFORE CREATING THE MAPPPPPPP TODO TODO TODO TOD TODO TODO
void AddCollisionWhiteList(unsigned long tile){//figure out a way to make a list of blacklisted tiles
  whitelist[whitelistAmount] = tile;
  whitelistAmount++;
}
int CheckWhitelist(unsigned long search){
  unsigned long i;
  for(i = 0; i < whitelistAmount; i++){
    if(search == whitelist[i]){
      return 1;
    }
  }
  return 0;
}

void DestroyMap(){
//erase collision blacklist
whitelistAmount = 0;
}
