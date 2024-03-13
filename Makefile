snek: main.cpp
	gcc -o snek main.cpp -std=c++11 -I/opt/homebrew/Cellar/raylib/5.0/include -L/opt/homebrew/Cellar/raylib/5.0/lib -lraylib

websnek: main.cpp
	clang --target=wasm32 -std=c++11 -Oz -o snek.wasm main.cpp
