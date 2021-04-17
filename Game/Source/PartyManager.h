#ifndef __PARTYMANAGER_H__
#define __PARTYMANAGER_H__

#include "ECS.h"
#include "Module.h"
#include "List.h"

#include <string>

enum Type {
	PLAYER1,
	PLAYER2,
	SKELETON,
	SHROOM1,
	SHROOM2
};

struct Action {
	enum class Target {
		ALLY,
		ENEMY,
		BOTH
	};

	std::string name;
	Target target;
	float damage;
	float heal;
};

struct Data {
	//Numero en la Party
	int id;
	float health;
	float power;
	bool dead = false;
	List<Action> actions;
};

struct Member {
	//Sprite sprite;
	std::string name;
	Type type;
	Data data;

	Member();
	Member(std::string name, Type type);
	Member(std::string name, Type type, float health, float damage, bool isDead);
	~Member();
};

struct Party {

	List<Member> list;
	std::string partyName;

	//Functions
	Party();
	Party(std::string partyName);
	Party(List<Member>& list);
	~Party();

	void PrintMemberDescription(std::string name);
	void PrintPartyDescription();
	void AddMember(Member member);
	void RemoveMember(const std::string name);

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

	void OpenPartyInventory();

private:

	ECS::World* world = nullptr;
	Party* allyParty = nullptr;
	Party* enemyParty = nullptr;

};

#endif