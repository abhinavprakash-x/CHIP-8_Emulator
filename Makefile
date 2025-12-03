all:
	g++ main.cpp chip8.cpp -o main.exe -I "C:\Libraries\SDL3\x86_64-w64-mingw32\include" -L "C:\Libraries\SDL3\x86_64-w64-mingw32\lib" -lSDL3