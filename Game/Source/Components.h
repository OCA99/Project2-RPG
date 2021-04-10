#pragma once

#include "Point.h"
#include "External/SDL/include/SDL.h"

struct Position
{
	Position(float x, float y) : position(x, y) {}
	Position(fPoint p) : position(p) {}
	Position() : position(0.f, 0.f) {}

	fPoint position;
};

struct Sprite
{
	Sprite(SDL_Texture* sprite, SDL_Rect section) : sprite(sprite), section(section) {}

	SDL_Texture* sprite;
	SDL_Rect section;
};