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

#include "Edge.h"
#include "EdgeHasher.h"

using AdjacencyList = std::unordered_set<Edge, EdgeHasher>;
using Graph = std::unordered_map<unsigned int, AdjacencyList>;

static const char* STATIONS_DELIMITER = ",";

class FileManager
{
private:
    std::string filename;

public:
    FileManager();
    FileManager(std::string filename);

    std::string getFilename();
    void setFilename(const std::string& filename);
    inline bool isValidTextFileNameFormat(const std::string&);
    inline bool fileExists(const std::string&);
    Graph readGraph();

    bool lineExists(const std::string&);
    void removeLine(const std::string&);
private:
    std::vector<Station> _stationsToVector(std::string& stations)
    {
        std::vector<Station> result;

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
};