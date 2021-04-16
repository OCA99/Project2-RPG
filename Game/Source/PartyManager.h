#ifndef __PARTYMANAGER_H__
#define __PARTYMANAGER_H__

#include "Module.h"

#include <string>

enum Type {
	NONE,
	ENEMY,
	PLAYER
};

struct Member {
	Type* type;
	std::string name;
	bool dead;

	struct Data {
		//Numero en la Party
		int id;
		float health;
		float damage;
	};

};

struct Party {
	//Getter
	//SETTER

	List<Member> list;
};

class PartyManager : public Module
{

public:

	PartyManager();
	~PartyManager();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	//Crear Party
	void AddMember(Member& member, Party* party);
	void RemoveMember(Member& member, Party* party);


	//PRINTEAR Stats de la Party / COMBAT LOG

	//SETTERS Y GETTERS

private:

	Party* currentParty;

};

#endif