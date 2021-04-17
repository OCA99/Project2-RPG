#ifndef __PARTYMANAGER_H__
#define __PARTYMANAGER_H__

#include "Module.h"
#include "List.h"

#include <string>

enum Type {
	NONE,
	ENEMY,
	PLAYER
};

struct Data {
	//Numero en la Party
	int id;
	float health;
	float damage;
	bool dead = false;
};

struct Member {
	//Sprite sprite;
	std::string name;
	Type type;
	Data data;

	Member();
	Member(std::string name, Type type);
	Member(std::string name, Type type, float health, float damage, bool dead);
};

struct Party {

	List<Member> list;

	//Functions
	Party();
	Party(List<Member*>& list);

	void PrintMemberDescription(Member member);
	void PrintPartyDescription();
	//Find Member
	ListItem<Member>* FindByName(const std::string name) const;

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