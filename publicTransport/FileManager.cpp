#include "FileManager.h"
#include <iostream>

const std::string TEXTFILE_EXTENSION            = ".txt";
const std::string DEFAULT_FILEMANAGER_FILENAME  = "PublicTransport.txt";

FileManager::FileManager()
{
    this->setFilename(DEFAULT_FILEMANAGER_FILENAME);
}

FileManager::FileManager(std::string filename)
{
    this->setFilename(filename);
}

std::string FileManager::getFilename()
{
    return this->filename;
}

void FileManager::setFilename(const std::string& filename)
{
    if (!isValidTextFileNameFormat(filename))
    {
        throw std::invalid_argument("Invalid file name format! (expected: .txt)");
    }

    if (!fileExists(filename))
    {
        throw std::runtime_error("Couldn't open the file with the given file name! (Check wether it is in the same folder)");
    }

    this->filename.resize(filename.size());
    for (size_t i{0}; i < filename.size(); ++i)
    {
        this->filename[i] = filename[i];
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
    std::ifstream inputFile (this->filename);
    
    if (inputFile)
    {
        Graph graph;
        std::string line;

        while (getline(inputFile, line))
        {
            std::istringstream iss(line); // descriptive much?

            int busNr;
            char delimiter;
            std::string stations;

            if (iss >> busNr >> delimiter && delimiter == ':')
            {
                stations = line.substr(line.find(':') + 1);
                if (stations.size() > 0)
                {
                    std::vector<Station> vectorStations = _stationsToVector(stations);
                    for (size_t i{ 0 }; i < vectorStations.size(); ++i)
                    {
                        Station currentStation = vectorStations[i];
                        graph[currentStation.s_nr];

                        if (i == 0)
                        {
                            Station nextStation = Station(vectorStations[i + 1]);
                            graph[currentStation.s_nr].insert(Edge(nextStation));
                        }
                        else if (i == vectorStations.size() - 1)
                        {
                            Station prevStation = Station(vectorStations[i - 1]);
                            graph[currentStation.s_nr].insert(Edge(prevStation));
                        }
                        else
                        {
                            Station prevStation = Station(vectorStations[i - 1]);
                            graph[currentStation.s_nr].insert(Edge(prevStation));

                            Station nextStation = Station(vectorStations[i + 1]);
                            graph[currentStation.s_nr].insert(Edge(nextStation));
                        }
                    }
                }
            }
        }
        return graph;
        inputFile.close();
    }
    else
    {
        throw std::exception("");
    }
}

bool FileManager::lineExists(const std::string& lineNumberToCheck)
{
    std::ifstream inputFile (this->filename);
    
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
    std::cout << "Not found!" << std::endl;
    return false;
}

void FileManager::addLine(const std::string& lineToAdd)
{
    std::ofstream fileOutput;
    std::ifstream fileInput;

    fileOutput.open("Temp.txt");
    fileInput.open(this->filename);

    if (fileInput.is_open())
    {
        std::string line;

        while (getline(fileInput, line))
        {
            fileOutput << line << '\n';
        }

        fileOutput << lineToAdd << '\n';


    }
    else
    {   
        fileInput.close();
        fileOutput.close();
        throw std::exception("Couldn't open file!");
    }
    fileInput.close();
    fileOutput.close();

    if (remove(this->filename.c_str()) != 0)
    {
        throw std::exception("Removing of temporary file unsuccessful!");
    }

    if (rename("Temp.txt", this->filename.c_str()) != 0)
    {
        throw std::exception("Renaming unsuccessful!");
    }

    std::cout << "Line successfully added to " << this->filename << std::endl;
}

void FileManager::removeLine(const std::string& lineNrToRemove)
{
    std::ifstream inputFile(this->filename);
    std::ofstream temp;

    if (inputFile && lineExists(lineNrToRemove))
    {
        temp.open("Temp.txt");
        std::string line;

        while (getline(inputFile, line))
        {
            if (line.substr(0, lineNrToRemove.size()) != lineNrToRemove)
            {
                temp << line << std::endl;
            }
        }

        inputFile.close();
        temp.close();
        
        if (remove(this->filename.c_str()) != 0) 
        {
            throw std::exception("Removing of temporary file unsuccessful!");
        }

        if (rename("Temp.txt", this->filename.c_str()) != 0)
        {
            throw std::exception("Renaming unsuccessful!");
        }

        std::cout << "Line number " << lineNrToRemove << " from " << this->filename <<  " successfully removed!" << std::endl;
    }
    else
    {
        inputFile.close();
        throw std::exception("Couldn't open file!");
    }
}
