Mix_Music* music = NULL;
SDL_Event event;

#ifdef _WIN32
int WinMain(int argc, char *argv[]);
#else
int main(int argc, char *argv[]);
#endif

void init();

void GlobalHandler();

void gameLoop();

void destroy();
