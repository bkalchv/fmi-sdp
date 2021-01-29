#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <forward_list>
#include <list>
#include <sstream>
#include <istream>
#include <fstream>

using namespace std;

using busNr = unsigned int;

static const busNr          DEFAULT_BUS_NR = 0;
static const unsigned int   DEFAULT_STATION_NR = 0;
static const string         DEFAULT_STATION_NAME = "unknown-station";
static const char*          STATIONS_DELIMITER = ",";
static const size_t         DEFAULT_EDGE_WEIGHT = 0;

struct Bus
{
    busNr b_nr;
    forward_list<unsigned int> b_route; // fwd_list<bus_station_numbers>
    
    Bus() : b_nr{DEFAULT_BUS_NR} {}
    Bus(busNr _b_nr) : b_nr{_b_nr} {}
};

using busArrivalMinutes = list<unsigned int>;
using schedule          = unordered_map<busNr, busArrivalMinutes>;

struct Station 
{
    unsigned int    s_nr;
    string          s_name;
    list<Bus>       s_busesList; // might be better to only have a schedule 
    schedule        s_schedule;
    
    Station() : s_nr{ DEFAULT_STATION_NR }, s_name{ DEFAULT_STATION_NAME } {}
    Station(unsigned int _s_nr, string _s_name) : s_nr{ _s_nr }, s_name{_s_name} {}

    bool operator==(const Station& otherStation) const
    {
        return (s_nr == otherStation.s_nr && s_name == otherStation.s_name);
    }

    void print() const
    {
        cout << "[" << s_nr << ", " << s_name << "]";
    }
};

struct StationHasher
{
    size_t operator()(const Station& s) const
    {
        return hash<unsigned int>()(s.s_nr);
    }
};

struct customStationComparator 
{
    bool operator()(const Station& f_s, const Station& s_s) const
    {
        return f_s.s_nr < s_s.s_nr;
    }
};

using AdjacencyList = list< pair<Station, size_t> >;
unordered_map<unsigned int, AdjacencyList> Graph;

const Station& findStation(unsigned int stationNr, const AdjacencyList& adjacencyList)
{
    for (const pair<Station, size_t>& st : adjacencyList)
    {
        if (st.first.s_nr == stationNr)
        {
            return st.first;
        }
    }

    throw exception("Station not found in Adjacency list!");
}

int findStationIndex(unsigned int stationNr, const AdjacencyList& adjacencyList)
{
    int result = 0;
   
    for (const pair<Station, size_t>& st : adjacencyList)
    {
        if (st.first.s_nr == stationNr)
        {
            return result;
        }
        ++result;
    }

    return -1;
}

void addStationToAdjacencyList(unsigned int s_nr, const Station& st)
{
    if (findStationIndex(st.s_nr, Graph[s_nr]) == -1)
    {
        Graph[s_nr].push_back(pair<Station, size_t>(Station(st), DEFAULT_EDGE_WEIGHT));
    }
}

//set<Station, customStationComparator> stationsSet;
list<Station> stationsList;

void addStationToList(const Station& stationToAdd)
{
    for (const Station& currentStation : stationsList)
    {
        if (stationToAdd == currentStation)
        {
            return;
        }
    }
    stationsList.push_back(Station(stationToAdd));
}

//const Station& findStationFromListByNumber(unsigned int stationNr)
//{
//    for (Station& currentStation : stationsList)
//    {
//        if (currentStation.s_nr == stationNr)
//        {
//            return currentStation;
//        }
//    }
//}

std::vector<Station> stationsToVector(string& stations)
{
    vector<Station> result;

    while (!stations.empty() && stations.find(',') != string::npos)
    {
        string stationToken = stations.substr(0, stations.find(','));

        unsigned int stationNr = stoul(stationToken.substr(0, stations.find('-')));
        string stationName = stationToken.substr(stations.find('-') + 1);

        result.push_back(Station(stationNr, stationName));

        stations.erase(0, stations.find(',') + 1);
    }

    unsigned int stationNr = stoul(stations.substr(0, stations.find('-')));
    string stationName = stations.substr(stations.find('-') + 1);

    result.push_back(Station(stationNr, stationName));

    return result;
}

void readGraph(const string& filename)
{   
    ifstream inFile(filename);
    if (inFile)
    {
        string line;
        while (getline(inFile, line))
        {
            istringstream   iss(line);
            
            int busNr;
            char delimiter;
            string stations;

            if (iss >> busNr >> delimiter && delimiter == ':')
            {
                stations = line.substr(line.find(':') + 1);
                if (stations.size() > 0)
                {   
                    std::vector<Station> vectorStations = stationsToVector(stations);
                    for (size_t i{0}; i < vectorStations.size(); ++i)
                    {
                        Station currentStation = vectorStations[i];
                        Graph[currentStation.s_nr];

                        if (i == 0)
                        {
                            Station nextStation = Station(vectorStations[i + 1]);
                            addStationToAdjacencyList(currentStation.s_nr, nextStation);
                        }
                        else if (i == vectorStations.size() - 1)
                        {
                            Station prevStation = Station(vectorStations[i- 1]);
                            addStationToAdjacencyList(currentStation.s_nr, prevStation);
                        }
                        else
                        {
                            Station nextStation = Station(vectorStations[i + 1]);
                            addStationToAdjacencyList(currentStation.s_nr, nextStation);

                            Station prevStation = Station(vectorStations[i - 1]);
                            addStationToAdjacencyList(currentStation.s_nr, prevStation);
                        }

                        addStationToList(Station(currentStation));
                        //stationsSet.insert(currentStation);
                    }
                }       
            }
        }
    }
    else
    {
        inFile.close();
        throw exception("File could not be read!");
    }
}

void printAdjacencyLists()
{
    for (const pair<unsigned int, AdjacencyList>& p : Graph)
    {
        cout << "[" << p.first << "]";

        if (!p.second.empty())
        {
            cout << ": ";
            for (AdjacencyList::const_iterator it = p.second.begin(); it != p.second.end(); ++it)
            {
                Station adjacentStation = it->first;
                cout << "->";
                adjacentStation.print();
                cout << "(" << it->second << ")";
            }
        }

        cout << endl;
    }
}

int main()
{
    readGraph("Text.txt");
    printAdjacencyLists(); cout << endl;
    //printAllStations();
 }