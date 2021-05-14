#include "Quest.h"

Quest::Quest(pugi::xml_node node)
{

	id = node.attribute("id").as_int();
	type = node.attribute("type").as_int();
	title = node.attribute("title").as_string();
	description = node.attribute("description").as_string();
	objective = node.attribute("objective").as_string();
	progress = node.attribute("progress").as_int();
	quantity = node.attribute("quantity").as_int();
	demandingNPC = node.attribute("demandingNPC").as_string();
	reward = node.attribute("reward").as_string();
	rewardQuantity = node.attribute("rewardQuantity").as_string();
	rewardingNPC = node.attribute("rewardingNPC").as_string();
	rewardXP = node.attribute("rewardXP").as_int();
	rewardGold = node.attribute("rewardGold").as_int();
	requiredId = node.attribute("requiredId").as_int();
	isCompleted = node.attribute("isCompleted").as_bool();
	status = node.attribute("status").as_int();

}