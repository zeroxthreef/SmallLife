COMPILENAMENIX = SmallLife
COMPILENAMEWIN = SmallLife.exe

SOURCES = $(wildcard *.c)

NIXCC = gcc
WINCC = i686-w64-mingw32-gcc

C_OBJ = $(SOURCES:.o)

LIBSNIX = -lSDL2 -lm -lSDL2_image -lSDL2_mixer
LIBSWIN = -lSDL2 -lm -lSDL2_image -lSDL2_mixer -mwindows
#LIBSWIN = -lmingw32 -lSDL2main -lSDL2 -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc

COMPILE_FLAGS = -g -Wall -o

linux:
	$(NIXCC) $(COMPILE_FLAGS) $(COMPILENAMENIX) $(SOURCES) $(LIBSNIX)

windows:
	$(WINCC) $(COMPILE_FLAGS) $(COMPILENAMEWIN) $(SOURCES) $(LIBSWIN)

mac:
	$(NIXCC) $(COMPILE_FLAGS) $(COMPILENAMENIX) $(C_OBJ) $(LIBSNIX)

run:
	./$(COMPILENAME)

clean:
	rm -f *.o
