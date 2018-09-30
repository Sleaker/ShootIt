#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string.h>

#include "game_types.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int MAX_FPS = 60;
const int MAX_TICKS_PER_SECONDS = 1000 / MAX_FPS;

// Global Game State Object
Game game;
SDL_Texture* gBackground = NULL;
SpriteSheet font;


// Function Declarations
void closeGame();
void doRendering();

bool handleEvents();
bool loadMedia();
bool init();

SDL_Texture* loadTexture(const char* path);

int main(int argc, char** argv) {
	if (!init()) {
		printf("Failed to initialize\n");
		return 1;
	}
	printf("Loading Media\n");
	if (!loadMedia()) {
		printf("Failed to load the image!\n");
		return 2;
	}
	printf("Main game loop\n");
	bool quit = false;
	while (!quit) {
		quit = handleEvents();
		doRendering();
		Uint32 currentTicks = SDL_GetTicks();
		
		if ( currentTicks - game.lastFrameTime < MAX_TICKS_PER_SECONDS) {
			SDL_Delay(MAX_TICKS_PER_SECONDS - (currentTicks - game.lastFrameTime));
		}
		currentTicks = SDL_GetTicks();
		//printf("Frame Time: %i ms\n", currentTicks - game.lastFrameTime);
		game.lastFrameTime = currentTicks;
		
	}

	closeGame();
	return 0;
}

void closeGame() {
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
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			quit = true;
		}
		else if (e.type == SDL_KEYDOWN) {
			printf("Key press detected.\n");
			// get the keypress
			switch (e.key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;
			default:
				break;
			}
			printf("Key: %d\n", e.key.keysym.sym);
		}
	}
	return quit;
}

bool loadMedia() {
	//Load splash image
	gBackground = loadTexture("resources/background.png");
	if (gBackground == NULL) {
		printf("Unable to load background image: %s!\n SDL Error: %s\n", "resources/background.png", SDL_GetError());
		return false;
	}
	font.texture = loadTexture("resources/fonts/12x12.png");
	if (font.texture == NULL) {
		printf("Unable to load font image: %s!\n SDL Error: %s\n", "resources/fonts/12x12.png", SDL_GetError());
		return false;
	}
	Uint32* format = NULL;
	int* access = NULL;
	int width = 0;
	int height = 0;
	SDL_QueryTexture(font.texture, format, access, &width, &height);
	font.spritesPerRow = 16;
	font.spriteHeight = height / font.spritesPerRow;
	font.spriteWidth = width / font.spritesPerRow;

	return true;
}

bool init() {
	printf("Initializing SDL2\n");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	game.lastFrameTime = SDL_GetTicks();

	printf("Creating the game window\n");
	game.window = SDL_CreateWindow("Shoot IT!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (game.window == NULL) {
		printf("SDL could not initialize a window! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	printf("Creating the game renderer\n");
	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_SOFTWARE);
	if (game.renderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(game.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	printf("Initializing SDL_Image & PNG Support\n");
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
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

Font* loadFontTexture(const char* path) {
	SpriteSheet* sheet = NULL;
	Font* font = NULL;
	SDL_Texture* newTexture = NULL;
	SDL_Surface* surface = IMG_Load(path);
	if (surface == NULL) {
		return font;
	}
	SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
	if (formattedSurface == NULL) {
		return font;
	}
	newTexture = SDL_CreateTexture(game.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
	if (newTexture == NULL) {
		printf("Failed to create blank texture! SDL Error: %s\n", SDL_GetError());
	}
	SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
	SDL_LockTexture(newTexture, &formattedSurface->clip_rect, sheet->pixels, &sheet->pitch);
	memcpy(sheet->pixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);
	
	sheet->spriteWidth = formattedSurface->w;
	sheet->spriteHeight = formattedSurface->h;
	sheet->spritesPerRow = 16;
	Uint32* pixels = (Uint32*) sheet->pixels;
	int pixelCount = (sheet->pitch / 4) * sheet->spriteHeight;
	Uint32 colorKey = SDL_MapRGB(formattedSurface->format, 0x00, 0x00, 0x00);
	Uint32 transparent = SDL_MapRGBA(formattedSurface->format, 0x00, 0x00, 0x00, 0x00);
	for (int i = 0; i < pixelCount; ++i) {
		if (pixels[i] == colorKey) {
			pixels[i] = transparent;
		}
	}
	SDL_UnlockTexture(newTexture);
	font->sheet = sheet;

	SDL_FreeSurface(formattedSurface);
	SDL_FreeSurface(surface);


	return font;
}
