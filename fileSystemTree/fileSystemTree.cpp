#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Node
{
	string dirName;
	Node* parent;
	vector<Node*> children;

	Node() : dirName{ "/" }, parent{ nullptr } {}
	Node(string _dirName, Node* _parent) : dirName{ _dirName }, parent{ _parent } {}
};

Node* createTree()
{
	Node* root = new Node();

	// Level 1
	root->children.push_back(new Node("bin", root));
	root->children.push_back(new Node("dev", root));
	root->children.push_back(new Node("etc", root));
	root->children.push_back(new Node("home", root));
	root->children.push_back(new Node("lib", root));
	root->children.push_back(new Node("mnt", root));
	root->children.push_back(new Node("proc", root));
	root->children.push_back(new Node("root", root));
	root->children.push_back(new Node("sbin", root));
	root->children.push_back(new Node("tmp", root));
	root->children.push_back(new Node("usr", root));

	// Level 2
	root->children[0]->children.push_back(new Node("cp", root->children[0]));
	root->children[0]->children.push_back(new Node("ksh", root->children[0]));
	root->children[0]->children.push_back(new Node("is", root->children[0]));
	root->children[0]->children.push_back(new Node("pwd", root->children[0]));

	root->children[2]->children.push_back(new Node("passwd", root->children[2]));

	root->children[3]->children.push_back(new Node("mthomas", root->children[3]));
	root->children[3]->children.push_back(new Node("stul", root->children[3]));

	root->children[9]->children.push_back(new Node("bin", root->children[9]));

	// Level 3
	root->children[3]->children[0]->children.push_back(new Node("bin", root->children[3]->children[0]));
	root->children[3]->children[0]->children.push_back(new Node("class_stuff", root->children[3]->children[0]));
	root->children[3]->children[0]->children.push_back(new Node("profile", root->children[3]->children[0]));

	// Level 4
	root->children[3]->children[0]->children[1]->children.push_back(new Node("foo", root->children[3]->children[0]->children[1]));
	root->children[3]->children[0]->children[1]->children.push_back(new Node("bar", root->children[3]->children[0]->children[1]));

	return root;
}

void deleteTree(Node* root)
{
	for (Node* child : root->children)
	{
		deleteTree(child);
	}

	root->parent = nullptr;
	delete root;
}

size_t getTreeHeight(const Node* root)
{
	size_t maxChildHeight = 0;

	for (const Node* child : root->children)
	{
		size_t currentChildHeight = getTreeHeight(child);
		if (currentChildHeight > maxChildHeight)
		{
			maxChildHeight = currentChildHeight;
		}
	}

	return 1 + maxChildHeight;
}

void printLevel(const Node* root, int level)
{
	if (level == 0)
	{
		cout << "[" << root->dirName << "] ";
		return;
	}

	for (const Node* child : root->children)
	{
		printLevel(child, level - 1);
	}
}

void printLevels(const Node* root)
{

	size_t treeHeight = getTreeHeight(root);
	for (size_t i{ 0 }; i < treeHeight; ++i)
	{
		printLevel(root, i); cout << endl;
	}
}

bool isVisited(const vector<const Node*>& visited, const Node* node)
{
	return std::find(visited.begin(), visited.end(), node) != visited.end();
}

const Node* BFSfindNode(const Node* startNode, const std::string& target)
{
	std::vector<const Node*>		visited;
	std::queue<const Node*>	wave;

	wave.push(startNode);
	while (!wave.empty())
	{
		const Node* currentNode = wave.front();

		if (currentNode->dirName == target)
		{
			return currentNode;
		}

		visited.push_back(currentNode);

		for (Node* child : currentNode->children)
			if (!isVisited(visited, child))
				wave.push(child);

		wave.pop();
	}

	cout << "BFS went throgh the whole tree and didnt find the target directory! " << endl;
	return nullptr;
}

std::vector<const Node*> getRelativePath(const Node* root, const std::string& fromDirectory, const std::string& toDirectory)
{
	std::vector<const Node*> reversePath;

	const Node* fromNode = BFSfindNode(root, fromDirectory);
	if (!fromNode)
	{
		cout << "Starting directory not found!";
	}

	const Node* toNode = BFSfindNode(root, toDirectory);
	if (!toNode)
	{
		cout << "Target directory not found!";
	}

	if (!fromNode || !toNode)
	{
		return vector<const Node*>();
	}

	size_t treeHeight = getTreeHeight(root);
	
	for (size_t i{ 0 }; i < treeHeight; ++i)
	{
		reversePath.push_back(toNode);
		toNode = toNode->parent;

		if (toNode->dirName == fromNode->dirName)
		{
			reverse(reversePath.begin(), reversePath.end());
			return reversePath;
			break;
		}
	}

	return vector<const Node*>();
}

int main()
{
	Node* root = createTree();
	printLevels(root);

	string fromDirectory = "/";
	string toDirectory = "bar";

	vector<const Node*> relativePath = getRelativePath(root, fromDirectory, toDirectory);

	if (relativePath.empty())
	{
		cout << "Relative path not found!" << endl;
	}
	else
	{
		cout << "Relative path from \"" << fromDirectory << "\" to \"" << toDirectory << "\": ./";
		for (const Node* node : relativePath)
		{	
			cout << node->dirName;
			if (node->dirName != toDirectory)
			{
				 cout << "/";
			}
		}
		cout << endl;
	}

	deleteTree(root);
	return 0;
}