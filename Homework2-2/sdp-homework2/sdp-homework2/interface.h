#pragma once

#include <sstream>
#include <string>
#include <list>

using std::string;
using std::list;

const size_t SALARY_PER_DIRECT_SUBORDINATE      = 500;
const size_t SALARY_PER_INDIRECT_SUBORDINATE    = 50;
const string THE_BOSS_NAME                          = "Uspeshnia";

class Hierarchy
{
public:
    Hierarchy() noexcept : root(new Node(THE_BOSS_NAME)) {};
    Hierarchy(Hierarchy&&) noexcept;
    Hierarchy(const Hierarchy& r);
    Hierarchy(const string& data);
    ~Hierarchy();
    void operator=(const Hierarchy&) = delete;

    string print()const;

    int longest_chain() const;
    bool find(const string& name) const;
    int num_employees() const;
    int num_employees_recursively() const;
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
    static bool containsDelimiterOnce(const string& token, const string& delimiter)
    {
        size_t counterOfDelimiterOccurances = 0;
        size_t delimiterPosition = token.find(delimiter, 0);
        while (delimiterPosition != std::string::npos) {
            counterOfDelimiterOccurances++;
            if (counterOfDelimiterOccurances > 1) break;
            delimiterPosition = token.find(delimiter, delimiterPosition + 1);
        }
        if (counterOfDelimiterOccurances > 1) return false;
        if (counterOfDelimiterOccurances == 1) return true;
    }
    static unsigned int countNonWhitespaceStrings(const string& str) {
        std::stringstream  stream(str);
        std::string        oneWord;
        unsigned int       count = 0;

        while (stream >> oneWord) { count++; }
        return count;
    }
    static bool isValidToken(const string& token)
    {
        std::string expectedDelimiter = "-";
        if (token.find(expectedDelimiter) == std::string::npos) return false;
        if (!containsDelimiterOnce(token, expectedDelimiter)) return false;

        std::string firstToken = token.substr(0, token.find(expectedDelimiter));
        if (firstToken.size() == 0) return false;

        if (countNonWhitespaceStrings(firstToken) != 1) return false;

        std::string secondToken = token.substr(token.find(expectedDelimiter) + expectedDelimiter.length());
        if (secondToken.size() == 0) return false;

        if (countNonWhitespaceStrings(secondToken) != 1) return false;

        return true;
    }
    static std::string removeWhitespacesFromString(string str) {
        str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
        return str;
    }
    
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

    const Node*             findNodeByName(const string&) const;
    Node*                   findNodeByName(const string&);
    
    bool                    isNodeOverloaded(const Node*, int overloadLevel) const;
    void                    eraseHierarchy(Node*&);
    int                     hierarchyHeight(const Node*) const;
    Node*                   copyNode(const Node*);
    int                     countAllSubordinatesOfNode(const Node*) const;
    int                     countAllSubordinatesOfNodeRecursively(const Node*) const;
    int                     countSubordinatesRecursively(const Node*, int currentAmount = 0) const;
    int                     countIndirectSubordinates(const Node*) const;
    list<Node*>             getNodeSubordinatesOrderedBySalaryDescendingAndNameAscending(const Node*);
    void                    incorporateNode(Node*);
    void                    sortNodeSubordinatesRecursively(Node*) const;
    int                     getLevelOfNodeByName(const string&) const;
    void                    modernizeNode(const Node*, int);

public:
    void            addToHierachy(const string&, const string&);
    //int         countIndirectSubordinates(const std::string&) const;
};