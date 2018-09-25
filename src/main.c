#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
SDL_Window* gWindow = NULL;
SDL_Texture* gBackground = NULL;
SDL_Renderer* gRenderer = NULL; // Global rendering Object

/* Function Declarations */

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

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}

void doRendering() {
	// Main Rendering loop
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, gBackground, NULL, NULL);
	SDL_RenderPresent(gRenderer);
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

	gWindow = SDL_CreateWindow("Shoot IT!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		printf("SDL could not initialize a window! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	if (!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
		printf("SDL_Image could not initialize! SDL_Image Error: %s\n", IMG_GetError());
		return false;
	}

	return true;
}

SDL_Texture* loadTexture(char* path) {
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL) {
		//TODO: Log an error
		return newTexture;
	}
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);
	if (newTexture == NULL) {
		//TODO: Log an error
	}
	return newTexture;
}
