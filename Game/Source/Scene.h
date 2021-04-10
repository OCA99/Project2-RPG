#pragma once

class Scene
{
public:
	Scene() {}

	~Scene() {}

	virtual void Load() {}

	virtual void CleanUp() {}
};

class MapScene : Scene
{
public:
	MapScene(const char* _filename) : Scene()
	{
		filename = _filename;
	}

	void Load();

	void CleanUp();

private:
	const char* filename;
};