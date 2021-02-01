﻿#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <sstream>
#include <fstream>

#include "Edge.h"
#include "Station.h"
#include "Instrument.h"

using namespace std;

using AdjacencyList = unordered_set<Edge, EdgeHasher>;
using Graph = unordered_map<unsigned int, AdjacencyList>;

void printGraph(const Graph& graph)
{
    for (const pair<unsigned int, AdjacencyList>& p : graph)
    {
        cout << "[" << p.first << "]";

        if (!p.second.empty())
        {
            cout << ": ";
            for (AdjacencyList::const_iterator it = p.second.begin(); it != p.second.end(); ++it)
            {
                Station adjacentStation = it->toStation;
                cout << "->";
                adjacentStation.print();
                cout << "(" << it->weight << ")";
            }
        }

        cout << endl;
    }
}

enum class userChoice
{
    AddStation,
    RemoveStation,
    AddRoute,
    RemoveRoute,
    AddLine,
    RemoveLine,
    Quit,
    Back,
    Error
};

using choicePair = pair<const string&, const string&>;

userChoice toEnum(const choicePair& choice)
{
    if (choice.first == "1")
    {
        if (choice.second == "1") return userChoice::AddStation;
        if (choice.second == "2") return userChoice::AddRoute;
        if (choice.second == "3") return userChoice::AddLine;
        if (choice.second == "4") return userChoice::Back;
    }
    else if (choice.first == "2")
    {
        if (choice.second == "1") return userChoice::RemoveStation;
        if (choice.second == "2") return userChoice::RemoveRoute;
        if (choice.second == "3") return userChoice::RemoveLine;
        if (choice.second == "4") return userChoice::Back;
    }
    else if (choice.first == "3")
    {
        return userChoice::Quit;
    }
  
    return userChoice::Error;
}

int main()
{
    //try 
    //{
    //    string filename = "Text.txt";
    //    FileManager fmanager = FileManager(filename);
    //    Graph g = fmanager.readGraph();
    //    printGraph(g);
    //}
    //catch (const std::exception& ex) 
    //{
    //    cerr << ex.what() << endl;
    //}

    const MenuItem* chosenAction;
    const MenuItem* chosenObject;
    
    string      filename = "PublicTransport.txt";
    FileManager fmanager = FileManager(filename);
    Instrument  instrument = Instrument(filename);

    while (true)
    {
        try
        {
            Instrument::display(Instrument::mainMenu);
            chosenAction = Instrument::getChoice(Instrument::mainMenu);

            string lastMainMenuItemNumber = Instrument::mainMenu.back()->getNumber();
            if (chosenAction->match(lastMainMenuItemNumber))
                break;

            system("CLS");

            Instrument::display(Instrument::objectMenu);
            chosenObject = Instrument::getChoice(Instrument::objectMenu);
            string lastObjectMenuItemNumber = Instrument::objectMenu.back()->getNumber();

            if (chosenObject->getNumber() == lastObjectMenuItemNumber)
            {
                system("CLS");
                continue;
            }

            userChoice choice = toEnum(make_pair(chosenAction->getNumber(), chosenObject->getNumber()));

            switch (choice)
            {
            case userChoice::AddStation:
                //TODO: AddNewStation();
                break;
            case userChoice::AddRoute:
                //TODO: AddNewRoute();
                break;
            case userChoice::AddLine:
                //TODO: AddNewLine();
                break;
            case userChoice::RemoveStation:
                //TODO: RemoveExistingStation();
                break;
            case userChoice::RemoveRoute:
                //TODO: RemoveExistingRoute(fromStationNr, toStationNr);
                break;
            case userChoice::RemoveLine:
                instrument.removeLine();
                break;
            default:
                throw exception("Case unhandled! Implement missing logic!");
            }

        }
        catch (std::exception& e)
        {
            cout << e.what() << endl;
        }
    }

    return 0;
 }