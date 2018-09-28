#pragma once

#include <SDL.h>
#include <inttypes.h>

typedef struct game {
	SDL_Window* window;
	SDL_Renderer* renderer;
} Game;

typedef struct spritesheet {
	SDL_Texture* texture;
	uint_fast8_t spriteWidth;
	uint_fast8_t spriteHeight;
	uint_fast8_t spritesPerRow;
} SpriteSheet;
