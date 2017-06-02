#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "util.h"
#include "scene.h"

SDL_Surface *loadSurface = NULL;
SDL_Renderer *utilRenderer = NULL;

float toAngle(float num){
  return num* 180 / 3.14159265;
}
float toRadian(float num){
  return num * 3.14159265 / 180;
}

void initUtil(SDL_Renderer **Renderer){
  utilRenderer = *Renderer;
}

int createTex(SDL_Texture **tex, const char *URL){
  int pass = 1;
  loadSurface = IMG_Load(URL);
  if(loadSurface == NULL){
    pass--;
  }
  *tex = SDL_CreateTextureFromSurface(utilRenderer, loadSurface);
  SDL_SetTextureBlendMode(*tex, SDL_BLENDMODE_BLEND);
  SDL_FreeSurface(loadSurface);
  return pass;
}

SDL_Renderer *giveRenderer(){
  return utilRenderer;
}

SDL_Rect findSheet(unsigned long width, unsigned long height, unsigned long rows, unsigned long columns, unsigned long number){//image width, image height
  SDL_Rect tempRect;//used to select it
  tempRect.x = 0;
  tempRect.y = 0;
  tempRect.w = 0;
  tempRect.h = 0;
  unsigned long tilewidth = width / columns;
  unsigned long tileheight = height / rows;
  unsigned long currectColumn = 0;
  unsigned long currentRow = 0;


  if(columns <= number){
    while (columns <= number){
      currentRow++;
      number -= columns;
      currectColumn = number;
    }
  } else {
    currectColumn = number;
  }

  tempRect.x = currectColumn * tilewidth;
  tempRect.y = currentRow * tileheight;
  tempRect.w = tilewidth;
  tempRect.h = tileheight;

  return tempRect;
}

unsigned char *readFile(const char *location, unsigned long *sizePtr){
  FILE *f;
  unsigned long size;
  unsigned char *dataPtr;
  f = fopen(location, "rb");
  if(f == NULL){
    printf("Error cant find\n");
    return (unsigned char *)NULL;
  }

  fseek(f,0,SEEK_END);
  size = ftell(f);
  rewind(f);

  dataPtr = (unsigned char *)malloc(size + 1);

  fread(dataPtr, sizeof(unsigned char), size, f);

  if(dataPtr == NULL){
    printf("Outta memory\n");
    return (unsigned char *)NULL;
  }


  //printf("neat [%c]\n", dataPtr[1]);
  fclose(f);
  printf("Found %s which is %lu and the first byte is %c\n", location, size, dataPtr[0]);
  *sizePtr = size;
  return dataPtr;
}

unsigned long parseNumber(const char *search, unsigned char *data, unsigned char **endData){
  unsigned long number = 0;
  char *tempDataPtr = strstr((signed char *)data, search);//this sets this one up just fine
  char *tempString = NULL;
  unsigned long tempStringLen = 0;
  if(tempDataPtr == NULL){
    //if(global.isDebug){
      printf("Couldnt find %s in TMX file\n", search);
    //}
  } else{
    //printf("Found [%s]\n", search);
    tempDataPtr += strlen(search);//gets to the end of found string in pplace

    //printf("%c\n", *tempDataPtr);
    if(isdigit(*tempDataPtr) != 0){//use levelWidth
      while(isdigit(tempDataPtr[tempStringLen]) != 0){
        tempStringLen++;
        //printf("%c", tempDataPtr[tempStringLen]);
        //printf("Num length %lu\n", tempStringLen);
      }
      tempString = (char *)calloc(tempStringLen + 1, sizeof(char));

      memcpy(tempString, tempDataPtr, tempStringLen);
      tempString[tempStringLen + 1] = '\0';
      //printf("%s %lu\n", tempString, tempStringLen);

      //number = (unsigned long)strtol(tempString, NULL, 10);//no
      sscanf(tempString, "%lu", &number);
      //number = 3;


      //printf("Final: %lu\n", number);

      if(endData != NULL)
        *endData = (unsigned char *)(tempDataPtr + tempStringLen);

      free(tempString);
    }
    //printf("\n");
  }
  return number;
}
