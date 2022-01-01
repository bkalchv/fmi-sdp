#include "interface.h"
#include <queue>
#include <iostream>

using std::queue;

Hierarchy::Hierarchy(Hierarchy&& rhsHierarchy) noexcept
{
	this->root = nullptr;
	//this->name.clear();
	std::swap(this->root, rhsHierarchy.root);
	//std::swap(this->name, rhsHierarchy.name);
}

Hierarchy::Hierarchy(const Hierarchy& rhsHierarchy)
{
	if (rhsHierarchy.root == nullptr)
		this->root = nullptr;
	else
		this->root = this->copyNode(rhsHierarchy.root);

	//this->name = rhsHierarchy.name;
}

Hierarchy::Hierarchy(const string& data)
{
	if (data.empty()) {
		this->root = nullptr;
	}
	else {
		this->root = new Node(THE_BOSS_NAME);

		std::vector<string> tokens;
		std::stringstream dataStringStream(data);
		std::string token;

		while (getline(dataStringStream, token, '\n')) {
			tokens.push_back(token);
		}

		std::string delimiter = "-";

		for (const std::string& token : tokens) {
			if (this->isValidToken(token)) {
				std::string stringUntilDelimiter = token.substr(0, token.find(delimiter));
				std::string managerName = removeWhitespacesFromString(stringUntilDelimiter);

				std::string stringAfterDelimiter = token.substr(token.find(delimiter) + delimiter.length());
				std::string subordinateName = removeWhitespacesFromString(stringAfterDelimiter);
				this->addToHierachy(managerName, subordinateName);
			}
			else {
				throw std::invalid_argument("Invalid input format!");
			}
		}
	}
}

Hierarchy::~Hierarchy()
{
	this->eraseHierarchy(this->root);
}

string Hierarchy::print() const
{
	this->sortNodeSubordinatesRecursively(this->root);

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
	if (this->root == nullptr) return 0;

	return 1 + this->countAllSubordinatesOfNode(this->root);
}

int Hierarchy::num_employees_recursively() const
{
	return this->countAllSubordinatesOfNodeRecursively(this->root);
}

int Hierarchy::num_overloaded(int level) const
{
	if (this->root == nullptr) return 0;

	if (!this->isNodeOverloaded(this->root, level)) return 0;

	int overloadedManagersAmount = 0;

	queue<const Node*> bfsQueue;
	bfsQueue.push(this->root);

	while (!bfsQueue.empty()) {
		const Node* pCurrent = bfsQueue.front();
		bfsQueue.pop();
		if (!this->isNodeOverloaded(pCurrent, level))
			continue;
		else {
			 overloadedManagersAmount++;

			 for (const Node* subordinate : pCurrent->subordinates) {
				 bfsQueue.push(subordinate);
			 }
		}
	}
	
	return overloadedManagersAmount;
}

const Hierarchy::Node* Hierarchy::findNodeByName(const string& _name) const
{
	if (this->root == nullptr) return nullptr;

	queue<const Node*> bfsQueue;
	bfsQueue.push(this->root);

	while (!bfsQueue.empty()) {
		size_t bfsQueueCurrentSize = bfsQueue.size();

		const Node* pCurrent = bfsQueue.front();
		bfsQueue.pop();

		if (pCurrent->name == _name) return pCurrent;

		// Pushes children in queue, if there are children of pCurrent
		for (const Node* subordinate : pCurrent->subordinates) {
			bfsQueue.push(subordinate);
		}
	}

	// Stays here for debugging purposes
	//std::cout << "findNodeByManagerName: didnt find manager with name: " << _name << std::endl;
	return nullptr;
}

Hierarchy::Node* Hierarchy::findNodeByName(const string& _managerName)
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
		for (Node* subordinate : pCurrent->subordinates) {
			bfsQueue.push(subordinate);
		}
	}

	// Stays here for debugging purposes
	string exceptionMessage = "findNodeByManagerName: didnt find manager with name: " + _managerName;
	//throw std::runtime_error(exceptionMessage);
	std::cout << exceptionMessage << std::endl;
	return nullptr;
}

int Hierarchy::countIndirectSubordinates(const Node* pNode) const
{
	/*int allSubordinates = this->countAllSubordinatesOfNode(pNode);

	return allSubordinates - pNode->subordinates.size();*/

	if (pNode == nullptr) return 0;

	int indirectSubordinates = 0;

	queue<const Node*> bfsQueue;
	for (Node* pSubordinate : pNode->subordinates) {
		bfsQueue.push(pSubordinate);
	}

	while (!bfsQueue.empty()) {
		const Node* pCurrent = bfsQueue.front();
		bfsQueue.pop();

		indirectSubordinates += pCurrent->subordinates.size();

		// Pushes children in queue, if there are children of pCurrent
		for (const Node* subordinate : pCurrent->subordinates) {
			bfsQueue.push(subordinate);
		}
	}

	return indirectSubordinates;
}

bool Hierarchy::isNodeOverloaded(const Node* nodeToCheck, int overloadLevel) const
{
	if (this->root == nullptr) return false;

	if (nodeToCheck == nullptr) return false;

	int directSubordinates = nodeToCheck->subordinates.size();

	if (directSubordinates > overloadLevel) return true;

	int indirectSubordinates = this->countIndirectSubordinates(nodeToCheck);

	return (directSubordinates + indirectSubordinates) > overloadLevel;
}

void Hierarchy::eraseHierarchy(Node*& root)
{
	if (root == nullptr) return;

	for (Node*& subordinate : root->subordinates) eraseHierarchy(subordinate);

	// delete root must be the last operation
	delete root;
	root = nullptr;
}

int Hierarchy::hierarchyHeight(const Node* root) const
{
	if (root == nullptr) return 0;

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

int Hierarchy::countAllSubordinatesOfNode(const Node* pNode) const
{
	if (pNode == nullptr) return 0;

	if (pNode->subordinates.empty()) return 0;

	int subordinatesCounter = 0;

	queue<const Node*> bfsQueue;
	bfsQueue.push(pNode);

	while (!bfsQueue.empty()) {
		const Node* pCurrent = bfsQueue.front();
		bfsQueue.pop();

		// Pushes children in queue, if there are children of pCurrent
		for (const Node* subordinate : pCurrent->subordinates) {
			subordinatesCounter++;
			bfsQueue.push(subordinate);
		}
	}

	return subordinatesCounter;
}

int Hierarchy::countAllSubordinatesOfNodeRecursively(const Node* pNode) const
{
	if (pNode == nullptr) return 0;

	if (pNode->subordinates.empty()) return 0;

	return this->countSubordinatesRecursively(pNode);
}

int Hierarchy::countSubordinatesRecursively(const Node* pNode, int currentAmount) const
{
	//std::cout << "Current node: " << pNode->name <<  " currentAmont: " << currentAmount << std::endl;
	if (pNode == nullptr) return 0;

	for (const Node* subordinate : pNode->subordinates) {
		currentAmount = this->countSubordinatesRecursively(subordinate, currentAmount + 1);
	}

	// std::cout << "            : " << pNode->name << " currentAmont: " << currentAmount << std::endl;
	return currentAmount;
}

void Hierarchy::addToHierachy(const string& _managerName, const string& _subordinateName)
{
	bool managerExists = this->find(_managerName);
	
	if (managerExists) {
		Node* pManagerNode = this->findNodeByName(_managerName);
		if (pManagerNode != nullptr) {
			pManagerNode->subordinates.push_back(new Node(_subordinateName));
			//pManagerNode->subordinates.sort(Hierarchy::Node::NodePointerComparator());
		}
	} else {
		throw std::invalid_argument("Invalid input! Manager not found!");
	}
}

//int Hierarchy::countIndirectSubordinates(const std::string& name) const
//{
//	const Hierarchy::Node* pNodeWithName = this->findNodeByName(name);
//	if (pNodeWithName == nullptr)
//		return 0;
//	else
//		return this->countIndirectSubordinates(pNodeWithName);
//}

string Hierarchy::manager(const string& name) const
{
	if (this->root->name == name) return "";

	if (this->find(name) && this->root != nullptr) {
		string managerName;
		queue<const Node*> bfsQueue;
		bfsQueue.push(this->root);

		while (!bfsQueue.empty()) {
			size_t bfsQueueCurrentSize = bfsQueue.size();

			const Node* pCurrent = bfsQueue.front();
			bfsQueue.pop();

			for (const Hierarchy::Node* pNode: pCurrent->subordinates) {
				if (pNode->name == name) {
					managerName = pCurrent->name;
					break;
				}
			}

			if (!managerName.empty()) return managerName;

			// Pushes children in queue, if there are children of pCurrent
			for (const Node* subordinate : pCurrent->subordinates) {
				bfsQueue.push(subordinate);
			}
		}
	}
}

int Hierarchy::num_subordinates(const string& name) const
{
	const Hierarchy::Node* nodeWithName = this->findNodeByName(name);

	if (nodeWithName != nullptr) {
		return nodeWithName->subordinates.size();
	} else
	{ 
		// Stays for debugging purposes
		//std::cout << "num_subordinates: returning -1, because node with name wasnt found in the tree." << std::endl;
		return -1;
	}
}

unsigned long Hierarchy::getSalary(const string& who) const
{
	const Node* pWho = this->findNodeByName(who);

	if (pWho != nullptr) {
		int pWhoDirectSubordinates = pWho->subordinates.size();
		int pWhoIndirectSubordinates = this->countIndirectSubordinates(pWho);
		return pWhoDirectSubordinates * SALARY_PER_DIRECT_SUBORDINATE + pWhoIndirectSubordinates * SALARY_PER_INDIRECT_SUBORDINATE;
	} else 
		return -1;
}

bool Hierarchy::fire(const string& who)
{
	if (!this->find(who)) {
		// Stays for debugging purposes
		//std::cout << "fire: Couldn't find " + who + " in Hierarchy." << std::endl;
		return false;
	} else if (who == THE_BOSS_NAME) {
		// Stays for debugging purposes
		//std::cout << "fire: Cant't fire Uspeshnia!" << std::endl;
		return false;
	} else {
		Node*		pWho			= this->findNodeByName(who);
		std::string whoManagerName	= this->manager(who);
		Node*		pWhoManager		= this->findNodeByName(whoManagerName);

		pWhoManager->subordinates.remove_if([pWho](const Node* pNode) {return pNode->name == pWho->name; });

		for (Node* subordinate : pWho->subordinates) {
			pWhoManager->subordinates.push_back(subordinate);
		}
		//pWhoManager->subordinates.sort(Hierarchy::Node::NodePointerComparator());

		delete pWho;
		// Stays for debugging purposes
		//std::cout << "fire: Fired " + who + " successfully." << std::endl;
		return true;
	}
}

bool Hierarchy::hire(const string& who, const string& boss)
{
	
	if (!this->find(boss)) {
		// Stays for debugging purposes
		//std::cout << "hire: Boss with name " << boss << " not found" << std::endl;
		return false;
	}

	if (this->find(who)) {
		// remove from current manager's subordinates; add to boss's subordinates
		Node* pNodeWho = this->findNodeByName(who);
		Node* pNodeWhoCurrentManager = this->findNodeByName(this->manager(who));
		pNodeWhoCurrentManager->subordinates.remove_if([who](const Node* pNode) {return pNode->name == who; });
		
		this->addToHierachy(boss, who);

		Node* updatedPNodeNew = this->findNodeByName(who);

		for (Node* subordinate : pNodeWho->subordinates) {
			updatedPNodeNew->subordinates.push_back(subordinate);
		}
		
		delete pNodeWho;
	} else {
		this->addToHierachy(boss, who);
	}

	return true;
}

list<Hierarchy::Node*> Hierarchy::getNodeSubordinatesOrderedBySalaryDescendingAndNameAscending(const Node* pNode)
{
	list<Node*> subordinatesCopyConstPtrs;
	
	for (Node* subordinate : pNode->subordinates) subordinatesCopyConstPtrs.push_back(subordinate);

	subordinatesCopyConstPtrs.sort([this](const Node* pNode1, const Node* pNode2) {
		size_t pNode1Salary = this->getSalary(pNode1->name);
		size_t pNode2Salary = this->getSalary(pNode2->name);

		return pNode2Salary != pNode1Salary ? pNode1Salary > pNode2Salary : pNode1->name < pNode2->name;
	});

	return subordinatesCopyConstPtrs;
}

void Hierarchy::incorporateNode(Node* pNode)
{
	if (pNode == nullptr) return;

	if (pNode->subordinates.empty()) return;

	for (Node* subordinate : pNode->subordinates) {
		incorporateNode(subordinate);
	}

	std::list<Node*> pNodeOrderedSubordinates = this->getNodeSubordinatesOrderedBySalaryDescendingAndNameAscending(pNode);

	Node* newBoss = pNodeOrderedSubordinates.front();

	std::list<Node*> newBossList = std::list<Node*>({newBoss});

	pNodeOrderedSubordinates.pop_front(); // all subordinates without boss

	// all other subordinates apart from from list of subordinates are getting added to the subordinates of newBoss;
	//if (pNodeOrderedSubordinates.size() >= 2) {
	for (Node* sub : pNodeOrderedSubordinates) {
	pNode->subordinates.remove_if([sub](Node* pNode) {return pNode->name == sub->name; });
	newBoss->subordinates.push_back(sub);
	}
}

void Hierarchy::sortNodeSubordinatesRecursively(Node* pNode) const 
{
	if (pNode == nullptr) return;

	if (pNode->subordinates.empty()) { return; }

	for (Node* subordinate : pNode->subordinates) {
		sortNodeSubordinatesRecursively(subordinate);
	}

	pNode->subordinates.sort(Hierarchy::Node::NodePointerComparator());
}

int Hierarchy::getLevelOfNodeByName(const string& nodeName) const
{
	int level = 0;

	if (this->root->name == nodeName) return 0;
	else {
		queue<const Node*> bfsQueue;
		bfsQueue.push(this->root);

		while (!bfsQueue.empty()) {
			const Node* pCurrent = bfsQueue.front();
			bfsQueue.pop();

			// Pushes children in queue, if there are children of pCurrent
			for (const Node* subordinate : pCurrent->subordinates) {
				if (subordinate == pCurrent->subordinates.front()) level++;
				if (subordinate->name == nodeName) return level;
				bfsQueue.push(subordinate);
			}
		}
	}

	std::cout << "getLevelByNodeName: Node with name " << nodeName << " not found in hierarchy. Return value -1" << std::endl;
	return -1; // not found
}

void Hierarchy::incorporate()
{
	//Node* bogdan = this->findNodeByName("Bogdan");
	this->incorporateNode(this->root);
}

void Hierarchy::modernizeNode(const Node* pNode, int currentLevel) {
	if (pNode == nullptr) return;

	if (pNode->subordinates.empty()) return;

	std::vector<string> peopleToFire;
	for (Node* subordinate : pNode->subordinates) {
		modernizeNode(subordinate, currentLevel + 1);
		if (currentLevel % 2 == 0 && !subordinate->subordinates.empty()) {
			peopleToFire.push_back(subordinate->name);
		}
	}

	for (const string& personToFire : peopleToFire) this->fire(personToFire);
}

void Hierarchy::modernize()
{
	this->modernizeNode(this->root, 0);
}

Hierarchy Hierarchy::join(const Hierarchy& right) const
{
	Hierarchy result = Hierarchy();

	queue<const Node*> bfsQueue;
	bfsQueue.push(this->root);

	size_t pCurrentLevel = 0;
	while (!bfsQueue.empty()) {
		const Node* pCurrent = bfsQueue.front();
		bfsQueue.pop();

		if (pCurrent->name != THE_BOSS_NAME) { // assuming all Hierarchys have "Uspeshnia" for root node

			string pCurrentManager = this->manager(pCurrent->name);
			bool pCurrentExistsInRight = right.find(pCurrent->name);

			if (pCurrentExistsInRight) {
				string pCurrentRightTreeOccuranceManager = right.manager(pCurrent->name);
				int pCurrentRightTreeOccuranceManagerLevel = right.getLevelOfNodeByName(pCurrentRightTreeOccuranceManager);

				int	pCurrentManagerLevel = pCurrentLevel - 1;

				if (pCurrentManager != pCurrentRightTreeOccuranceManager && pCurrentManagerLevel != pCurrentRightTreeOccuranceManagerLevel) {
					if (pCurrentManagerLevel < pCurrentRightTreeOccuranceManagerLevel) { //pCurrentManager is higher in the Hierarchy
						result.addToHierachy(pCurrentManager, pCurrent->name);
					}
					else if (pCurrentManagerLevel > pCurrentRightTreeOccuranceManagerLevel) { // pCurentRightTreeOccuranceManagerLevel is higher in the Hierarchy
						result.addToHierachy(pCurrentRightTreeOccuranceManager, pCurrent->name);
					}
					else {// pCurrentManager and pCurrentRightTreeOccuranceManagerLevel are equal => choose lexicographically;
						result.addToHierachy((pCurrentManager < pCurrentRightTreeOccuranceManager) ? pCurrentManager : pCurrentRightTreeOccuranceManager, pCurrent->name);
					}
				}
				else {  // pCurrentManager's name == pCurrentRightTreeOccuranceManager's name AND they're on the same level in both
					result.addToHierachy(pCurrentManager, pCurrent->name);
				}

			}
			else {// pCurrent exists only in lhs Hierarchy
				result.addToHierachy(pCurrentManager, pCurrent->name);
			}
		}

		for (const Node* subordinate : pCurrent->subordinates) {
			if (subordinate == pCurrent->subordinates.front()) pCurrentLevel++;
			bfsQueue.push(subordinate);
		}
	}

	queue<const Node*> BfsQueueRHS;
	BfsQueueRHS.push(right.root);

	while (!BfsQueueRHS.empty()) {
		const Node* pCurrentRHS = BfsQueueRHS.front();
		BfsQueueRHS.pop();

		if (pCurrentRHS->name != THE_BOSS_NAME) { // assuming all Hierarchys have "Uspeshnia" for root node

			string pCurrentManagerRHS = right.manager(pCurrentRHS->name);
			bool pCurrentExistsInResult = result.find(pCurrentRHS->name);

			if (!pCurrentExistsInResult) {
				string pCurrentRightTreeOccuranceManager = right.manager(pCurrentRHS->name);
				result.addToHierachy(pCurrentManagerRHS, pCurrentRHS->name);
			}
			else
			{
				//std::cout << "pCurrentRHS with name: " << pCurrentRHS->name << " already exists in result." << std::endl;
			}
		}

		for (const Node* subordinate : pCurrentRHS->subordinates) {
			BfsQueueRHS.push(subordinate);
		}
	}

	return result;
}