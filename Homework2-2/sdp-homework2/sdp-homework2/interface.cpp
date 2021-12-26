#include "interface.h"
#include <queue>
#include <algorithm>
#include <iostream>

using std::queue;
Hierarchy::Hierarchy(Hierarchy&& rhsHierarchy) noexcept
{
	this->root = nullptr;
	std::swap(this->root, rhsHierarchy.root);
}

Hierarchy::Hierarchy(const Hierarchy& rhsHierarchy)
{
	if (rhsHierarchy.root == nullptr)
		this->root = nullptr;
	else
		this->root = this->copyNode(rhsHierarchy.root);
}

Hierarchy::~Hierarchy()
{
	this->eraseHierarchy(this->root);
}

string Hierarchy::print() const
{
	if (this->root == nullptr) return string();

	string result;

	queue<Node*> bfsQueue;
	bfsQueue.push(this->root);

	while (!bfsQueue.empty()) {
		size_t bfsQueueCurrentSize = bfsQueue.size();

		Node* pCurrent = bfsQueue.front();
		bfsQueue.pop();

		for (std::list<Hierarchy::Node*>::iterator it = pCurrent->subordinates.begin(); it != pCurrent->subordinates.end(); it++) {
			bfsQueue.push(*it);
			result.append(pCurrent->getName() + "-" + (*it)->getName() + '\n');
		}
	}

	return result;
}

int Hierarchy::longest_chain() const
{
	return this->hierarchyHeight(this->root);
}
// checks if a Node with that name exists - using BFS
bool Hierarchy::find(const string& name) const 
{
	if (this->root == nullptr) return false;

	queue<Node*> bfsQueue;
	bfsQueue.push(this->root);

	while (!bfsQueue.empty()) {
		size_t bfsQueueCurrentSize = bfsQueue.size();

		Node* pCurrent = bfsQueue.front();
		bfsQueue.pop();

		if (pCurrent->name == name) return true;

		for (std::list<Hierarchy::Node*>::iterator it = pCurrent->subordinates.begin(); it != pCurrent->subordinates.end(); it++) {
			bfsQueue.push(*it);
		}
	}

	return false;
}

int Hierarchy::num_employees() const
{
	if (this->root == nullptr) return -1;

	int employeesCounter = 0;

	queue<Node*> bfsQueue;
	bfsQueue.push(this->root);

	while (!bfsQueue.empty()) {
		Node* pCurrent = bfsQueue.front();
		bfsQueue.pop();

		// Pushes children in queue, if there are children of pCurrent
		for (std::list<Hierarchy::Node*>::iterator it = pCurrent->subordinates.begin(); it != pCurrent->subordinates.end(); it++) {
			employeesCounter++;
			bfsQueue.push(*it);
		}
	}

	return employeesCounter;
}

int Hierarchy::num_overloaded(int level) const
{
	if (this->root == nullptr) return 0;

	if (!this->isNodeOverloaded(this->root, level)) return 0;

	int overloadedManagersAmount = 0;

	queue<Node*> bfsQueue;
	bfsQueue.push(this->root);

	while (!bfsQueue.empty()) {
		Node* pCurrent = bfsQueue.front();
		bfsQueue.pop();
		if (!this->isNodeOverloaded(pCurrent, level))
			continue;
		else {
			 overloadedManagersAmount++;

			for (std::list<Hierarchy::Node*>::iterator it = pCurrent->subordinates.begin(); it != pCurrent->subordinates.end(); it++) {
				bfsQueue.push(*it);
			}
		
		}
	}
	
	return overloadedManagersAmount;
}

Hierarchy::Node* Hierarchy::findNodeByManagerName(const string& _managerName) const
{
	if (this->root == nullptr) return nullptr;

	queue<Node*> bfsQueue;
	bfsQueue.push(this->root);

	while (!bfsQueue.empty()) {
		size_t bfsQueueCurrentSize = bfsQueue.size();

		Node* pCurrent = bfsQueue.front();
		bfsQueue.pop();

		if (pCurrent->name == _managerName) return pCurrent;

		// Pushes children in queue, if there are children of pCurrent
		for (std::list<Hierarchy::Node*>::iterator it = pCurrent->subordinates.begin(); it != pCurrent->subordinates.end(); it++) {
			bfsQueue.push(*it);
		}
	}

	// Stays here for debugging purposes
	string exceptionMessage = "findNodeByManagerName: didnt find manager with name: " + _managerName;
	//throw std::runtime_error(exceptionMessage);
	std::cout << exceptionMessage << std::endl;
	return nullptr;
}

Hierarchy::Node* Hierarchy::findNodeByManagerName(const string& _managerName)
{
	if (this->root == nullptr) return nullptr;

	queue<Node*> bfsQueue;
	bfsQueue.push(this->root);

	while (!bfsQueue.empty()) {
		size_t bfsQueueCurrentSize = bfsQueue.size();

		Node* pCurrent = bfsQueue.front();
		bfsQueue.pop();

		if (pCurrent->name == _managerName) return pCurrent;

		// Pushes children in queue, if there are children of pCurrent
		for (std::list<Hierarchy::Node*>::iterator it = pCurrent->subordinates.begin(); it != pCurrent->subordinates.end(); it++) {
			bfsQueue.push(*it);
		}
	}

	// Stays here for debugging purposes
	string exceptionMessage = "findNodeByManagerName: didnt find manager with name: " + _managerName;
	//throw std::runtime_error(exceptionMessage);
	std::cout << exceptionMessage << std::endl;
	return nullptr;
}

bool Hierarchy::isNodeOverloaded(const Node* nodeToCheck, int overloadLevel) const
{
	if (this->root == nullptr) return false;

	if (nodeToCheck == nullptr) return false;

	int directSubordinates = nodeToCheck->subordinates.size();

	if (directSubordinates > overloadLevel) return true;

	int indirectSubordinates = 0;

	queue<Node*> bfsQueue;
	for (Node* pSubordinate : nodeToCheck->subordinates) {
		bfsQueue.push(pSubordinate);
	}

	while (!bfsQueue.empty()) {
		Node* pCurrent = bfsQueue.front();
		bfsQueue.pop();

		indirectSubordinates += pCurrent->subordinates.size();

		// Pushes children in queue, if there are children of pCurrent
		for (std::list<Hierarchy::Node*>::iterator it = pCurrent->subordinates.begin(); it != pCurrent->subordinates.end(); it++) {
			bfsQueue.push(*it);
		}
	}

	return (directSubordinates + indirectSubordinates) > overloadLevel;
}

void Hierarchy::eraseHierarchy(Node*& root)
{
	if (!root) return;

	for (Node*& subordinate : root->subordinates) eraseHierarchy(subordinate);

	// delete root must be the last operation
	delete root;
	root = nullptr;
}

int Hierarchy::hierarchyHeight(const Node* root) const
{
	if (!root)
		return 0;

	int currentMaxHeight = 0;
	for (const Node* currentNode : root->subordinates)
	{
		int subordinateHeight = hierarchyHeight(currentNode);
		if (subordinateHeight > currentMaxHeight)
			currentMaxHeight = subordinateHeight;
	}

	return 1 + currentMaxHeight;
}

Hierarchy::Node* Hierarchy::copyNode(const Node* fromNode)
{
	Node* toNode = new Node(fromNode->name);

	for (const Node* subordinate : fromNode->subordinates) {
		toNode->subordinates.push_back(copyNode(subordinate));
	}

	return toNode;
}

void Hierarchy::addToHierachy(const string& _managerName, const string& _subordinateName)
{
	bool managerExists = this->find(_managerName);
	
	if (managerExists) {
		Node* pManagerNode = this->findNodeByManagerName(_managerName);
		if (pManagerNode != nullptr) {
			pManagerNode->subordinates.push_back(new Node(_subordinateName));
			pManagerNode->subordinates.sort(Hierarchy::Node::NodePointerComparator());
		}
	} else {
		std::cout << "addToHierarchy: Node with name: " + _managerName << "could NOT be found." << std::endl;
	}
}

string Hierarchy::manager(const string& name) const
{
	if (this->root->name == name) return "none";

	if (this->find(name) && this->root != nullptr) {
		string managerName;
		queue<Node*> bfsQueue;
		bfsQueue.push(this->root);

		while (!bfsQueue.empty()) {
			size_t bfsQueueCurrentSize = bfsQueue.size();

			Node* pCurrent = bfsQueue.front();
			bfsQueue.pop();

			for (const Hierarchy::Node* pNode: pCurrent->subordinates) {
				if (pNode->name == name) {
					managerName = pCurrent->name;
					break;
				}
			}

			if (!managerName.empty()) return managerName;

			// Pushes children in queue, if there are children of pCurrent
			for (std::list<Hierarchy::Node*>::iterator it = pCurrent->subordinates.begin(); it != pCurrent->subordinates.end(); it++) {
				bfsQueue.push(*it);
			}
		}
	}
}

int Hierarchy::num_subordinates(const string& name) const
{
	Hierarchy::Node* nodeWithName = this->findNodeByManagerName(name);

	if (nodeWithName != nullptr) {
		return nodeWithName->subordinates.size();
	} else
	{ 
		std::cout << "num_subordinates: returning -1, because node with name wasnt found in the tree." << std::endl;
		return -1;
	}
}
