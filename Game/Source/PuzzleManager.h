#ifndef __PUZZLEMANAGER_H__
#define __PUZZLEMANAGER_H__

#include "Module.h"

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

public:
	ActivePuzzle currentPuzzle = ActivePuzzle::SEWERENTRANCE;

	//Properties
	bool key1 = false;
	bool key2 = false;
	bool key3 = false;
	bool completed = false;


};

#endif // __PUZZLEMANAGER_H__