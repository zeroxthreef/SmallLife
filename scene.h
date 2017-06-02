typedef struct{
  SDL_Texture* texture;
  SDL_Rect textureSelect;
  SDL_Rect objectCollision;//this doesnt have the accuracy of floats, so that might take more work than I want. Use this to get world coordinates
  SDL_Rect objectTransform;
  float objectVelX;
  float objectVelY;
  float objectX;
  float objectY;
  float objectRotation;
  unsigned short isFlipped;
  unsigned short hasCollision;
  unsigned short hasPhysics;
  unsigned short used;
  unsigned short visible;
  unsigned short clipVisible;
  unsigned long type;
}Object;

struct Global{
  unsigned long state;
  unsigned short isDebug;
  unsigned short isNoclip;
  unsigned short health;
  unsigned short points;
  float gravity;
  unsigned short playerMoveLeft;
  unsigned short playerMoveRight;
  unsigned short playerJump;
  unsigned short playerCrouch;
  unsigned short playerCollided;
  unsigned short playerUse;//use
  unsigned short playerHasGun;//use
  unsigned short inCinematic;
  unsigned short canExit;
  unsigned short isPaused;
  float cameraZoom;
  float cameraX;
  float cameraY;
  float cameraWidth;
  float cameraHeight;
  int windowWidth;
  int windowHeight;
  unsigned long levelWidth;
  unsigned long levelHeight;
  Uint8 hueR;
  Uint8 hueG;
  Uint8 hueB;
}global;

enum ObjectTypes {
  TYPE_PLAYER,
  TYPE_TILE,
  TYPE_BACKGROUND,
  TYPE_STRING_CHARACTER,//make a bunch of these and make a text manager or something that creates and destroys and renders numbers. Idealy, you should destroy all text
  //each render pass, and then re render it
  TYPE_BOSS,
  TYPE_ANT,
  TYPE_BULLET,
  TYPE_RANDOMOBJECT,
};


unsigned long AddObject(unsigned long type, SDL_Rect collisionRect, SDL_Rect selectTexture, const char *textureUrl);

int DeleteObject(unsigned long ID);

Object *GetObject(unsigned long ID);

Object *GetBaseObject();

unsigned long GetObjectCount();

int CreateScene();

int DestroyScene();
