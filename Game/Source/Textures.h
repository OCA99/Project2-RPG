#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "Module.h"

#include "List.h"
#include "SDL_image/include/SDL_image.h"
#include <map>
#include <vector>

struct SDL_Texture;
struct SDL_Surface;

class Textures : public Module
{
public:

	Textures();

	// Destructor
	virtual ~Textures();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture** const Load(const char* path);
	SDL_Texture* const LoadSurface(SDL_Surface* surface);
	bool UnLoad(SDL_Texture** texture);
	void GetSize(SDL_Texture* texture, uint& width, uint& height) const;

	void ReloadAllTextures();

public:
	std::map<const char*, SDL_Texture**> textures;

private:
	std::vector<SDL_Texture*> textureMemory;
};


#endif // __TEXTURES_H__