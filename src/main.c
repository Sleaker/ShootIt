#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "game_types.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

// Global Game State Object
Game game;
SDL_Texture* gBackground = NULL;


// Function Declarations 
void close();
void doRendering();

bool handleEvents();
bool loadMedia();
bool init();

SDL_Texture* loadTexture(char* path);

int main(int argc, char** argv) {
	if (!init()) {
		return 1;
	}
	if (!loadMedia()) {
		printf("Failed to load the image!\n");
		return 2;
	}
	bool quit = false;
	while (!quit) {
		quit = handleEvents();
		doRendering();
	}

	close();
	return 0;
}

void close() {
	SDL_DestroyTexture(gBackground);
	gBackground = NULL;

	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	game.renderer = NULL;
	game.window = NULL;

	IMG_Quit();
	SDL_Quit();
}

void doRendering() {
	// Main Rendering loop
	SDL_RenderClear(game.renderer);
	SDL_RenderCopy(game.renderer, gBackground, NULL, NULL);
	SDL_RenderPresent(game.renderer);
}

bool handleEvents() {
	bool quit = false;
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			quit = true;
		}
		else if (e.type == SDL_KEYDOWN) {
			// get the keypress
			switch (e.key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;
			default:
				break;
			}
		}
	}
	return quit;
}

bool loadMedia() {
	bool success = true;
	//Load splash image 
	gBackground = loadTexture("resources/background.png");
	if (gBackground == NULL) {
		printf("Unable to load background image: %s!\n SDL Error: %s\n", "resources/background.png", SDL_GetError());
		success = false;
	}
	return success;
}

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	game.window = SDL_CreateWindow("Shoot IT!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (game.window == NULL) {
		printf("SDL could not initialize a window! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
	if (game.renderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(game.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	if (!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
		printf("SDL_Image could not initialize! SDL_Image Error: %s\n", IMG_GetError());
		return false;
	}


	return true;
}

SDL_Texture* loadTexture(const char* path) {
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL) {
		//TODO: Log an error
		return newTexture;
	}
	newTexture = SDL_CreateTextureFromSurface(game.renderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);
	if (newTexture == NULL) {
		//TODO: Log an error
	}
	return newTexture;
}
