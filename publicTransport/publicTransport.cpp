#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <sstream>
#include <fstream>

#include "Bus.h"
#include "BusHasher.h"
#include "Edge.h"
#include "Station.h"
#include "StationHasher.h"
#include "Instrument.h"

using namespace std;

using AdjacencyList = unordered_set<Edge, EdgeHasher>;
using Graph = unordered_map<Station, AdjacencyList, StationHasher>;
using Buses = unordered_set<Bus, BusHasher>;

void printGraph(const Graph& graph)
{
    for (const pair<Station, AdjacencyList>& p : graph)
    {
        p.first.print();

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

Edge getEdgeFromAdjacencyListByStationNumber(const AdjacencyList& adj, int number)
{
    for (const Edge& edge : adj)
    {
        if (edge.toStation.s_nr == number)
        {
            return edge;
        }
    }
}

Station getStationByStationNumber(const Graph& graph, unsigned int stationNr)
{
    for (const std::pair<Station, AdjacencyList>& p : graph)
    {
        if (p.first.s_nr == stationNr)
        {
            return p.first;
        }
    }

    throw std::exception("Couldn't get station by station number");
}

void getAllPossiblePathsRec(const Graph& graph, unsigned int fromStationNr, unsigned int toStationNr, vector<bool>& visited, const unordered_map<unsigned int, unsigned int>& stationNumberToIndex, vector<unsigned int>& path, unsigned int& pathIndex, std::vector<std::vector<int>>& routes)
{
    visited[stationNumberToIndex.at(fromStationNr)] = true;
    path[pathIndex] = fromStationNr;
    pathIndex++;

    if (fromStationNr == toStationNr)
    {
        std::vector<int> route;
        for (size_t i{0}; i < pathIndex ;++i)
        {
            route.push_back(path[i]);
        }
        routes.push_back(route);
    }
    else
    {
            for (AdjacencyList::const_iterator it = graph.at(getStationByStationNumber(graph, fromStationNr)).begin(); it != graph.at(getStationByStationNumber(graph, fromStationNr)).end(); ++it)
            {
                unsigned int adjacentStationNr = it->toStation.s_nr;
                if (!visited[stationNumberToIndex.at(adjacentStationNr)])
                {
                    getAllPossiblePathsRec(graph, adjacentStationNr, toStationNr, visited, stationNumberToIndex, path, pathIndex, routes);
                }
            }
    }

    --pathIndex;
    visited[stationNumberToIndex.at(fromStationNr)] = false;
}

std::vector<std::vector<int>> getAllPossiblePaths(const Graph& graph, unsigned int fromStationNr, unsigned int toStationNr)
{
    Station fromStation = getStationByStationNumber(graph, fromStationNr);
    if (graph.find(fromStation) == graph.end())
    {
        std::cout << fromStationNr << std::endl;
        throw std::exception("No station with such number found in graph! [fromStationNr]");
    }

    Station toStation = getStationByStationNumber(graph, toStationNr);
    if (graph.find(getStationByStationNumber(graph, toStationNr)) == graph.end())
    {
        throw std::exception("No station with such number found in graph! [toStationNr]");
    }

    vector<bool> visited;
    visited.resize(graph.size());

    for (size_t i{ 0 }; i < visited.size(); ++i)
    {
        visited[i] = false;
    }

    vector<unsigned int> path;
    path.resize(graph.size());
    unsigned int pathIndex = 0;

    std::vector<std::vector<int>> result;

    std::unordered_map<unsigned int, unsigned int> stationNumberToIndex;

    size_t i{ 0 };
    for (const std::pair<Station, AdjacencyList>& p : graph)
    {
        stationNumberToIndex[p.first.s_nr] = i;
        ++i;
    }

    getAllPossiblePathsRec(graph, fromStationNr, toStationNr, visited, stationNumberToIndex, path, pathIndex, result);

    return result;
}

int calculateWeightOfPath(const Graph& graph, const std::vector<int>& path)
{
    int accumulatedWeight{0};

    for (size_t i{0}; i < path.size() - 1; ++i)
    {
        int currentStationNr = path[i];
        int nextStationNr = path[i + 1];
        Edge next = getEdgeFromAdjacencyListByStationNumber(graph.at(getStationByStationNumber(graph, currentStationNr)), nextStationNr);
        accumulatedWeight += next.weight;
    }

    return accumulatedWeight;
}

void displayInformation(const Graph& graph, unsigned int timeInMinutes, unsigned int stationNr)
{
    Station station = getStationByStationNumber(graph, stationNr);
    station.displayInformation(timeInMinutes);
}

using choicePair = pair<const unsigned int&, const unsigned int&>;

int main()
{
    try 
    {
        string filenameStations, filenameTimetable;
        filenameStations = "Stations.txt";
        filenameTimetable = "Timetable.txt";
        FileManager fmanager = FileManager(filenameStations, filenameTimetable);
        Graph g = fmanager.readGraph();
        printGraph(g);
        displayInformation(g, 307, 8);
        vector<vector<int>> allPossibleRoutes = getAllPossiblePaths(g, 8, 14);

        size_t i{ 1 };
        for (vector<int> route : allPossibleRoutes)
        {   
            std::cout << "Route [" << i << "]: ";
            for (int a : route)
            {
                std::cout << a << " ";
            }
            std::cout << "costs " << calculateWeightOfPath(g, route) <<  std::endl;
            ++i;
        }
        std::cout << std::endl;

    }
    catch (const std::exception& ex) 
    {
        cerr << ex.what() << endl;
    }

    const MenuItem* chosenAction;
    const MenuItem* chosenObject;
    
    string      stationsFilename    = "Stations.txt";
    string      timetableFilename   = "Timetable.txt";
    FileManager fmanager = FileManager(stationsFilename, timetableFilename);
    Instrument  instrument = Instrument(fmanager);

    while (true)
    {
        try
        {
            instrument.display(instrument.mainMenu);
            chosenAction = instrument.getChoice(instrument.mainMenu);
            string lastMainMenuItemNumber = instrument.mainMenu.back()->getNumber();
            if (chosenAction->match(lastMainMenuItemNumber))
                break;

            system("CLS");

            instrument.display(instrument.objectMenu);
            chosenObject = instrument.getChoice(instrument.objectMenu);
            string lastObjectMenuItemNumber = instrument.objectMenu.back()->getNumber();
            if (chosenObject->match(lastObjectMenuItemNumber))
            {
                system("CLS");
                continue;
            }

            unsigned int choice = stoi(chosenAction->getNumber()) * 10 + stoi(chosenObject->getNumber());

            switch (choice)
            {
            case 11:
                instrument.addStation();
                break;
            case 12:
                instrument.addRoute();
                break;
            case 13:
                instrument.addLine();
                break;
            case 21:
                instrument.removeStation();
                break;
            case 22:
                instrument.removeRoute();
                break;
            case 23:
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