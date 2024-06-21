gcc src/main.c -o bin/emulator -lSDL2 -lGL -lm -lpthread -ldl -lrt -lX11
./bin/emulator rom.rom