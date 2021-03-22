#pragma once

#include <vector>
#include <map>
#include <string>

using Attributes = std::map<std::string, std::string>;

struct DialogNode {
public:
	enum class NodeType {
		DIALOG,
		LINE,
		OPTIONS,
		OPTION
	};

	DialogNode(Attributes* _attributes = nullptr) {
		type = NodeType::DIALOG;
		children = new std::vector<DialogNode*>();
		attributes = _attributes;
	}

	~DialogNode() {
		delete children;
		delete attributes;
	}

	void SetAttributes(Attributes* _attributes) {
		attributes = _attributes;
	}

	void AddChild(DialogNode* child)
	{
		children->push_back(child);
	}

	NodeType type;
	std::vector<DialogNode*>* children;
	std::map<std::string, std::string>* attributes;
};

/*struct DialogLine : public DialogNode {
public:
	DialogLine(std::string _value, Attributes* _attributes = nullptr) : DialogNode(_attributes) {
		type = NodeType::DIALOG;
		value = _value;
	}

private:
	std::string value;
};

struct DialogOptions : public DialogNode {
public:
	DialogOptions(Attributes* _attributes = nullptr) : DialogNode(_attributes) {
		type = NodeType::OPTIONS;
	}
};

struct DialogOption : public DialogNode {
public:
	DialogOption(Attributes* _attributes = nullptr) : DialogNode(_attributes) {
		type = NodeType::OPTION;
	}
};*/