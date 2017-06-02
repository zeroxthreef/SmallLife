//#ifdef _WIN32
//#include <windows.h>
//#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "core.h"
#include "scene.h"
#include "render.h"
#include "menu.h"
#include "level1.h"
#include "level2.h"
#include "level3.h";
//#include "level4.h";
//#include "level5.h";


Object *accessObject = NULL;


Uint8 *keyState = NULL;
//Uint8 *mouseState = NULL;

#ifdef _WIN32
int WinMain(int argc, char *argv[]){
	/*FreeConsole();
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
	freopen("CON", "r", stdin);
	SetConsoleTitle("Debug Console");*/
#else
int main(int argc, char *argv[]){
#endif
	init();
	gameLoop();
	destroy();

	return 0;
}

void init(){
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

	//init other stuff
	CreateScene();
	//scene.debug = True;
	InitRender();

	printf("0xDEADAF mini engine by 0x3F specifically for ludum dare\n");
	printf("Done initializing\n");
	printf("Sending excecution to gameloop\n");
	//the states do most of the work here
	//slot 0 is always the camera
	//slot 1 is always the player
	//slot 2 is always the rendered level texture <--- decided not to do these things. Map rendering works as dynamic
	//the rest is up to choice
}

void GlobalHandler(){
	static int backtickPressed = 0, plusPressed = 0, minusPressed = 0, vPressed = 0;
	keyState = SDL_GetKeyboardState(NULL);

	if(keyState[SDL_SCANCODE_ESCAPE]){

		global.canExit = 1;
	}
	if(keyState[SDL_SCANCODE_GRAVE]){
		if(!backtickPressed){
			if(global.isDebug){
					printf("Debug mode disabled\n");
						global.isDebug = 0;
					} else{
						printf("Debug mode enabled\n");
						global.isDebug = 1;
					}
					backtickPressed = 1;
			}
	} else{
		backtickPressed = 0;
	}
	if(keyState[SDL_SCANCODE_V]){//isNoclip
		if(!vPressed){
			if(global.isNoclip){
					printf("Noclip disabled\n");
						global.isNoclip = 0;
					} else{
						printf("Noclip enabled\n");
						global.isNoclip = 1;
					}
					vPressed = 1;
			}
	} else{
		vPressed = 0;
	}

	if(!global.inCinematic){
		if(keyState[SDL_SCANCODE_A]){
			global.playerMoveLeft = 1;
		} else{
			global.playerMoveLeft = 0;
		}
		if(keyState[SDL_SCANCODE_D]){
			global.playerMoveRight = 1;
		} else{
			global.playerMoveRight = 0;
		}
		if(keyState[SDL_SCANCODE_SPACE]){
			global.playerJump = 1;
		} else{
			global.playerJump = 0;
		}
		if(keyState[SDL_SCANCODE_LCTRL]){
			global.playerCrouch = 1;
		} else{
			global.playerCrouch = 0;
		}
		if(keyState[SDL_SCANCODE_RETURN]){
			global.playerUse = 1;
		} else{
			global.playerUse = 0;
		}
	} else{
		global.playerCrouch = 0;
		global.playerJump = 0;
		global.playerMoveRight = 0;
		global.playerMoveLeft = 0;
		global.playerUse = 0;
	}

	if(global.isDebug){
		if(keyState[SDL_SCANCODE_KP_MINUS]){
			if(!minusPressed){
					global.state-= 1;
					minusPressed = 1;
					printf("State-: %lu\n", global.state);
				}
		} else{
			minusPressed = 0;
		}
		if(keyState[SDL_SCANCODE_KP_PLUS]){
			if(!plusPressed){
					global.state+= 1;;
					plusPressed = 1;
					printf("State+: %lu\n", global.state);
				}
		} else{
			plusPressed = 0;
		}
		if(keyState[SDL_SCANCODE_UP]){
			global.cameraY-= 0.4;
		}
		if(keyState[SDL_SCANCODE_DOWN]){
			global.cameraY+= 0.4;
		}
		if(keyState[SDL_SCANCODE_LEFT]){
			global.cameraX-= 0.4;
		}
		if(keyState[SDL_SCANCODE_RIGHT]){
			global.cameraX+= 0.4;
		}
		if(keyState[SDL_SCANCODE_1]){
			global.cameraZoom += 0.1f * global.cameraZoom;
		}
		if(keyState[SDL_SCANCODE_2]){
			global.cameraZoom -= 0.1f * global.cameraZoom;
		}

	}



	//============================================================================
	if(SDL_QuitRequested()){
		global.canExit = 1;
	}
	//============================================================================
	//============================================================================
	//============================================================================
	//============================================================================
	//============================================================================
	//============================================================================

	//handle events for pausing and such and increminting states and moving camera
	static unsigned long prevState = 9999;//random ass numer to throw it off and init the new level;

	if(prevState != global.state){
		switch (prevState) {//current needs to inot, and previous needs to use prev in switch
			//========================================================================
			case 0:
				//destroy the main menu here
				DestroyMenu();
			break;
			//========================================================================
			case 1:
				//Destroylevel 1
				DestroyLevel1();
			break;
			//========================================================================
			case 2:
				//Destroylevel 2
				DestroyLevel2();
			break;
			//========================================================================
			case 3:
				DestroyLevel3();
			break;
			//========================================================================
			case 4:

			break;
			//========================================================================
			case 5:

			break;

			//========================================================================
		}
		switch (global.state) {//current needs to inot, and previous needs to use prev in switch
			//========================================================================
			case 0:
				//init the main menu here
				CreateMenu();
			break;
			//========================================================================
			case 1:
				//initlevel 1
				CreateLevel1();
			break;
			//========================================================================
			case 2:
				//initlevel 2
				CreateLevel2();
			break;
			//========================================================================
			case 3:
				CreateLevel3();
			break;
			//========================================================================
			case 4:

			break;
			//========================================================================
			case 5:

			break;
		}
	}
	prevState = global.state;



}

void gameLoop(){
	const int fps = 1000/60;//, printFps = 1000/2;

	int lastTime = SDL_GetTicks(), skipTime = 0, currentTime = 0, lastframeLag = 0;



	while(!global.canExit){
		GlobalHandler();
		//printf("STATE %lu\n", global.state);
		switch (global.state) {
			//========================================================================
			case 0:
				//put the main menu here
				//events
				//logic
				EventMenu();
				LogicMenu();
			break;
			//========================================================================
			case 1:
				//level 1
				//events
				//logic
				EventLevel1();
				LogicLevel1();
			break;
			//========================================================================
			case 2:
				//level 2
				EventLevel2();
				LogicLevel2();
				//events
				//logic
			break;
			//========================================================================
			case 3:
			EventLevel3();
			LogicLevel3();
			break;
			//========================================================================
			case 4:

			break;
			//========================================================================
			case 5:

			break;
		}

		if(!lastframeLag){
			//render
			switch (global.state) {
				//======================================================================
				case 0:
					//put the main menu here
					//render
					RenderMenu();
				break;
				//======================================================================
				case 1:
					//level 1
					//render
					RenderLevel1();
				break;
				//======================================================================
				case 2:
					//level 2
					//render
					RenderLevel2();
				break;
				//======================================================================
				case 3:
					RenderLevel3();
				break;
				//========================================================================
				case 4:

				break;
				//========================================================================
				case 5:

				break;
			}
		} else{
			lastframeLag = 0;
		}


		lastTime += fps;
		currentTime = SDL_GetTicks();

		skipTime = lastTime - currentTime;


		//printf("FPS: %u\n", skipTime * currentTime);

		if(skipTime >= 0){
			//printf("%u %u\n", skipTime, SDL_GetTicks());
			SDL_Delay(skipTime);
		} else{
			//printf("Lagging\n");
			lastframeLag = 1;
		}

		//==========================================================================
		SDL_PumpEvents();
		//SDL_Delay(15);
	}
}

void destroy(){
	//destroyPlayer();
	//destroyRender();
	DestroyScene();
	DestroyRender();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
	printf("Destroyed everything successfully\n");
}
