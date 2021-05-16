#include "PuzzleManager.h"
#include "App.h"
#include "SceneManager.h"
#include "Scene.h"

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

	return ret;
}

// Called before quitting
bool PuzzleManager::CleanUp()
{
	return true;
}

void PuzzleManager::SewerEntrancePuzzle()
{


}

void PuzzleManager::CagePuzzle()
{
}

void PuzzleManager::MagicGemPuzzle()
{
}
