#include "PuzzleManager.h"
#include "App.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Render.h"
#include "Fonts.h"
#include "SString.h"
#include "Audio.h"

#include <iostream>
#include "ToUpperCase.h"

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
	tombTex = app->tex->Load("Textures/Environment/tomb.png");
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
			DrawGraveyardPuzzle();
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
		key1 = false;
		key2 = false;
		key3 = false;
		app->audio->PlayFx(13, 0);
	}
}

void PuzzleManager::CagePuzzle()
{
	if (key4 && key5 && key6)
	{
		if (!completed) completed = true;
		currentPuzzle = ActivePuzzle::MAGICGEM;
		ClearButtons();
		key1 = false;
		key2 = false;
		key3 = false;
		key4 = false;
		key5 = false;
		key6 = false;
		app->audio->PlayFx(13, 0);
	}

}

void PuzzleManager::MagicGemPuzzle()
{
}

void PuzzleManager::DrawGraveyardPuzzle()
{
	if (key1)app->render->DrawTexture(tombTex, 0, 0, &SDL_Rect({ 0,0,48,64 }), 0.5f, 0, 0, 0, false);
	if (key2)app->render->DrawTexture(tombTex, 32, 0, &SDL_Rect({ 0,0,48,64 }), 0.5f, 0, 0, 0, false);
	if (key3)app->render->DrawTexture(tombTex, 64, 0, &SDL_Rect({ 0,0,48,64 }), 0.5f, 0, 0, 0, false);

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
		if (codeButtons.Count() <= 0)
		{
			codeButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 276 + 50, 110 + 50, 17, 17 }), 21));// 0
			codeButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 276, 110, 17, 17 }), 21));// 1
			codeButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 276 + 25, 110, 17, 17 }), 21));//2
			codeButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 276 + 50, 110, 17, 17 }), 21));//3
			codeButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 276 + 75, 110, 17, 17 }), 21));//4


			codeButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 276, 110 + 25, 17, 17 }), 21));// 5
			codeButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 276 + 25, 110 + 25, 17 , 17 }), 21));//6
			codeButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 276 + 50, 110 + 25, 17 , 17 }), 21));//7
			codeButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 276 + 75, 110 + 25, 17 , 17 }), 21));//8

			codeButtons.Add(app->ui->CreateGuiControl(GuiControlType::BUTTON, SDL_Rect({ 276 + 25, 110 + 50, 17, 17 }), 21));// 9


		}

		app->render->DrawTexture(passwordTex, 0, 0, &SDL_Rect({ 0, 0, 1280, 720 }), 0.5f, 0, 0, 0, 0, false);


		ListItem<GuiControl*>* item = codeButtons.start;
		int a = 0;

		while (item)
		{
			item->data->Draw(app->render);

			if (item->data->checkPuzzle)
			{
				if (a == 5 && !key5 && !key6)
				{
					if (!key4)
					{
						key4 = true;
						app->audio->PlayFx(11, 0);
					}

				}
				if (a == 2 && key4 && !key6)
				{
					if (!key5)
					{
						key5 = true;
						app->audio->PlayFx(11, 0);
					}
					
				}
				if (a == 1 && key4 && key5)
				{
					if (!key6)
					{
						key6 = true;
						app->audio->PlayFx(11, 0);
					}

					
				}

				
			}
			a++;
			item = item->next;

		}


	}

}

void PuzzleManager::ClearButtons()
{
	ListItem<GuiControl*>* item = codeButtons.start;
	while (item)
	{
		app->ui->DestroyGuiControl(item->data);
		item = item->next;
	}
	codeButtons.Clear();
}
