#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Textures.h"
#include "App.h"

#include "Log.h"

#include <vector>
#include <math.h>

#include "PugiXml\src\pugixml.hpp"
#include "SDL/include/SDL.h"

// L03: DONE 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	~TileSet()
	{
		app->tex->UnLoad(texture);
		loaded = false;
	}

	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;

	SDL_Texture* texture;
	int	texWidth;
	int	texHeight;
	int	numTilesWidth;
	int	numTilesHeight;
	int	offsetX;
	int	offsetY;

	bool loaded = true;

	// L04: TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const
	{
		id = id - firstgid;
		return { id % numTilesWidth * tileWidth, id / numTilesWidth * tileHeight, tileWidth, tileHeight };
	}
};

// L03: DONE 1: We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// L06: TODO 5: Create a generic structure to hold properties
struct Properties
{
	struct Property
	{
		SString name;
		SString type;
		int value;
	};


	~Properties()
	{

	}

	List<Property*> propertyList;

	int GetProperty(const char* name, int defaultValue = 1) const;

	void SetProperty(const char* name, int value);

};

// L04: DONE 1: Create a struct for the map layer
struct MapLayer
{
	SString	name;
	int width;
	int height;
	uint* data;

	Properties properties;

	bool draw = true;
	bool navigation = false;
	bool isEvent = false;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// L04: TODO 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		return data[y * width + x];
	}

	uint Size() const
	{
		return width * height;
	}
};

// L03: DONE 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	int nextObjectId;
	int nextLayerId;
	SDL_Color backgroundColor;
	MapTypes type;
	List<TileSet*> tilesets;
	List<MapLayer*> maplayers;
	Properties properties;

	// This function returns the gid relative to the tileset it belongs to
	int GetTileSetGid(int gid)
	{
		for (int i = 0; i < tilesets.Count(); i++)
		{
			if (gid < tilesets[i]->firstgid)
			{
				return gid - tilesets[i - 1]->firstgid;
			}
		}

		return gid - tilesets[tilesets.Count() - 1]->firstgid;
	}

	// L04: TODO 2: Add a list/array of layers to the map
};

struct MapEvent
{
	int mapId;
	int layerId;
	int eventId;

	std::map<std::string, std::string>* attributes;
};

class Map : public Module
{
public:

	Map();

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool Update(float dt);

	bool PostUpdate(float dt);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	void LoadEvents();

	// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	SDL_Rect OuterRectangle();

	std::vector<SDL_Rect>* NavigationIntersection(SDL_Rect other);

	bool Map::EventIntersection(SDL_Rect other, std::pair<int, int>& result);

	MapEvent* GetEvent(int layerId, int eventId)
	{
		int mapId = data.properties.GetProperty("mapId");

		for (int i = 0; i < events->size(); i++)
		{
			MapEvent* e = events->at(i);
			if (e->mapId == mapId && e->layerId == layerId && e->eventId == eventId)
			{
				return e;
			}
		}

		return nullptr;
	}

private:

	// L03: Methods to load all required map data
	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties* properties);
	bool StoreID(pugi::xml_node& node, MapLayer* layer, int ID);
	void LoadNPCs();

	bool Intersects(SDL_Rect a, SDL_Rect b)
	{
		int leftX = std::max(a.x, b.x);
		int rightX = std::min(a.x + a.w, b.x + b.w);
		int topY = std::max(a.y, b.y);
		int bottomY = std::min(a.y + a.h, b.y + b.h);

		if (leftX < rightX && topY < bottomY)
			return true;
		else
			return false;
	}

public:

	// L03: DONE 1: Add your struct for map info
	MapData data;
	std::vector<MapEvent*>* events;

private:
	pugi::xml_document mapFile;
	SString folder;
	bool mapLoaded;
	SDL_Rect outerRectangle;
};

#endif // __MAP_H__