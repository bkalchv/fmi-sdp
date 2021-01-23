#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <stdexcept>

using namespace std;

const static string NUCLEOID_BASES      = {'A', 'C', 'T', 'G'};
const static string TEXTFILE_EXTENSION  = ".txt";

using CodonToAAMap = unordered_map<string, char>;
using ProteinsMap = unordered_map<size_t, string>;

string readDna(const string& dnaFilename)
{
    string dnaSequence;
    fstream inFile(dnaFilename);
    if (inFile)
    {   
        string line;
        getline(inFile, line);

        istringstream iss(line);

        string fileDna;

        if (iss >> fileDna)
        {
            for (const char& c : fileDna)
            {   
                if (NUCLEOID_BASES.find(c) != string::npos)
                    dnaSequence.push_back(c);
                else
                {
                    dnaSequence.clear();
                    inFile.close();
                    throw exception("DNA file contains invalid DNA sequence!\nNOTE: DNA consists of As, Cs, Ts and Gs only!");
                }
            }
        }
    }
    else
    {
        inFile.close();
        throw exception("DNA file could not be read!");
    }
    inFile.close();
    return dnaSequence;
}

CodonToAAMap readCodonToAAMap(const string& filenameCodonToAA)
{
    CodonToAAMap codonToAminoacids;
    fstream inFile(filenameCodonToAA);
    if (inFile)
    {
        string line;

        while (getline(inFile, line))
        {
            istringstream iss(line);

            string  fileDnac;
            char    fileAA;

            if (iss >> fileDnac >> fileAA)
            {
                codonToAminoacids[fileDnac] = fileAA;
            }
        }
    }
    else
    {
        inFile.close();
        throw exception("Codon to aminoacids file could not be read!");
    }
    inFile.close();
    return codonToAminoacids;
}

void printCodonToAAMap(const CodonToAAMap& map)
{
    for (const std::pair<string, char>& p : map)
    {
        cout << "[" << p.first << "] - " << p.second << endl;
    }
}

string getFirstTriplet(const string& protein)
{
    return protein.substr(0, 3);
}

string getLastTriplet(const string& protein)
{
    return protein.substr(protein.size() - 3);
}

bool isStartCodone(const string& codon, const CodonToAAMap& dnacToAAMap)
{
    return dnacToAAMap.find(codon) != dnacToAAMap.end() && dnacToAAMap.at(codon) != '*';
}

bool isStopCodone(const string& codon, const CodonToAAMap& dnacToAAMap)
{
    return dnacToAAMap.find(codon) != dnacToAAMap.end() && dnacToAAMap.at(codon) == '*';
}

bool isValidProteinSequence(const string& proteinSeq, const CodonToAAMap& dnacToAAMap)
{
    string firstTriplet = getFirstTriplet(proteinSeq);
    string lastTriplet = getLastTriplet(proteinSeq);

    if (proteinSeq.size() % 3 == 0 &&  isStartCodone(firstTriplet, dnacToAAMap) && isStopCodone(lastTriplet, dnacToAAMap))
    {
        return true;
    }

    return false;
}

void printInvalidProteinMessage(const string& proteinsFilename, size_t proteinId)
{
    cout << "Invalid protein sequence in " << proteinsFilename << "! Protein with ID: " << proteinId << " not inserted in map." << endl;
}

ProteinsMap readProteins(const string& proteinsFilename, const CodonToAAMap& codonToAAMap)
{
    ProteinsMap proteins;
    fstream inFile(proteinsFilename);
    if (inFile)
    {
        string line;

        while (getline(inFile, line))
        {
            istringstream iss(line);

            size_t  proteinId;
            string  protein;

            if (iss >> proteinId >> protein)
            {
                if (isValidProteinSequence(protein, codonToAAMap))
                {
                    proteins[proteinId] = protein;
                }
                else
                {
                    printInvalidProteinMessage(proteinsFilename, proteinId);
                }
            }
        }
    }
    else
    {
        inFile.close();
        throw exception("Proteins file could not be read!");
    }
    inFile.close();
    return proteins;
}

void printProteins(const ProteinsMap& map)
{
    for (const std::pair<size_t, string>& p : map)
    {
        cout << p.first << " " << p.second << endl;
    }
}

bool proteinMapContainsId(const ProteinsMap& map, size_t id)
{
    return map.find(id) != map.end();
}

string getProteinById(const ProteinsMap& map, size_t id)
{
    return map.at(id);
}

int dnaContainsProtein(const string& dna, const string& protein)
{
    string proteinStartCodone   = getFirstTriplet(protein);
    string proteinStopCodone    = getLastTriplet(protein);

    // TODO : iterate through the whole dna, dont stop at first appearance of the startCodone

    bool dnaContainsProtein = false;
    size_t startCodonePos = dna.find(proteinStartCodone);

    while (startCodonePos != string::npos)
    {
        if (dna.substr(startCodonePos, protein.size()) == protein)
        {
            dnaContainsProtein = true;
            return startCodonePos;
        }

        startCodonePos = dna.find(proteinStartCodone, startCodonePos + 1);
    }
   
    return -1;
}

string getCorrespondingAASequence(const string& protein, const CodonToAAMap& dnacToAAMap)
{
    string result;

    size_t i = 0;
    while (i < protein.size() - 2)
    {
        string currentCodone = protein.substr(i, 3);
        char correspondingAA = dnacToAAMap.at(currentCodone);
        if (correspondingAA == '*')
        {
            break;
        }
        result.push_back(correspondingAA);
        i += 3;
    }

    return result;
}

bool isValidTextFileNameInputFormat(const string& filename)
{
    return  filename.size() > 4 && filename.substr(filename.size() - 4) == TEXTFILE_EXTENSION;
}

void printMightBeInDifferentFolderMessage(const string& filename)
{
    cout << "* Check whether " << filename << " is in the same folder! *" << endl;
    cout << "If not, make sure " << filename << " is in the same folder and then rerun Homework3.cpp" << endl;
    cout << "NOTE: Text files should be in the same folder as Homework3.cpp!" << endl;
}

bool fileValidator(const string& dnaFilename)
{
    bool dnaFileGood = static_cast<bool>(ifstream(dnaFilename));
    if (!dnaFileGood)
    {
        printMightBeInDifferentFolderMessage(dnaFilename);
        return false;
    }

    return true;
}

string consoleFilenameReader()
{
    string filename;
    cin >> filename;
    while (!isValidTextFileNameInputFormat(filename))
    {
        filename.clear();
        cout << "Wrong text filename format! Try again!" << endl;
        cin >> filename;
    }

    if (!fileValidator(filename))
    {
        throw exception("Something with the filename/file went wrong!");
    }

    return filename;
}

void programRunner(const string& dna, const CodonToAAMap& codonToAminoacids, const ProteinsMap& proteins, const string& proteinsFilename)
{
    size_t requestsAmount;
    cin >> requestsAmount;
    vector<size_t> requests;
    requests.resize(requestsAmount);

    for (size_t i{ 0 }; i < requests.size(); ++i)
    {
        size_t proteinIdInput;
        cin >> proteinIdInput;
        requests[i] = proteinIdInput;
    }

    for (const size_t& idRequest : requests)
    {
        if (!proteinMapContainsId(proteins, idRequest))
        {
            cout << "No protein in " << proteinsFilename << " with id " << idRequest << endl;
        }
        else
        {
            string proteinOfRequest = getProteinById(proteins, idRequest);
            int proteinInDnaStartIndex = dnaContainsProtein(dna, proteinOfRequest);

            if (proteinInDnaStartIndex != -1)
            {
                string correspondingAASequence = getCorrespondingAASequence(proteinOfRequest, codonToAminoacids);
                cout << "Yes " << proteinInDnaStartIndex << " " << correspondingAASequence << endl;
            }
            else
            {
                cout << "No" << endl;
            }
        }
    }
}

int main()
{   
    try 
    {
        string dnaFilename = consoleFilenameReader();
        string proteinsFilename = consoleFilenameReader();
        string codonToAAFilename = consoleFilenameReader();

        string dna = readDna(dnaFilename);
        CodonToAAMap codonToAminoacids = readCodonToAAMap(codonToAAFilename);
        ProteinsMap proteins = readProteins(proteinsFilename, codonToAminoacids);

        programRunner(dna, codonToAminoacids, proteins, proteinsFilename);
    }
    catch (const exception& except)
    {
     cout << except.what() << endl;
     return 1;
    }

    return 0;
}