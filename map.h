void CreateMap(int tileWidth, int tileHeight, unsigned long textureWidth, unsigned long textureHeight, unsigned long columns, unsigned long rows, const char *TMXloc, const char *tileSheet, const char *backgroundImage);

void AddCollisionWhiteList(unsigned long tile);

int CheckWhitelist(unsigned long search);

void DestroyMap();
