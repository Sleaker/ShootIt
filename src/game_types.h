#pragma once

#include <SDL.h>
#include <inttypes.h>
#include <stdint.h>

typedef struct game {
	SDL_Window* window;
	SDL_Renderer* renderer;
	Uint32 lastFrameTime;
} Game;

typedef struct spritesheet {
	SDL_Texture* texture;
	uint_fast8_t spriteWidth;
	uint_fast8_t spriteHeight;
	uint_fast8_t spritesPerRow;

	void* pixels;
	int pitch;
} SpriteSheet;


typedef struct font {
	SpriteSheet* sheet;
	SDL_Rect chars[256];
	int newLine, space;
} Font;
