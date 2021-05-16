#pragma once

#include "Point.h"
#include "Animation.h"
#include "App.h"
#include "DialogSytem.h"
#include "External/SDL/include/SDL.h"
#include "External/PugiXml/src/pugixml.hpp"
#include "Quest.h"
#include "QuestManager.h"

#include <map>
#include <string>
#include <math.h>

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
	Sprite(SDL_Texture* sprite, float scale = 1.0f, int zindex = 0) : sprite(sprite), scale(scale), zindex(zindex) {}

	SDL_Texture* sprite;
	float scale;
	int zindex;
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
		anim->pingpong = false;
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

struct MapCollider
{
	MapCollider(SDL_Rect rect) : rect(rect) {}

	bool Intersects(SDL_Rect other, SDL_Rect& intersection)
	{
		int leftX = std::max(rect.x, other.x);
		int rightX = std::min(rect.x + rect.w, other.x + other.w);
		int topY = std::max(rect.y, other.y);
		int bottomY = std::min(rect.y + rect.h, other.y + other.h);

		if (leftX < rightX && topY < bottomY) {
			intersection.x = leftX;
			intersection.y = topY; 
			intersection.w = rightX - leftX;
			intersection.h = bottomY - topY;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Intersects(SDL_Rect other)
	{
		int leftX = std::max(rect.x, other.x);
		int rightX = std::min(rect.x + rect.w, other.x + other.w);
		int topY = std::max(rect.y, other.y);
		int bottomY = std::min(rect.y + rect.h, other.y + other.h);

		if (leftX < rightX && topY < bottomY) {
			return true;
		}
		else
		{
			return false;
		}
	}

	SDL_Rect rect;
};

struct EventCollider
{
	EventCollider(SDL_Rect rect) : rect(rect) {}

	bool Intersects(SDL_Rect other, SDL_Rect& intersection)
	{
		int leftX = std::max(rect.x, other.x);
		int rightX = std::min(rect.x + rect.w, other.x + other.w);
		int topY = std::max(rect.y, other.y);
		int bottomY = std::min(rect.y + rect.h, other.y + other.h);

		if (leftX < rightX && topY < bottomY) {
			intersection.x = leftX;
			intersection.y = topY;
			intersection.w = rightX - leftX;
			intersection.h = bottomY - topY;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Intersects(SDL_Rect other)
	{
		int leftX = std::max(rect.x, other.x);
		int rightX = std::min(rect.x + rect.w, other.x + other.w);
		int topY = std::max(rect.y, other.y);
		int bottomY = std::min(rect.y + rect.h, other.y + other.h);

		if (leftX < rightX && topY < bottomY) {
			return true;
		}
		else
		{
			return false;
		}
	}

	SDL_Rect rect;
};

struct NPCMover
{
	NPCMover(float speed) : speed(speed) {}

	float speed;
};

struct NPCMoverAuto
{
	NPCMoverAuto(float speed, int x, int y) : speed(speed), x(x), y(y) {}

	int x;
	int y;
	float speed;
};

struct NPCCollider
{
	NPCCollider(SDL_Rect rect) : rect(rect) {}

	bool Intersects(SDL_Rect other, SDL_Rect& intersection)
	{
		int leftX = std::max(rect.x, other.x);
		int rightX = std::min(rect.x + rect.w, other.x + other.w);
		int topY = std::max(rect.y, other.y);
		int bottomY = std::min(rect.y + rect.h, other.y + other.h);

		if (leftX < rightX && topY < bottomY) {
			intersection.x = leftX;
			intersection.y = topY;
			intersection.w = rightX - leftX;
			intersection.h = bottomY - topY;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Intersects(SDL_Rect other)
	{
		int leftX = std::max(rect.x, other.x);
		int rightX = std::min(rect.x + rect.w, other.x + other.w);
		int topY = std::max(rect.y, other.y);
		int bottomY = std::min(rect.y + rect.h, other.y + other.h);

		if (leftX < rightX && topY < bottomY) {
			return true;
		}
		else
		{
			return false;
		}
	}

	SDL_Rect rect;
};

struct DialogTrigger {
	DialogTrigger(SDL_Rect rect, std::string filename, std::string dialogName) : rect(rect), filename(filename), dialogName(dialogName) {
		app->dialog->LoadDialog(filename.c_str());
	}

	bool Intersects(SDL_Rect other, SDL_Rect& intersection)
	{
		int leftX = std::max(rect.x, other.x);
		int rightX = std::min(rect.x + rect.w, other.x + other.w);
		int topY = std::max(rect.y, other.y);
		int bottomY = std::min(rect.y + rect.h, other.y + other.h);

		if (leftX < rightX && topY < bottomY) {
			intersection.x = leftX;
			intersection.y = topY;
			intersection.w = rightX - leftX;
			intersection.h = bottomY - topY;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Intersects(SDL_Rect other)
	{
		int leftX = std::max(rect.x, other.x);
		int rightX = std::min(rect.x + rect.w, other.x + other.w);
		int topY = std::max(rect.y, other.y);
		int bottomY = std::min(rect.y + rect.h, other.y + other.h);

		if (leftX < rightX && topY < bottomY) {
			return true;
		}
		else
		{
			return false;
		}
	}

	SDL_Rect rect;
	std::string filename;
	std::string dialogName;

	bool started = false;
};

struct LogoFade {
	LogoFade(float speed) : speed(speed) {}

	float speed;
};

struct MenuFade
{
	MenuFade(float speed) : speed(speed) {}

	float speed;
}; 

struct SceneFade
{
	SceneFade(float speed, int squares) : speed(speed), squares(squares) {}

	float speed;
	float squares;
};

struct Member;

struct BattleMember
{
	BattleMember(Member* member) : member(member) {}

	Member* member;
};

struct Quest;


struct QuestList
{
	SString receiver;

	List<Quest*> qActive;
	List<Quest*> qInactive;

	fPoint position;

	bool activeQuest = true;
	bool inactiveQuest = true;

	void SetReceiver(SString info)
	{
		receiver = info;
	}

	void SetPosition(fPoint pos)
	{
		position = pos;
	}

	void LoadActive(List<Quest*>* active)
	{
		Quest* first = active->start->data;
		for (int i = 0; i <= active->Count(); i++)
		{
			if (first->demandingNPC == receiver)
			{
				qActive.Add(first);
			}
			if(active->start->next != nullptr) first = active->start->next->data;
		}
		if (qActive.Count() == 0)
		{
			activeQuest = false;
		}

	}
	
	void LoadInactive(List<Quest*>* inactive)
	{
		Quest* first = inactive->start->data;
		for (int i = 0; i <= inactive->Count(); i++)
		{
			if (first->demandingNPC == receiver)
			{
				qInactive.Add(first);
			}
			if (inactive->start->next != nullptr) first = inactive->start->next->data;

		}
		if (qInactive.Count() == 0)
		{
			inactiveQuest = false;
		}


	}


	
	pugi::xml_node node;

	bool active = false;
};


