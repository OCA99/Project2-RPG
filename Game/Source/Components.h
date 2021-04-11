#pragma once

#include "Point.h"
#include "Animation.h"
#include "External/SDL/include/SDL.h"

#include <map>
#include <string>

struct Position
{
	Position(float x, float y) : position(x, y), deltaMovement(0.f, 0.f) {}
	Position(fPoint p) : position(p), deltaMovement(0.f, 0.f) {}
	Position() : position(0.f, 0.f), deltaMovement(0.f, 0.f) {}

	void Translate(fPoint dm)
	{
		deltaMovement = dm;
	}

	fPoint position;
	fPoint deltaMovement;
};

struct Sprite
{
	Sprite(SDL_Texture* sprite, SDL_Rect section) : sprite(sprite) {}

	SDL_Texture* sprite;
};

struct Animator
{
	Animator() {}
	void CreateAnimation(std::string name, const SDL_Rect& rect, int rows = 1, int columns = 1, float speed = 1.0f, bool loop = true, bool pingpong = false)
	{
		Animation* anim = new Animation();
		anim->GenerateAnimation(rect, rows, columns);
		anim->speed = speed;
		anim->loop = loop;
		anim->pingpong = pingpong;
		animations.insert(std::make_pair(name, anim));
	}

	void SetAnimation(std::string name)
	{
		if (animations.find(name) != animations.end())
			currentAnimation = animations[name];
	}

	std::map<std::string, Animation*> animations;
	Animation* currentAnimation = nullptr;
};

struct KeyboardMovement
{
	KeyboardMovement(float speed) : speed(speed) {}

	float speed;
};

struct Humanoid
{
	Humanoid() {};
};

struct CameraFollower
{
	CameraFollower() {};
};