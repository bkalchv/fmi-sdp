#include <iostream>
#include <optional>
#include <vector>

using namespace std;

struct BSTNode
{
    int data;
    BSTNode* leftChild;
    BSTNode* rightChild;

    BSTNode(int _data, BSTNode* _leftChild = nullptr, BSTNode* _rightChild = nullptr) : data{ _data }, leftChild{ _leftChild }, rightChild{_rightChild} {}
};

BSTNode* initTree()
{
    return new BSTNode(0);
}

BSTNode* initTree(int data)
{
    return new BSTNode(data);
}

void deleteBST(BSTNode*& root)
{   
    if (root)
    {
        deleteBST(root->leftChild);
        deleteBST(root->rightChild);
        delete root;
        root = nullptr;
    }
}

bool addBSTNode(BSTNode*& root, int data)
{
    if (!root)
    {
        root = new BSTNode(data);
        cout << "Node with value " << data << " successfully added to BST." << endl;
        return true;
    }

    if (data <= root->data)
    {
        addBSTNode(root->leftChild, data);
    }
    else
    {
        addBSTNode(root->rightChild, data);
    }

    return false;
}

bool isLeaf(const BSTNode* node)
{
    return node && !node->leftChild && !node->rightChild;
}

// Prints the BST Inorder (Left, Root, Right); Huge bonus - Everything is sorted well.
void printInorder(const BSTNode* root)
{
    if (root)
    {
        printInorder(root->leftChild);
        cout << "[" << root->data << "] ";
        printInorder(root->rightChild);
    }
}

void printPreorder(const BSTNode* root)
{
    if (root)
    {
        cout << "[" << root->data << "] ";
        printPreorder(root->leftChild);
        printPreorder(root->rightChild);
    }
}

void printPostorder(const BSTNode* root)
{
    if (root)
    {
        printPostorder(root->leftChild);
        printPostorder(root->rightChild);
        cout << "[" << root->data << "] ";
    }
}

using ref_wrapper = std::optional<std::reference_wrapper<int>>;
ref_wrapper findData(BSTNode* root, const int& dataToLookFor)
{
    if (root)
    {
        if (dataToLookFor == root->data)
        {
            return root->data;
        }

        if (dataToLookFor <= root->data)
        {
            return findData(root->leftChild, dataToLookFor);
        }

        if (dataToLookFor > root->data)
        {
            return findData(root->rightChild, dataToLookFor);
        }
    }

    return std::nullopt;
}

using const_ref_wrapper = std::optional<std::reference_wrapper<const int>>;
const_ref_wrapper findData(const BSTNode* root, const int& dataToLookFor)
{
    if (root)
    {
        if (dataToLookFor == root->data)
        {
            return root->data;
        }

        if (dataToLookFor <= root->data)
        {
            return findData(root->leftChild, dataToLookFor);
        }

        if (dataToLookFor > root->data)
        {
            return findData(root->rightChild, dataToLookFor);
        }
    }

    return std::nullopt;
}

// Using Inorder traversial
bool isValidBST(const BSTNode* root)
{
    if (root)
    {
        int currentData = root->data;

        if (root->leftChild && root->leftChild->data > currentData)
        {
            return false;
        }
        else
        {
            isValidBST(root->leftChild);
        }
        
        if (root->rightChild && root->rightChild->data <= currentData)
        {
            return false;
        }
        else
        {
            isValidBST(root->rightChild);
        }
    }

    return true;
}

typedef void (*Function) (int&); 

void square(int& x)
{
    x *= x;
}

void tree_map(BSTNode* root, Function f)
{
    if (root)
    {
        f(root->data);
        tree_map(root->leftChild, f);
        tree_map(root->rightChild, f);
    }
}

vector<const BSTNode*> getVectorOfLeaves(const BSTNode* root, vector<const BSTNode*>& currentVectorOfLeaves)
{
    if (root)
    {
        if (isLeaf(root))
        {
            currentVectorOfLeaves.push_back(root);
            return currentVectorOfLeaves;
        }
        else
        {
            getVectorOfLeaves(root->leftChild, currentVectorOfLeaves);
            getVectorOfLeaves(root->rightChild, currentVectorOfLeaves);
        }
    }

    return currentVectorOfLeaves;
}

void deleteLeaf(BSTNode*& node)
{
    if (isLeaf(node))
    {
        delete node;
        node = nullptr;
    }
}

void trim(BSTNode*& root)
{   
    if (isLeaf(root))
    {
        deleteLeaf(root);
        return;
    }
    else
    {
        trim(root->leftChild);
        trim(root->rightChild);
    }
}

int main()
{
    BSTNode* root = initTree(1);
    addBSTNode(root, 5);
    addBSTNode(root, 3);
    addBSTNode(root, 6);
    addBSTNode(root, 0);

    cout << "Inorder: "; printInorder(root); cout << endl;
    cout << "Preorder: "; printPreorder(root); cout << endl;
    cout << "Postorder: "; printPostorder(root); cout << endl;

    const_ref_wrapper const_ref_find1 = findData(root, 1);
    if (const_ref_find1.has_value())
    {
        cout << const_ref_find1.value() << " found" << endl;
    }
    else
    {
        cout << "1 not found!" << endl;
    }

    const_ref_wrapper const_ref_find5 = findData(root, 5);
    if (const_ref_find5.has_value())
    {
        cout << const_ref_find5.value() << " found!" << endl;
    }
    else
    {
        cout << "5 not found!" << endl;
    }

    if (isValidBST(root))
    {
        cout << "BST is valid!" << endl;
    }

    ref_wrapper ref_find5 = findData(root, 5);
    ref_find5.value().get() = -1;
    cout << "5 assigned to -1" << endl;

    if (isValidBST(root))
    {
        cout << "BST is still valid! (Unexpected behavior)" << endl;
    }
    else
    {
        cout << "BST is now invalid!" << endl;
    }

    cout << "Invalid BST: ";
    printInorder(root); cout << endl;

    ref_find5.value().get() = 5;
    cout << "-1 assigned back to 5" << endl;
    printInorder(root); cout << endl;

    if (isValidBST(root))
    {
        cout << "BST is valid again!" << endl;
    }
    else
    {
        cout << "BST is still invalid! (unexpected behavior)" << endl;
    }

    Function sqrt = square;
    tree_map(root, sqrt);

    cout << "BST after sqrt for each node: "; printInorder(root); cout << " (Inorder)" << endl;
    if (isValidBST(root))
    {
        cout << "BST is still valid!" << endl;
    }
    else
    {
        cout << "BST is invalid after sqrt for each node" << endl;
    }

    vector<const BSTNode*> leaves;
    getVectorOfLeaves(root, leaves);

    if (!leaves.empty())
    {
        cout << "Leaves found: ";
        for (const BSTNode* leaf : leaves)
        {
            cout << "[" << leaf->data << "] ";
        }
        cout << endl;
    }
    else
    {
        cout << "No leaves found!";
    }

    trim(root);
    cout << "BST after trim: "; printInorder(root); cout << endl;

    for (const BSTNode* leaf : leaves)
    {
        if (findData(root, leaf->data) != std::nullopt)
        {
            cout << "A leaf hasn't been deleted after trim! (Unexpected behavior.)";
        }
    }

    return 0;
}