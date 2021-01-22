#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>

using namespace std;

const static string NUCLEOID_BASES      = {'A', 'C', 'T', 'G'};
const static string TEXTFILE_EXTENSION  = ".txt";

using CodonToAAMap = unordered_map<string, char>;
using ProteinsMap = unordered_map<size_t, string>;

void printInvalidDnaSequenceMessage(const string& dnaFilename)
{
    cout << "* Invalid DNA sequence! Check \'" << dnaFilename << "\'!" << endl;
    cout << "NOTE: DNA consists of As, Cs, Ts and Gs!" << endl;
}

bool readDna(const string& dnaFilename, string& dna)
{
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
                    dna.push_back(c);
                else
                {
                    printInvalidDnaSequenceMessage(dnaFilename);
                    dna.clear();
                    break;
                    return false;
                }
            }
        }
    }
    else
    {
        cout << "Couldn't open " << dnaFilename << endl;
        return false;
    }

    inFile.close();
    return true;
}

bool readCodonToAAMap(const string& filenameDnacToAA, CodonToAAMap& dnacToAAMap)
{
    fstream inFile(filenameDnacToAA);
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
                dnacToAAMap[fileDnac] = fileAA;
            }
        }
    }
    else
    {
        cout << "Couldn't open " << filenameDnacToAA << endl;
        return false;
    }
    inFile.close();
    return true;
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
    if (dnacToAAMap.find(codon) != dnacToAAMap.end() && dnacToAAMap.at(codon) != '*')
    {
        return true;
    }

    return false;
}

bool isStopCodone(const string& codon, const CodonToAAMap& dnacToAAMap)
{
    if (dnacToAAMap.find(codon) != dnacToAAMap.end() && dnacToAAMap.at(codon) == '*')
    {
        return true;
    }

    return false;
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

bool readProteins(const string& proteinsFilename, ProteinsMap& proteins, const CodonToAAMap& dnacToAAMap)
{
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
                if (isValidProteinSequence(protein, dnacToAAMap))
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
        cout << "Couldn't open " << proteinsFilename << endl;
        return false;
    }
    inFile.close();
    return true;
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

bool fileValidator(string& dnaFilename)
{
    cin >> dnaFilename;
    while (!isValidTextFileNameInputFormat(dnaFilename))
    {
        dnaFilename.clear();
        cout << "Wrong text filename format! Try again!" << endl;
        cin >> dnaFilename;
    }

    bool dnaFileGood = static_cast<bool>(ifstream(dnaFilename));
    if (!dnaFileGood)
    {
        printMightBeInDifferentFolderMessage(dnaFilename);
        return false;
    }

    return true;
}

void programRunner(string& dna, CodonToAAMap& codonToAminoacids, ProteinsMap& proteins, string& proteinsFilename)
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

void printReadingDataUnsuccessfulMessage()
{
    cout << "Reading data unsuccessful!" << endl;
    cout << "* Check wether files aren't being currently used by another program! *" << endl;
}

int main()
{
    string dnaFilename;
    if (!fileValidator(dnaFilename))
    {
        return 0;
    }

    string proteinsFilename;
    if (!fileValidator(proteinsFilename))
    {
        return 0;
    }

    string codonToAAFilename;
    if (!fileValidator(codonToAAFilename))
    {
        return 0;
    }

    string dna;
    CodonToAAMap codonToAminoacids;
    ProteinsMap proteins;

    bool successFullyReadData = readDna(dnaFilename, dna) && readCodonToAAMap(codonToAAFilename, codonToAminoacids) && readProteins(proteinsFilename, proteins, codonToAminoacids);

    if (successFullyReadData)
    {
        programRunner(dna, codonToAminoacids, proteins, proteinsFilename);
    }
    else
    {
        printReadingDataUnsuccessfulMessage();
    }

    return 0;
}