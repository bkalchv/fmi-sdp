#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <stdexcept>
#include <sys/stat.h>
#include <fstream>
#include <string>


#include "Bus.h"
#include "BusHasher.h"
#include "Edge.h"
#include "EdgeHasher.h"
#include "Station.h"
#include "StationHasher.h"

using AdjacencyList = std::unordered_set<Edge, EdgeHasher>;
using Graph = std::unordered_map<Station, AdjacencyList, StationHasher>;
using Buses = std::unordered_set<Bus, BusHasher>;

static const char* STATIONS_DELIMITER = ",";
static const char* TIMETABLE_DELIMITER = " ";

class FileManager
{
private:
    std::string filenameStations;
    std::string filenameTimetable;

public:
    FileManager();
    FileManager(std::string, std::string);

    std::string getFilenameStations() const;
    std::string getFilenameTimetable() const;
    void setFilename(const std::string&, const std::string&);
    inline bool isValidTextFileNameFormat(const std::string&);
    inline bool fileExists(const std::string&);
    Graph readGraph();
    Buses readBuses();
    Station getStationByStationNumber(const Graph& graph, unsigned int stationNr);

    void addStation(const std::string&, const std::string&, const std::string&);
    void removeStation(const std::string& stationToRemove);
    void addRoute(const std::string&, const std::string&, const std::string&, const std::vector<Station>&);
    void removeRoute(const std::string&, const std::string&, const std::string&);
    void addLine(const std::string&);
    void removeLine(const std::string&);
private:
    std::pair<std::vector<std::vector<unsigned int>>, std::vector<std::vector<unsigned int>>> _getTimetables(unsigned int busNr);
    std::vector<Station> _stationsToVector(std::string& stations)
    {
        std::vector<Station> result;

        // do while condition?

        while (!stations.empty() && stations.find(STATIONS_DELIMITER) != std::string::npos)
        {
            std::string stationToken = stations.substr(0, stations.find(STATIONS_DELIMITER));

            unsigned int stationNr = stoul(stationToken.substr(0, stations.find(STATIONS_DELIMITER)));
            std::string stationName = stationToken.substr(stations.find('-') + 1);

            result.push_back(Station(stationNr, stationName));

            stations.erase(0, stations.find(STATIONS_DELIMITER) + 1);
        }

        unsigned int stationNr = stoul(stations.substr(0, stations.find('-')));
        std::string stationName = stations.substr(stations.find('-') + 1);
        result.push_back(Station(stationNr, stationName));

        return result;
    }
    std::vector<unsigned int> _timeDifferencesToVector(std::string& timetable)
    {
        std::vector<unsigned int> result;

        // do while condition?

        while (!timetable.empty() && timetable.find(TIMETABLE_DELIMITER) != std::string::npos)
        {
            std::string timeToken = timetable.substr(0, timetable.find(TIMETABLE_DELIMITER));

            unsigned int time = stoi(timeToken.substr(0, timetable.find(TIMETABLE_DELIMITER)));

            result.push_back(time);

            timetable.erase(0, timetable.find(TIMETABLE_DELIMITER) + 1);
        }

        unsigned int time = stoi(timetable);

        result.push_back(time);

        return result;
    }
    std::vector<unsigned int> _timesToVector(std::string& times) 
    {
        std::vector<unsigned int> result;

        // do while condition?

        while (!times.empty() && times.find(STATIONS_DELIMITER) != std::string::npos)
        {
            std::string timeToken = times.substr(0, times.find(STATIONS_DELIMITER));

            unsigned int currentTime = stoi(timeToken);

            result.push_back(currentTime);

            times.erase(0, times.find(STATIONS_DELIMITER) + 1);
        }
        unsigned int currentTime = stoi(times);
        result.push_back(currentTime);

        return result;
    }
    std::vector<unsigned int> _calculateTimesBasedOnTimeDifference(const std::vector<unsigned int>&, unsigned int);
    std::vector<std::vector<unsigned int>> _calculateTimesForAllStations(const std::vector<unsigned int>&, const std::vector<unsigned int>&);
    std::unordered_map<unsigned int, std::vector<unsigned int>> _getTimeDifferenceMap();
    bool stationExists(const std::string&);
    bool lineExists(const std::string&);
    bool isLastStringInLine(const std::string&, const std::string&);
    std::vector<Station> _reverseVector(const std::vector<Station>&);
    const Bus& _accessBusFromBusesSet(const std::unordered_set<Bus,BusHasher>&, unsigned int, const std::string&);
};