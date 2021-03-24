#include "DialogSytem.h"
#include "DialogNode.h"

#include "App.h"
#include "Render.h"
#include "Input.h"

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
	StartDialog("TEST");

	return true;
}

bool DialogSystem::PreUpdate()
{
	return true;
}

bool DialogSystem::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) {
		NextDialog();
	}

	return true;
}

bool DialogSystem::PostUpdate()
{
	if (currentDialog == nullptr) return true;

	app->render->DrawRectangle(SDL_Rect({ 0, (app->render->camera.h / 3) * 2, app->render->camera.w, app->render->camera.h / 3 }), 255, 255, 255, 255, true, true);

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
		/* Start TODO 1 */

		pugi::xml_node dialogRoot = dialogFile.child("dialog");
		std::string id = dialogRoot.attribute("id").as_string();
		DialogNode* dialog = ParseDialogXML(dialogRoot);
		dialogues.insert(std::make_pair(id, dialog));

		/* End TODO 1 */
	}

	return ret;
}

void DialogSystem::StartDialog(const char* id)
{
	currentDialog = dialogues.at(id);
	NextDialog();
}

void DialogSystem::EndDialog()
{
}

void DialogSystem::NextDialog()
{
	if (currentDialog == nullptr) return;

	if (currentDialog->type == DialogNode::NodeType::DIALOG)
	{
		currentDialog = *currentDialog->children->begin();
	}
	else if (currentDialog->type == DialogNode::NodeType::OPTIONS)
	{
		// Select correct option
		currentDialog = *currentDialog->children->at(0)->children->begin();
	}
	else
	{
		currentDialog = currentDialog->Next();
	}

	if (currentDialog == nullptr)
	{
		EndDialog();
		return;
	}

	if (currentDialog->attributes->find("value") != currentDialog->attributes->end())
	{
		LOG("%s\n", currentDialog->attributes->at("value").c_str());
	}
	else
	{
		LOG("Not found!");
	}
}

DialogNode* DialogSystem::ParseDialogXML(pugi::xml_node currentNode)
{
	DialogNode* dialogNode = new DialogNode();

	/* Start TODO 2 */

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
			DialogNode* child = ParseDialogXML(*cIt);
			child->parent = dialogNode;
			if (dialogNode->children->size() != 0)
			{
				child->prev = *(dialogNode->children->end() - 1);
				(*(dialogNode->children->end() - 1))->next = child;
			}
			dialogNode->AddChild(child);
		}
	}

	/* End TODO 2 */

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