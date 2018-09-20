#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Surface* loadedImage = NULL;


bool handleEvents() {
	bool quit = false;
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			quit = true;
		}
	}
	return quit;
}

void close() {
	SDL_FreeSurface(loadedImage);
	loadedImage = NULL;

	SDL_FreeSurface(surface);
	surface = NULL;

	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

bool loadMedia() { 
	bool success = true; 
	//Load splash image 
	loadedImage = IMG_Load("resources/background.png");
	if( loadedImage == NULL ) { 
		printf( "Unable to load image %s! SDL Error: %s\n", "resources/background.png", SDL_GetError() ); 
		success = false; 
	} 
	return success; 
}

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else {
		window = SDL_CreateWindow("Shoot IT!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("SDL could not initialize a window! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			surface = SDL_GetWindowSurface(window);
			SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xCC, 0xCC, 0xCC));
			SDL_UpdateWindowSurface(window);
		}
	}

	return true;
}

int main(int argc, char** argv) {
	if (!init()) {
		return 1;
	}
	if (!loadMedia()) {
		printf("Failed to load the image!\n");
	}
	else {
		SDL_BlitSurface(loadedImage, NULL, surface, NULL);
		SDL_UpdateWindowSurface(window);
	}
	bool quit = false;
	while (!quit) {
		quit = handleEvents();
	}

	close();
	return 0;
}
