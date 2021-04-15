#include "DialogSytem.h"
#include "DialogNode.h"

#include "App.h"
#include "Render.h"
#include "Input.h"
#include "Fonts.h"
#include "Window.h"
#include "Textures.h"

#include <utility>

#include "ToUpperCase.h"

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
	// Get dialog file directory
	folder = config.child("folder").child_value();

	return true;
}

bool DialogSystem::Start()
{
	dialogFrame = app->tex->Load("Assets/Textures/Dialogue/frame.png");
	tavernLady = app->tex->Load("Assets/Textures/Dialogue/tavern_lady_dialogue.png");

	return true;
}

bool DialogSystem::PreUpdate()
{
	return true;
}

bool DialogSystem::Update(float dt)
{
	// The key to skip to the next dialog line.
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) {
		NextDialog();
	}

	// Select the next option.
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_DOWN && currentDialog != nullptr) {
		selectedOption += 1;
		if (selectedOption == currentDialog->children->size())
			selectedOption = currentDialog->children->size() - 1;
	}

	// Select the previous option.
	if (app->input->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_DOWN && currentDialog != nullptr) {
		selectedOption -= 1;
		if (selectedOption < 0) selectedOption = 0;
	}

	/* ONLY FOR TESTING */

	/*if (app->input->GetKey(SDL_SCANCODE_1) == KeyState::KEY_DOWN) {
		StartDialog("1");
	}

	if (app->input->GetKey(SDL_SCANCODE_2) == KeyState::KEY_DOWN) {
		StartDialog("2");
	}*/

	/* END ONLY FOR TESTING */

	return true;
}

bool DialogSystem::PostUpdate(float dt)
{
	if (currentDialog == nullptr) return true;

	DrawDialog();

	return true;
}

void DialogSystem::DrawDialog()
{
	// Draw the background rectangle.
	//app->render->DrawRectangle(SDL_Rect({ 0, (app->render->camera.h / 3) * 2, app->render->camera.w, app->render->camera.h / 3 }), 255, 255, 255, 255, true, false);
	app->render->DrawTexture(dialogFrame, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);

	std::string speaker = currentDialog->attributes->at("speaker");

	if (std::strcmp(speaker.c_str(), "Oscar") == 0)
	{
		app->render->DrawTexture(tavernLady, 0, 0, nullptr, .5f, 0.0f, 0.0f, INT_MAX, INT_MAX, false);
	}

	// Set the text to uppercase, since our font only supports uppercase.
	std::string text = ToUpperCase(currentDialog->attributes->at("value"));

	// Write the dialog line.
	app->fonts->BlitText(25, 260, 0, text.c_str());

	// If the current node is a question, we should also draw the possible answers
	if (currentDialog->type == DialogNode::NodeType::OPTIONS)
	{
		std::vector<DialogNode*>::iterator i = currentDialog->children->begin();
		int y = 0;
		// Iterate through the answers.
		for (i; i < currentDialog->children->end(); i++)
		{
			// Set them to uppercase.
			text = ToUpperCase((*i)->attributes->at("value"));
			// Draw them, increasing the y offset at every iteration.
			app->fonts->BlitText(42, 275 + y * 15, 0, text.c_str());
			y++;
		}
		// Draw a small black rectangle next to the selected option.
		SDL_Rect selectedRectangle = SDL_Rect({ 30 * int(app->win->GetScale()), (277 + (15 * selectedOption)) * int(app->win->GetScale()), 6, 6 });
		app->render->DrawRectangle(selectedRectangle, 255, 255, 255, 255, true, false);
	}
}

bool DialogSystem::CleanUp()
{
	nodeRoutes.clear();
	callbacks.clear();
	dialogues.clear();

	return true;
}

bool DialogSystem::LoadDialog(const char* filename)
{
	bool ret = true;

	// Get the file path.
	std::string path = folder + filename;

	LOG("%s\n", path.c_str());

	// Load the file.
	pugi::xml_parse_result result = dialogFile.load_file(path.c_str());

	if (result == NULL) {
		LOG("Could not load map xml file %s. pugi error: %s", path.c_str(), result.description());
		ret = false;
	}

	if (ret == true) {
		/* Start TODO 1 */

		// Get the dialog root.
		pugi::xml_node dialogRoot = dialogFile.child("dialog");
		std::string id = dialogRoot.attribute("id").as_string();
		// Parse the dialog XML into the tree structure.
		DialogNode* dialog = ParseDialogXML(dialogRoot);
		// Insert the dialog into the dictionary.
		dialogues.insert(std::make_pair(id, dialog));

		/* End TODO 1 */
	}

	return ret;
}

void DialogSystem::StartDialog(const char* id)
{
	// If the dialog does not exist, return.
	if (dialogues.find(id) == dialogues.end()) return;
	// If it does exist, set it to currentDialog and run NextDialog() to start.
	currentDialog = dialogues.at(id);
	NextDialog();
}

void DialogSystem::NextDialog()
{
	/* Start TODO 4 */

	// If we have reached the end, currentDialog will be nullptr.
	if (currentDialog == nullptr) return;

	// If the currentDialog is DIALOG, it means we are at the root of the tree. We can just skip to the first child.
	if (currentDialog->type == DialogNode::NodeType::DIALOG)
	{
		currentDialog = *currentDialog->children->begin();
	}
	// If currentDialog is OPTIONS, we should skip to the selected child.
	else if (currentDialog->type == DialogNode::NodeType::OPTIONS)
	{
		currentDialog = *currentDialog->children->at(selectedOption)->children->begin();
	}
	// Else, we just skip to the next line.
	else
	{
		currentDialog = currentDialog->Next();
	}

	// We reset the selectedOption to 0.
	selectedOption = 0;

	// Again, if we have reached the end of the dialog we return.
	if (currentDialog == nullptr)
	{
		return;
	}

	// If the current line has a callback, we execute it.
	if (currentDialog->attributes->find("callback") != currentDialog->attributes->end())
	{
		callbacks.at(currentDialog->attributes->at("callback"))();
	}

	// If the current dialog is GOTO, we get the "route" and travel to the new line.
	if (currentDialog->type == DialogNode::NodeType::GOTO) {
		std::string route = currentDialog->attributes->at("route");
		currentDialog = nodeRoutes.at(route);
	}

	/* End TODO 4 */
}

DialogNode* DialogSystem::ParseDialogXML(pugi::xml_node currentNode)
{
	DialogNode* dialogNode = new DialogNode();

	/* Start TODO 2 */

	// We set the type variable acording to the XML node name. 
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
	else if (type == "goto")
	{
		dialogNode->type = DialogNode::NodeType::GOTO;
	}
	else
		return nullptr;


	Attributes* attributes = new Attributes();

	// We iterate over the attributes and store them in a dictionary.
	pugi::xml_attribute_iterator aIt = currentNode.attributes_begin();
	for (aIt; aIt != currentNode.attributes_end(); aIt++)
	{
		attributes->insert(std::make_pair(aIt->name(), aIt->value()));
	}

	dialogNode->SetAttributes(attributes);

	// If the node contains an "id", we store it in a dictionary that will be useful for GOTOs.
	if (dialogNode->attributes->find("id") != dialogNode->attributes->end())
	{
		nodeRoutes.insert(std::make_pair(dialogNode->attributes->at("id"), dialogNode));
	}

	// If the type is not LINE, we execute ParseDialogXML recursively over its children and store them in the "children" vector.
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