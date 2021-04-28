#include "FileManager.h"
#include <iostream>

const std::string TEXTFILE_EXTENSION            = ".txt";
const std::string DEFAULT_FILEMANAGER_STATIONS_FILENAME  = "Stations.txt";
const std::string DEFAULT_FILEMANAGER_TIMETABLE_FILENAME = "Timetable.txt";
const std::string NO_PREVIOUS_INPUT = "-";
const char LINE_NUMBER_ROUTE_DELIMITER = ':';
const char TIMETABLE_STARTING_MINUTES_OPENING_BRACKET = '[';
const char TIMETABLE_STARTING_MINUTES_CLOSING_BRACKET = ']';

FileManager::FileManager()
{
    this->setFilename(DEFAULT_FILEMANAGER_STATIONS_FILENAME, DEFAULT_FILEMANAGER_TIMETABLE_FILENAME);
}

FileManager::FileManager(std::string filenameStations, std::string filenameTimetable)
{
    this->setFilename(filenameStations, filenameTimetable);
}

std::string FileManager::getFilenameStations() const
{
    return this->filenameStations;
}

std::string FileManager::getFilenameTimetable() const
{
    return this->filenameTimetable;
}

void FileManager::setFilename(const std::string& filenameStations, const std::string& filenameTimetable)
{
    if (!isValidTextFileNameFormat(filenameStations) || !(isValidTextFileNameFormat(filenameTimetable)))
    {
        throw std::invalid_argument("Invalid file name format! (expected: .txt)");
    }

    if (!fileExists(filenameStations) || !fileExists(filenameTimetable))
    {
        throw std::runtime_error("Couldn't open the file with the given file name! (Check wether it is in the same folder)");
    }

    this->filenameStations.resize(filenameStations.size());
    for (size_t i{0}; i < filenameStations.size(); ++i)
    {
        this->filenameStations[i] = filenameStations[i];
    }

    this->filenameTimetable.resize(filenameTimetable.size());
    for (size_t i{ 0 }; i < filenameTimetable.size(); ++i)
    {
        this->filenameTimetable[i] = filenameTimetable[i];
    }
}

inline bool FileManager::isValidTextFileNameFormat(const std::string& filename)
{
    return  filename.size() > 4 && filename.substr(filename.size() - 4) == TEXTFILE_EXTENSION;
}

inline bool FileManager::fileExists(const std::string& filename)
{
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

Graph FileManager::readGraph()
{
    Buses buses = readBuses();
    std::unordered_map<unsigned int, std::vector<unsigned int>> timeDifferences = _getTimeDifferenceMap();

    std::ifstream inputFileStations(this->filenameStations); 

    if (inputFileStations)
    {
        Graph graph;

        std::string lineStations;

        while (getline(inputFileStations, lineStations))
        {
            std::istringstream iss(lineStations); // descriptive much?

            unsigned int busNr;
            char delimiter;
            std::string stations;

            if (iss >> busNr >> delimiter && delimiter == LINE_NUMBER_ROUTE_DELIMITER)
            {
                stations = lineStations.substr(lineStations.find(LINE_NUMBER_ROUTE_DELIMITER) + 1);
                if (stations.size() > 0)
                {
                    std::vector<unsigned int> currentTimeDifferences            = timeDifferences[busNr];
                    std::vector<Station> vectorStations                         = _stationsToVector(stations);
                    std::pair<std::vector<std::vector<unsigned int>>, std::vector<std::vector<unsigned int>>> pair = _getTimetables(busNr);
                    std::vector<std::vector<unsigned int>> timetable            = pair.first;
                    std::vector<std::vector<unsigned int>> timetableReverse     = pair.second; 
                    Bus currentBus                                              = _accessBusFromBusesSet(buses, busNr, std::to_string(busNr));  
                    Bus currentBusReverse                                       = _accessBusFromBusesSet(buses, busNr, std::to_string(busNr) + "reverse");
                   
                    if (vectorStations.size() - 1 == currentTimeDifferences.size())
                    {
                        for (size_t i{ 0 }; i < vectorStations.size(); ++i)
                        {
                            Station currentStation = vectorStations[i];
                            busArrivalMinutes nextStationArrivalMinutes = timetable[i];
                            currentStation.s_schedule[currentBus] = nextStationArrivalMinutes;
                            busArrivalMinutes nextStationArrivalMinutesReverse = timetableReverse[i];
                            currentStation.s_schedule[currentBusReverse] = nextStationArrivalMinutesReverse;

                            if (graph.find(currentStation) != graph.end())
                            {
                                Station copyStation = getStationByStationNumber(graph, currentStation.s_nr);
                                AdjacencyList copyAdjacencyList = graph[copyStation];
                                graph.erase(currentStation);
                                copyStation.addSchedule(currentBus, nextStationArrivalMinutes);
                                copyStation.addSchedule(currentBusReverse, nextStationArrivalMinutesReverse);
                                graph[copyStation] = copyAdjacencyList;
                            }
                            else
                            {
                                graph[currentStation];
                            }
                            
                            if (i == 0)
                            {
                                Station nextStation                             = Station(vectorStations[i + 1]);
                                busArrivalMinutes nextStationArrivalMinutes     = timetable[i + 1];
                                nextStation.s_schedule[currentBus]              = nextStationArrivalMinutes;
                                busArrivalMinutes nextStopArrivalMinutesReverse = timetableReverse[i + 1];
                                nextStation.s_schedule[currentBusReverse]       = nextStopArrivalMinutesReverse;
                                graph[currentStation].insert(Edge(nextStation, currentTimeDifferences[i]));
                            }
                            else if (i == vectorStations.size() - 1)
                            {
                                Station prevStation                             = Station(vectorStations[i - 1]);
                                busArrivalMinutes prevStationArrivalMinutes     = timetable[i - 1];
                                prevStation.s_schedule[currentBus]              = prevStationArrivalMinutes;
                                busArrivalMinutes prevStopArrivalMinutesReverse = timetableReverse[i - 1];
                                prevStation.s_schedule[currentBusReverse]       = prevStopArrivalMinutesReverse;
                                graph[currentStation].insert(Edge(prevStation, currentTimeDifferences[i - 1]));
                            }
                            else
                            {
                                Station prevStation                             = Station(vectorStations[i - 1]);
                                busArrivalMinutes prevStopArrivalMinutes        = timetable[i - 1];
                                prevStation.s_schedule[currentBus]              = prevStopArrivalMinutes;
                                busArrivalMinutes prevStopArrivalMinutesReverse = timetableReverse[i - 1];
                                prevStation.s_schedule[currentBusReverse]       = prevStopArrivalMinutesReverse;
                                graph[currentStation].insert(Edge(prevStation, currentTimeDifferences[i - 1]));

                                Station nextStation                             = Station(vectorStations[i + 1]);
                                busArrivalMinutes nextStopArrivalMinutes        = timetable[i + 1];
                                nextStation.s_schedule[currentBus]              = nextStopArrivalMinutes;
                                busArrivalMinutes nextStopArrivalMinutesReverse = timetableReverse[i + 1];
                                nextStation.s_schedule[currentBusReverse]       = nextStopArrivalMinutesReverse;
                                graph[currentStation].insert(Edge(nextStation, currentTimeDifferences[i]));
                            }
                        }
                    }
                    else
                    {
                        inputFileStations.close();
                        throw std::exception("There's a difference between the amount of Stations(in Stations.txt) and the amount of Edges(in Timetable.txt)");
                    }
                }
            }
        }
        inputFileStations.close();
        return graph;
    }
    else
    {
        inputFileStations.close();
        throw std::exception("Couldn't read file!");
    }
}

Buses FileManager::readBuses()
{
    std::ifstream inputFileStations(this->filenameStations);

    if (inputFileStations)
    {
        Buses buses;

        std::string lineStations;

        while (getline(inputFileStations, lineStations))
        {
            std::istringstream iss(lineStations); // descriptive much?

            int busNr;
            char delimiter;
            std::string stations;

            if (iss >> busNr >> delimiter && delimiter == LINE_NUMBER_ROUTE_DELIMITER)
            {
                stations = lineStations.substr(lineStations.find(LINE_NUMBER_ROUTE_DELIMITER) + 1);
                if (stations.size() > 0)
                {
                    std::vector<Station> vectorStations = _stationsToVector(stations);
                    std::vector<Station> reverseVectorStations = _reverseVector(vectorStations);

                    for (size_t i{0}; i < 2; ++i)
                    {
                        std::string busName;
                        if (i == 0)
                        {
                            busName = std::to_string(busNr);
                        }
                        else if (i == 1)
                        {
                            busName = std::to_string(busNr).append("reverse");
                        }

                        Bus newBus(busNr, busName);

                        if (i == 0)
                        {
                            for (const Station& st : reverseVectorStations)
                            {
                                newBus.b_route.push_front(st.s_nr);
                            }
                        }
                        else if (i == 1)
                        {
                            for (const Station& st : vectorStations)
                            {
                                newBus.b_route.push_front(st.s_nr);
                            }
                        }

                        buses.insert(newBus);
                    }
                }
            }
        }
        inputFileStations.close();

        return buses;
    }
    else
    {
        inputFileStations.close();
        throw std::exception("");
    }
}

Station FileManager::getStationByStationNumber(const Graph& graph, unsigned int stationNr)
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

void FileManager::addStation(const std::string& stationToAdd, const std::string& stationToAddLineNumber, const std::string& stationToAddPrevious) // What if there's no previous one?
{
    if (lineExists(stationToAddLineNumber))
    {
        std::ofstream fileOutput;
        std::ifstream fileInput;

        fileOutput.open("Temp.txt");
        fileInput.open(this->filenameStations);

        if (fileInput.is_open())
        {
            std::string line;

            while (getline(fileInput, line))
            {

                if (line.substr(0, stationToAddLineNumber.size()) == stationToAddLineNumber)
                {
                    if (stationToAddPrevious == NO_PREVIOUS_INPUT)
                    {
                        line.insert(line.find(LINE_NUMBER_ROUTE_DELIMITER) + 2, stationToAdd + ", ");
                        fileOutput << line << "\n";
                    }
                    else
                    {
                        size_t previousStationIndex = line.find(stationToAddPrevious);
                        if (previousStationIndex != -1)
                        {
                            if (isLastStringInLine(line, stationToAddPrevious))
                            {
                                line.insert(previousStationIndex + stationToAddPrevious.size(), ", " + stationToAdd);
                            }
                            else
                            {
                                line.insert(previousStationIndex + stationToAddPrevious.size() + 1, " " + stationToAdd + ",");
                            }

                            fileOutput << line << "\n";
                        }
                        else
                        {
                            std::cout << "Previous station doesn't exist in that line!" << std::endl;
                        }
                    }
                }
                else
                {
                    fileOutput << line << "\n";
                }
            }
        }
        else
        {
            fileInput.close();
            fileOutput.close();
            throw std::exception("Couldn't open file!");
        }
        fileInput.close();
        fileOutput.close();

        if (remove(this->filenameStations.c_str()) != 0)
        {
            throw std::exception("Removing of temporary file unsuccessful!");
        }

        if (rename("Temp.txt", this->filenameStations.c_str()) != 0)
        {
            throw std::exception("Renaming unsuccessful!");
        }

        std::cout << "Station " << stationToAdd << " successfully added!" << std::endl;
    }
    else
    {
        std::cout << "Station " << stationToAdd << " couldn't be added, because Line " << stationToAddLineNumber << " doesn't exist!" << std::endl;
    }
}

void FileManager::removeStation(const std::string& stationToRemove)
{
    std::ifstream inputFile(this->filenameStations);
    std::ofstream temp;

    std::cout << stationToRemove << std::endl;

    if (inputFile && stationExists(stationToRemove))
    {
        temp.open("Temp.txt");
        std::string line;

        while (getline(inputFile, line) && !line.empty())
        {
            size_t stationToRemoveIndex = line.find(stationToRemove);
            if (stationToRemoveIndex != -1)
            {
                line.erase(stationToRemoveIndex, stationToRemove.size() + 2);
            }           
             
            temp << line << '\n';
        }

        inputFile.close();
        temp.close();

        if (remove(this->filenameStations.c_str()) != 0)
        {
            throw std::exception("Removing of temporary file unsuccessful!");
        }

        if (rename("Temp.txt", this->filenameStations.c_str()) != 0)
        {
            throw std::exception("Renaming unsuccessful!");
        }

        std::cout << "Station " << stationToRemove << " from " << this->filenameStations << " successfully removed!" << std::endl;
    }
    else
    {
        inputFile.close();
        throw std::exception("Couldn't open file!");
    }
}

void FileManager::addRoute(const std::string& lineNumber, const std::string& fromStation, const std::string& toStation, const std::vector<Station>& stationsToAdd)
{
    if (lineExists(lineNumber))
    {
        this->removeRoute(lineNumber, fromStation, toStation);

        std::ofstream fileOutput;
        std::ifstream fileInput;

        fileOutput.open("Temp.txt");
        fileInput.open(this->filenameStations);

        if (fileInput.is_open())
        {
            std::string line;

            while (getline(fileInput, line))
            {
                if (line.substr(0, lineNumber.size()) == lineNumber)
                {
                    size_t fromStationIndex, toStationIndex;
                    fromStationIndex = line.find(fromStation);
                    toStationIndex = line.find(toStation);
                    if (line.find(fromStation) != -1 && line.find(toStation) != -1 && fromStationIndex < toStationIndex)
                    {
                        std::string routeToAdd;
                        for (const Station& station : stationsToAdd)
                        {
                            if (station == stationsToAdd.front())
                            {
                                routeToAdd.append(" ");
                            }
                            routeToAdd.append(station.toString()).append(", ");
                        }

                        line.insert(fromStationIndex + fromStation.size() + 1, routeToAdd);

                        fileOutput << line << "\n";
                    }
                    else
                    {
                        std::cout << "Route from " << fromStation << " to " << toStation << " doesn't exist!" << std::endl;
                        fileOutput << line << "\n";
                    }
                }
                else
                {
                    fileOutput << line << "\n";
                }
            }
        }
        else
        {
            fileInput.close();
            fileOutput.close();
            throw std::exception("Couldn't open file!");
        }
        fileInput.close();
        fileOutput.close();

        if (remove(this->filenameStations.c_str()) != 0)
        {
            throw std::exception("Removing of temporary file unsuccessful!");
        }

        if (rename("Temp.txt", this->filenameStations.c_str()) != 0)
        {
            throw std::exception("Renaming unsuccessful!");
        }

        std::cout << "Route " << fromStation << " to " << toStation << " successfully added to Line" << lineNumber;
    }
    else
    {
    std::cout << "Route " << fromStation <<  " to " << toStation <<" couldn't be added to Line" << lineNumber << ", because Line " << lineNumber << " doesn't exist!" << std::endl;
    }
}

void FileManager::removeRoute(const std::string& lineNumber, const std::string& fromStation, const std::string& toStation)
{
    if (lineExists(lineNumber))
    {
        std::ofstream fileOutput;
        std::ifstream fileInput;

        fileOutput.open("Temp.txt");
        fileInput.open(this->filenameStations);

        if (fileInput.is_open())
        {
            std::string line;

            while (getline(fileInput, line))
            {
                if (line.substr(0, lineNumber.size()) == lineNumber)
                {
                    size_t fromStationIndex, toStationIndex;
                    fromStationIndex = line.find(fromStation);
                    toStationIndex = line.find(toStation);
                    if (fromStationIndex != -1 && toStationIndex != -1 && fromStationIndex < toStationIndex)
                    {
                        line.erase(fromStationIndex + fromStation.size() + 1, (toStationIndex - (fromStationIndex + fromStation.size() + 1)));
                        fileOutput << line << "\n";
                    }
                    else
                    {
                        std::cout << "Route from " <<  fromStation << " to " << toStation << " doesn't exist!" << std::endl;
                        fileOutput << line << "\n";
                    }
                }
                else
                {
                    fileOutput << line << "\n";
                }
            }
        }
        else
        {
            fileInput.close();
            fileOutput.close();
            throw std::exception("Couldn't open file!");
        }
        fileInput.close();
        fileOutput.close();

        if (remove(this->filenameStations.c_str()) != 0)
        {
            throw std::exception("Removing of temporary file unsuccessful!");
        }

        if (rename("Temp.txt", this->filenameStations.c_str()) != 0)
        {
            throw std::exception("Renaming unsuccessful!");
        }

        std::cout << "Route from " << fromStation << " to " << toStation << " in Line " << lineNumber <<  " successfully removed!" << std::endl;
    }
    else
    {
        std::cout << "Route from " << fromStation << " to " << toStation <<  " couldn't be removed, because Line " << lineNumber << " doesn't exist!" << std::endl;
    }
}

void FileManager::addLine(const std::string& lineToAdd)
{
    std::ofstream fileOutput;
    std::ifstream fileInput;

    fileOutput.open("Temp.txt");
    fileInput.open(this->filenameStations);

    if (fileInput.is_open())
    {
        std::string line;

        while (getline(fileInput, line))
        {
            fileOutput << line << '\n';
        }

        fileOutput << lineToAdd;
    }
    else
    {   
        fileInput.close();
        fileOutput.close();
        throw std::exception("Couldn't open file!");
    }
    fileInput.close();
    fileOutput.close();

    if (remove(this->filenameStations.c_str()) != 0)
    {
        throw std::exception("Removing of temporary file unsuccessful!");
    }

    if (rename("Temp.txt", this->filenameStations.c_str()) != 0)
    {
        throw std::exception("Renaming unsuccessful!");
    }

    std::cout << "Line successfully added to " << this->filenameStations << std::endl;
}

void FileManager::removeLine(const std::string& lineNrToRemove)
{
    std::ifstream inputFile(this->filenameStations);
    std::ofstream temp;

    if (inputFile && lineExists(lineNrToRemove))
    {
        temp.open("Temp.txt");
        std::string line;

        while (getline(inputFile, line) && !line.empty())
        {
            if (line.substr(0, lineNrToRemove.size()) != lineNrToRemove)
            {
                temp << line << '\n';
            }
        }

        inputFile.close();
        temp.close();
        
        if (remove(this->filenameStations.c_str()) != 0) 
        {
            throw std::exception("Removing of temporary file unsuccessful!");
        }

        if (rename("Temp.txt", this->filenameStations.c_str()) != 0)
        {
            throw std::exception("Renaming unsuccessful!");
        }

        std::cout << "Line number " << lineNrToRemove << " from " << this->filenameStations <<  " successfully removed!" << std::endl;
    }
    else
    {
        inputFile.close();
        throw std::exception("Couldn't open file!");
    }
}

std::pair<std::vector<std::vector<unsigned int>>, std::vector<std::vector<unsigned int>>> FileManager::_getTimetables(unsigned int busNumber)
{
    std::pair<std::vector<std::vector<unsigned int>>, std::vector<std::vector<unsigned int>>> result;

    std::ifstream inputFileTimetable(this->filenameTimetable);

    if (inputFileTimetable)
    {
        std::string lineTimetable1;
        std::string lineTimetable2;

        while (getline(inputFileTimetable, lineTimetable1))
        {
            std::istringstream iss1(lineTimetable1); // descriptive much?

            int busNr;
            char delimiter;
            char timetableOpeningBracket;

            std::vector<std::vector<unsigned int>> timetableForLine1, timetableForLine2;


            if (iss1 >> busNr >> delimiter >> timetableOpeningBracket && busNr == busNumber && delimiter == LINE_NUMBER_ROUTE_DELIMITER &&  timetableOpeningBracket == TIMETABLE_STARTING_MINUTES_OPENING_BRACKET)
            {
                std::string times                                           = lineTimetable1.substr(lineTimetable1.find(TIMETABLE_STARTING_MINUTES_OPENING_BRACKET) + 1, lineTimetable1.find(TIMETABLE_STARTING_MINUTES_CLOSING_BRACKET) - lineTimetable1.find(TIMETABLE_STARTING_MINUTES_OPENING_BRACKET) + 1);
                std::vector<unsigned int> timesVector                       = _timesToVector(times);
                std::string weights                                         = lineTimetable1.substr(lineTimetable1.find(TIMETABLE_STARTING_MINUTES_CLOSING_BRACKET) + 2);
                std::vector<unsigned int> weightsVector                     = _timeDifferencesToVector(weights);
                timetableForLine1                                           = _calculateTimesForAllStations(timesVector, weightsVector);
                
                result.first                                                = timetableForLine1;

                getline(inputFileTimetable, lineTimetable2);
                std::istringstream iss2(lineTimetable2);

                if (iss2 >> timetableOpeningBracket && timetableOpeningBracket == TIMETABLE_STARTING_MINUTES_OPENING_BRACKET)
                {
                    std::string timesReverse = lineTimetable2.substr(lineTimetable2.find(TIMETABLE_STARTING_MINUTES_OPENING_BRACKET) + 1, lineTimetable2.find(TIMETABLE_STARTING_MINUTES_CLOSING_BRACKET) - lineTimetable2.find(TIMETABLE_STARTING_MINUTES_OPENING_BRACKET));
                    std::vector<unsigned int> timesVectorReverse = _timesToVector(timesReverse);                    
                    std::string weightsReverse = lineTimetable2.substr(lineTimetable2.find(TIMETABLE_STARTING_MINUTES_CLOSING_BRACKET) + 2);
                    std::vector<unsigned int> weightsVectorReverse = _timeDifferencesToVector(weightsReverse);
                    timetableForLine2 = _calculateTimesForAllStations(timesVectorReverse, weightsVectorReverse);

                    result.second = timetableForLine2;
                }
            }

        }

        inputFileTimetable.close();
        return result;
    }
    else
    {
        inputFileTimetable.close();
        throw std::exception("Couldn't read input file!");
    }
}

bool FileManager::lineExists(const std::string& lineNumberToCheck)
{
    std::ifstream inputFile(this->filenameStations);

    if (inputFile)
    {
        std::string line;

        while (getline(inputFile, line))
        {
            if (line.substr(0, lineNumberToCheck.size()) == lineNumberToCheck)
            {
                return true;
            }
        }
    }
    inputFile.close();
    std::cout << "Line " << lineNumberToCheck << " not found!" << std::endl;
    return false;
}

bool FileManager::isLastStringInLine(const std::string& line, const std::string& str)
{
    return (line.substr((line.size()-1) - (str.size()-1)) == str);
}

std::vector<Station> FileManager::_reverseVector(const std::vector<Station>& other)
{
    std::vector<Station> result;

    for (size_t i{other.size()}; i > 0; --i)
    {
        result.push_back(other[i - 1]);
    }

    return result;
}

const Bus& FileManager::_accessBusFromBusesSet(const std::unordered_set<Bus, BusHasher>& buses, unsigned int busNr, const std::string& busName)
{
    for (const Bus& b : buses)
    {
        if (b.b_nr == busNr && b.b_name == busName)
        {
            return b;
        }
    }
}

std::vector<unsigned int> FileManager::_calculateTimesBasedOnTimeDifference(const std::vector<unsigned int>& times, unsigned int timeDifference)
{
    std::vector<unsigned int> result;
    result.resize(times.size());

    for (size_t i{0}; i < result.size(); ++i)
    {
        result[i] = times[i] + timeDifference;
    }

    return result;
}

std::vector<std::vector<unsigned int>> FileManager::_calculateTimesForAllStations(const std::vector<unsigned int>& times, const std::vector<unsigned int>& timeDifferences)
{
    std::vector<std::vector<unsigned int>> result;
    result.resize(1 + timeDifferences.size());
    
    size_t i{ 0 };
    for (; i < timeDifferences.size(); ++i)
    {
        if (i == 0)
        {
            result[i] = times;
        }
        else
        {
            std::vector<unsigned int> temp = _calculateTimesBasedOnTimeDifference(result[i - 1], timeDifferences[i - 1]);
            result[i] = temp;
        }       
    }
    std::vector<unsigned int> temp = _calculateTimesBasedOnTimeDifference(result[i - 1], timeDifferences[i - 1]);
    result[i] = temp;

    return result;
}

std::unordered_map<unsigned int, std::vector<unsigned int>> FileManager::_getTimeDifferenceMap()
{
    std::unordered_map<unsigned int, std::vector<unsigned int>> result;
    
    std::ifstream inputFileTimetable(this->filenameTimetable);
    std::string lineTimetable;

    if (inputFileTimetable)
    {
        while (getline(inputFileTimetable, lineTimetable))
        {
            if (!lineTimetable.empty())
            {
                std::istringstream iss(lineTimetable); // descriptive much?
                unsigned int lineNr;
                char lineNrDelimiter;
                char timetableOpeningBracket;

                if (iss >> lineNr >> lineNrDelimiter >> timetableOpeningBracket && lineNrDelimiter == LINE_NUMBER_ROUTE_DELIMITER && timetableOpeningBracket == TIMETABLE_STARTING_MINUTES_OPENING_BRACKET)
                {
                    std::string weights = lineTimetable.substr(lineTimetable.find(TIMETABLE_STARTING_MINUTES_CLOSING_BRACKET) + 2);
                    std::vector<unsigned int> weightsVector = _timeDifferencesToVector(weights);
                    result[lineNr] = weightsVector;
                }
            }
        }
        inputFileTimetable.close();
    }
    else
    {
        inputFileTimetable.close();
        throw std::exception("");
    }

    return result;
}

bool FileManager::stationExists(const std::string& station)
{
    std::ifstream inputFile(this->filenameStations);

    if (inputFile)
    {
        std::string line;

        while (getline(inputFile, line))
        {
            if (line.find(station) != -1) 
            {
                return true;
            }
        }
    }
    inputFile.close();
    std::cout << "Station " << station << " not found!" << std::endl;
    return false;
}
