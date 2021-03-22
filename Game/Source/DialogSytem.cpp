#include "DialogSytem.h"
#include "DialogNode.h"

#include <utility>

#include "Log.h"

DialogSystem::DialogSystem()
{
	name.Create("dialog");
}

DialogSystem::~DialogSystem()
{

}

bool DialogSystem::Awake(pugi::xml_node& config)
{
	folder = config.child("folder").child_value();

	return true;
}

bool DialogSystem::Start()
{
	LoadDialog("test.xml");

	DialogNode* dialog = dialogues.at("TEST");

	return true;
}

bool DialogSystem::PreUpdate()
{
	return true;
}

bool DialogSystem::Update(float dt)
{
	return true;
}

bool DialogSystem::PostUpdate()
{
	return true;
}

bool DialogSystem::CleanUp()
{
	return true;
}

bool DialogSystem::LoadDialog(const char* filename)
{
	bool ret = true;

	std::string path = folder + filename;

	pugi::xml_parse_result result = dialogFile.load_file(path.c_str());

	if (result == NULL) {
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	if (ret == true) {
		pugi::xml_node dialogRoot = dialogFile.child("dialog");
		std::string id = dialogRoot.attribute("id").as_string();
		DialogNode* dialog = ParseDialogXML(dialogRoot);
		dialogues.insert(std::make_pair(id, dialog));
	}

	return ret;
}

DialogNode* DialogSystem::ParseDialogXML(pugi::xml_node currentNode)
{
	DialogNode* dialogNode = new DialogNode();

	/* Start TODO 1 */

	std::string type = currentNode.name();
	if (type == "dialog") {}
	else if (type == "line")
	{
		dialogNode->type = DialogNode::NodeType::LINE;
	}
	else if (type == "options")
	{
		dialogNode->type = DialogNode::NodeType::OPTIONS;
	}
	else if (type == "option")
	{
		dialogNode->type = DialogNode::NodeType::OPTION;
	}
	else
		return nullptr;


	Attributes* attributes = new Attributes();

	pugi::xml_attribute_iterator aIt = currentNode.attributes_begin();
	for (aIt; aIt != currentNode.attributes_end(); aIt++)
	{
		attributes->insert(std::make_pair(aIt->name(), aIt->value()));
	}

	dialogNode->SetAttributes(attributes);

	if (dialogNode->type != DialogNode::NodeType::LINE)
	{
		pugi::xml_node_iterator cIt = currentNode.begin();
		for (cIt; cIt != currentNode.end(); cIt++)
		{
			dialogNode->AddChild(ParseDialogXML(*cIt));
		}
	}

	/* End TODO 1 */

	return dialogNode;

	/*pugi::xml_node_iterator it = currentNode.begin();
	for (it; it != currentNode.end(); it++) 
	{
		const char* type = it->name();
		if (type == "line")
		{
			dialogNode->type = DialogNode::NodeType::LINE;
		}
		else if (type == "options")
		{
			dialogNode->type = DialogNode::NodeType::OPTIONS;
		}
		else if (type == "option")
		{
			dialogNode->type = DialogNode::NodeType::OPTION;
		}
		else
			return nullptr;

		Attributes* attributes = new Attributes();

		pugi::xml_attribute_iterator aIt = currentNode.attributes_begin();
		for (aIt; aIt != currentNode.attributes_end(); aIt++)
		{
			attributes->insert(std::make_pair(aIt->name(), aIt->value()));
		}

		dialogNode->SetAttributes(attributes);
	}*/

	/* End TODO 1 */

	return dialogNode;
}

void DialogSystem::PrintDialog(DialogNode* node)
{
	LOG("TYPE: %i", (int)node->type);

	Attributes::iterator it = node->attributes->begin();
	for (it; it != node->attributes->end(); it++) {
		LOG("%s, %s", it->first.c_str(), it->second.c_str());
	}

	LOG("BEGIN CHILDREN");
	std::vector<DialogNode*>::iterator cit = node->children->begin();
	for (cit; cit != node->children->end(); cit++) {
		PrintDialog(*cit);
	}
	LOG("END CHILDREN");
}




