#include <iostream>
#include <vector>
#include <forward_list>
#include <queue>
#include <time.h>

using namespace std;

// Find height by a given Node
// using DFS

struct Node
{
    int data;

    Node* parent;
    std::vector<Node*> children;

    Node() : parent{ nullptr }, data{ 0 } {}

    Node(Node* _parent, int _data) {
        parent = _parent;
        data = _data;
    }   
};

Node* createTree()
{
    Node* root  = new Node();
    root->children.push_back(new Node(root, 1));
    root->children.push_back(new Node(root, 2));
    root->children.push_back(new Node(root, 3));

    root->children[0]->children.push_back(new Node(root->children[0], 4));
    root->children[1]->children.push_back(new Node(root->children[1], 5));
    root->children[1]->children.push_back(new Node(root->children[1], 6));
    root->children[2]->children.push_back(new Node(root->children[2], 7));

    root->children[2]->children[0]->children.push_back(new Node(root->children[2]->children[0], 8));
    root->children[2]->children[0]->children.push_back(new Node(root->children[2]->children[0], 9));
    root->children[2]->children[0]->children.push_back(new Node(root->children[2]->children[0], 10));
    root->children[2]->children[0]->children.push_back(new Node(root->children[2]->children[0], 11));
    root->children[2]->children[0]->children.push_back(new Node(root->children[2]->children[0], 12));

    return root;
}

void deleteTree(const Node* root)
{
    for (const Node* child : root->children) 
    {
        deleteTree(child);
    }

    delete root;
}

// Tree's height
// DFS, using stack frame recursively
size_t getTreeHeight(const Node* root)
{
    size_t maxChildHeight = 0;

    for (const Node* child : root->children) // using the stack frame's base (Node - leaf)
    {
        size_t currentChildHeight = getTreeHeight(child);
        if (currentChildHeight > maxChildHeight)
        {
            maxChildHeight = currentChildHeight;
        }
    }
        
    return 1 + maxChildHeight;
}

// Tree's degree / branching factor
// (Maximum number of children of a node in the tree)
// DFS, using stack frame recursively
size_t getTreeDegree(const Node* root) 
{
    size_t maxChildDegree = root->children.size();

    for (const Node* child : root->children) // using the stack frame's base (Node - leaf)
    {
        size_t currChildDegree = getTreeDegree(child);
        if (currChildDegree > maxChildDegree)
        {
            maxChildDegree = currChildDegree;
        }
    }

    return maxChildDegree;
}

void printLevel(const Node* root, size_t level)
{
    if (level == 0)
    {
        cout << "[" << root->data << "]" << " ";
        return;
    }

    for (const Node* child : root->children)
    {
        printLevel(child, level - 1);
    }
}

void printTree(const Node* root)
{
    printLevel(root, 0); cout << endl;

    size_t height = getTreeHeight(root);
    for (size_t i{0}; i < height; ++i)
    {
        printLevel(root, i); cout << endl;
    }
    
}

bool isVisited(const vector<const Node*>& visited, const Node* node)
{
    return std::find(visited.begin(), visited.end(), node) != visited.end();
}

// Returns a vector of leaves, using BFS
vector<const Node*> getLeavesBFS(const Node* root)
{
    vector<const Node*> leaves;

    vector<const Node*>   visited;
    queue<const Node*>    wave;

    wave.push(root);
    while (!wave.empty())
    {
        const Node* currentNode = wave.front();
        visited.push_back(currentNode);

        if (currentNode->children.empty())
            leaves.push_back(currentNode);

        for (const Node* child : currentNode->children)
        {
            if (!isVisited(visited, child))
                wave.push(child);
        }

        wave.pop();
    }

    return leaves;
}

// Returns a vector of leaves, using DFS
vector<const Node*> getLeavesDFS(const Node* root, vector<const Node*>& leavesSoFar)
{
    if (root->children.empty()) 
    {
        leavesSoFar.push_back(root);
        return leavesSoFar;
    }

    for (const Node* child : root->children)
    {
        getLeavesDFS(child, leavesSoFar);
    }

    return leavesSoFar;
}

int main()
{
    Node* root = createTree();
    cout << "Tree's height: " << getTreeHeight(root) << endl;
    cout << "Tree's degree: " << getTreeDegree(root) << endl;
    printTree(root); cout << endl;

    clock_t dfsStart = clock();
    vector<const Node*> leaves;
    leaves = getLeavesDFS(root, leaves);
    cout << "leaves: ";
    for (const Node* leaf : leaves)
    {
        cout << "[" << leaf->data << "] ";
    }
    cout << endl;
    cout << "Time taken for DFS: " << (double)(clock() - dfsStart) / CLOCKS_PER_SEC << "s" << endl;

    clock_t bfsStart = clock();
    leaves = getLeavesBFS(root);
    cout << "leaves: ";
    for (const Node* leaf : leaves)
    {
        cout << "[" << leaf->data << "] ";
    }
    cout << endl;
    cout << "Time taken for BFS: " << (double)(clock() - bfsStart) / CLOCKS_PER_SEC << "s" << endl;

    return 0;
}
