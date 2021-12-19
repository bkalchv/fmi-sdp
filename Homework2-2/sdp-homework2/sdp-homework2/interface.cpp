#include "interface.h"
#include <queue>

using std::queue;
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

		if (pCurrent->managerName == name) return true;

		for (std::list<Hierarchy::Node*>::iterator it = pCurrent->subordinates.begin(); it != pCurrent->subordinates.end(); it++) {
			bfsQueue.push(*it);
		}
	}

	return false;
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

		if (pCurrent->managerName == _managerName) return pCurrent;

		// Pushes children in queue, if there are children of pCurrent
		for (std::list<Hierarchy::Node*>::iterator it = pCurrent->subordinates.begin(); it != pCurrent->subordinates.end(); it++) {
			bfsQueue.push(*it);
		}
	}

	// Stays here for debugging purposes
	string exceptionMessage = "findNodeByManagerName: didnt find manager with name: " + _managerName;
	throw std::runtime_error(exceptionMessage);
	return nullptr;
}

void Hierarchy::addToHierachy(const string& _managerName, const string& _subordinateName)
{
	bool managerExists = this->find(_managerName);
	
	if (managerExists) {
		Node* pManagerNode = this->findNodeByManagerName(_managerName);
		if (pManagerNode != nullptr) {
			pManagerNode->subordinates.push_back(new Node(_subordinateName));
		}
	}
}
