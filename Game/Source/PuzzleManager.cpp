#include "PuzzleManager.h"
#include "App.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Render.h"

#include <iostream>

PuzzleManager::PuzzleManager() : Module()
{
	name.Create("puzzles");
}

// Destructor
PuzzleManager::~PuzzleManager()
{}

// Called before render is available
bool PuzzleManager::Awake()
{
	LOG("Loading Party");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool PuzzleManager::Start()
{
	noteTex = app->tex->Load("Textures/Puzzles/note.png");
	passwordTex = app->tex->Load("Textures/Puzzles/code_lock.png");
	return true;
}

// Called each loop iteration
bool PuzzleManager::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool PuzzleManager::Update(float dt)
{
	bool ret = true;

	if (app->scene->currentScene->type == Scene::TYPE::MAP)
	{
		switch (currentPuzzle)
		{
		case PuzzleManager::ActivePuzzle::NONE:
			break;
		case PuzzleManager::ActivePuzzle::SEWERENTRANCE:
			SewerEntrancePuzzle();
			break;
		case PuzzleManager::ActivePuzzle::CAGEPUZZLE:
			CagePuzzle();
			break;
		case PuzzleManager::ActivePuzzle::MAGICGEM:
			MagicGemPuzzle();
			break;
		default:
			break;
		}
	}

	return ret;
}

// Called each loop iteration
bool PuzzleManager::PostUpdate(float dt)
{
	bool ret = true;


	if (app->scene->currentScene->type == Scene::TYPE::MAP)
	{
		switch (currentPuzzle)
		{
		case PuzzleManager::ActivePuzzle::NONE:
			break;
		case PuzzleManager::ActivePuzzle::SEWERENTRANCE:
			
			break;
		case PuzzleManager::ActivePuzzle::CAGEPUZZLE:
			DrawCagePuzzle();
			break;
		case PuzzleManager::ActivePuzzle::MAGICGEM:
		
			break;
		default:
			break;
		}
	}


	return ret;
}

// Called before quitting
bool PuzzleManager::CleanUp()
{
	return true;
}

void PuzzleManager::SewerEntrancePuzzle()
{
	if (key1 == true && key2 == true && key3 == true)
	{
		if (!completed) completed = true;
		currentPuzzle = ActivePuzzle::CAGEPUZZLE;
	}
}

void PuzzleManager::CagePuzzle()
{


}

void PuzzleManager::MagicGemPuzzle()
{
}

void PuzzleManager::DrawCagePuzzle()
{
	if (key1 && !key2)
	{
		
		//app->render->DrawRectangle({ 0,0,200,200 }, 255, 255, 255, 255,true,false);
		app->render->DrawTexture(noteTex, 0, 0, &SDL_Rect({ 0, 0, 1280, 720 }), 0.5f, 0, 0, 0, 0, false);
	}
	if (key2 && !key1)
	{
		app->render->DrawTexture(passwordTex, 0, 0, &SDL_Rect({ 0, 0, 1280, 720 }), 0.5f, 0, 0, 0, 0, false);

	}

}
