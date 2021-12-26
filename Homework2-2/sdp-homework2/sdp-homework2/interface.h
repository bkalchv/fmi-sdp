#pragma once

#include <string>
#include <list>

#include<stdexcept>

using std::string;
using std::list;

class Hierarchy
{
public:
    Hierarchy() noexcept : root(new Node("Uspeshnia")) {};
    Hierarchy(Hierarchy&&) noexcept;
    Hierarchy(const Hierarchy& r);
    //Hierarchy(const string& data); // TODO
    ~Hierarchy();
    void operator=(const Hierarchy&) = delete;

    string print()const;

    int longest_chain() const;
    bool find(const string& name) const;
    int num_employees() const;
    int num_overloaded(int level = 20) const;

    string manager(const string& name) const;
    int num_subordinates(const string& name) const;
    unsigned long getSalary(const string& who) const;

    bool fire(const string& who);
    bool hire(const string& who, const string& boss);

    void incorporate();
    void modernize();

    Hierarchy join(const Hierarchy& right) const;

    //If you need it - add more public methods here


private:
    //Add whatever you need here
    struct Node {
        string name;
        list<Node*> subordinates;

        Node(const string& _name) : name(_name) {};
        Node(const string& _name, const std::initializer_list<Node*> _subordinates) : name(_name) {
            for (Node* const pNode : _subordinates) {
                this->subordinates.push_back(pNode);
            }
        };
        string getName() const { return this->name; }

        struct NodePointerComparator {
            bool operator() (const Node* pNode1, const Node* pNode2) {
                if (pNode1 != nullptr && pNode2 != nullptr) {
                    return pNode1->name < pNode2->name;
                }
            }
        };

    };
    Node* root;

    Node*   findNodeByManagerName(const string&) const;
    Node*   findNodeByManagerName(const string&);
    bool    isNodeOverloaded(const Node*, int overloadLevel) const;
    void    eraseHierarchy(Node*&);
    int     hierarchyHeight(const Node*) const;
    Node*   copyNode(const Node*);

public:
    list<Node*> getRootSubordinates() const { return this->root->subordinates; }
    list<Node*> getNodeSubordinates(Node* const _node) const { return _node->subordinates; }
    void addToHierachy(const string&, const string&);
};