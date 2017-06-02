float toAngle(float num);

float toRadian(float num);

void initUtil(SDL_Renderer **Renderer);

int createTex(SDL_Texture **tex, const char *URL);

SDL_Renderer *giveRenderer();

SDL_Rect findSheet(unsigned long width, unsigned long height, unsigned long rows, unsigned long columns, unsigned long number);

unsigned char *readFile(const char *location, unsigned long *sizePtr);

unsigned long parseNumber(const char *search, unsigned char *data, unsigned char **endData);
