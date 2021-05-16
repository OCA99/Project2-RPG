#ifndef __PUZZLEMANAGER_H__
#define __PUZZLEMANAGER_H__

#include "Module.h"
#include "Textures.h"
#include "GuiManager.h"

#include "SString.h"

class PuzzleManager : public Module
{
public:

	enum class ActivePuzzle {
		NONE,
		SEWERENTRANCE,
		CAGEPUZZLE,
		MAGICGEM
	};

	PuzzleManager();
	~PuzzleManager();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	//PUZLESS
	void SewerEntrancePuzzle();
	void CagePuzzle();
	void MagicGemPuzzle();

	void DrawGraveyardPuzzle();
	void DrawCagePuzzle();

	void ClearButtons();


public:

	ActivePuzzle currentPuzzle = ActivePuzzle::SEWERENTRANCE;
	SDL_Texture** noteTex = nullptr;
	SDL_Texture** passwordTex = nullptr;
	SDL_Texture** tombTex = nullptr;

	//Properties
	bool key1 = false;
	bool key2 = false;
	bool key3 = false;

	bool key4 = false;
	bool key5 = false;
	bool key6 = false;
	bool completed = false;

	bool drawPass = false;

	List<GuiControl*> codeButtons;//Buttons List

};

#endif // __PUZZLEMANAGER_H__