#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>

using namespace std;

const static string NUCLEOID_BASES      = {'A', 'C', 'T', 'G'};
const static string TEXTFILE_EXTENSION  = ".txt";

//struct DNAC_AA_PAIR
//{
//    string codone;
//    char aa;
//
//    DNAC_AA_PAIR(string _codone, char _aa)
//    {
//        if (_codone.size() > 0 && _codone.size() == 3)
//            codone = _codone;
//
//        if (isupper(_aa))
//            aa = _aa;
//        else
//            aa = toupper(_aa);
//    }
//};

using DNACToAAMap = unordered_map<string, char>;
using proteinsMap = unordered_map<size_t, string>;

bool isValidDnaSequence(const string& dnaSeq)
{
    for (const char& base : dnaSeq)
    {
        if (NUCLEOID_BASES.find(base) == string::npos)
        {
            return false;
        }
    }

    return true;
}

void printInvalidDnaInputMessage()
{
    cout << "* Invalid DNA cut! Check \"dna.txt\" *" << endl;
    cout << "NOTE: DNA consists of As, Cs, Ts and Gs!" << endl;
}

bool readDna(const string& filenameDna, string& dna)
{
    fstream inFile(filenameDna);
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
                    dna.clear();
                    break;
                }
            }
        }
    }
    else
    {
        cout << "Couldn't open " << filenameDna << endl;
        return false;
    }
    inFile.close();
    return true;
}

bool readDnacToAAMap(const string& filenameDnacToAA, DNACToAAMap& dnacToAAMap)
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

void printDnacToAAMap(const DNACToAAMap& map)
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

bool isStartCodone(const string& codone, const DNACToAAMap& dnacToAAMap)
{
    if (dnacToAAMap.find(codone) != dnacToAAMap.end() && dnacToAAMap.at(codone) != '*')
    {
        return true;
    }

    return false;
}

bool isStopCodone(const string& codone, const DNACToAAMap& dnacToAAMap)
{
    if (dnacToAAMap.find(codone) != dnacToAAMap.end() && dnacToAAMap.at(codone) == '*')
    {
        return true;
    }

    return false;
}

bool isValidProteinSequence(const string& proteinSeq, const DNACToAAMap& dnacToAAMap)
{

    string firstTriplet = getFirstTriplet(proteinSeq);
    string lastTriplet = getLastTriplet(proteinSeq);

    if (proteinSeq.size() % 3 == 0 &&  isStartCodone(firstTriplet, dnacToAAMap) && isStopCodone(lastTriplet, dnacToAAMap))
    {
        return true;
    }

    return false;
}

using ProteinPair = std::pair<size_t, string>;

//struct ProteinPair
//{
//    size_t id;
//    string proteinSequence;
//
//    ProteinPair(size_t _id, string _proteinSequence) : id{ _id }, proteinSequence{_proteinSequence} {}
//    bool operator==(const ProteinPair& other) const 
//    {
//        return id == other.id;
//    }
//};

//template<>
//struct hash<ProteinPair>
//{
//    size_t operator() (const ProteinPair& pair) const
//    {
//        return hash<size_t>() (pair.id);
//    }
//};

bool readProteins(const string& filenameProteins, proteinsMap& proteins, const DNACToAAMap dnacToAAMap)
{
    fstream inFile(filenameProteins);
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
                    cout << "Invalid protein input in " << filenameProteins << ". Didn't input " << protein << " in set." << endl;
                }
            }
        }
    }
    else
    {
        cout << "Couldn't open " << filenameProteins << endl;
        return false;
    }
    inFile.close();
    return true;
}

void printProteins(const proteinsMap& map)
{
    for (const std::pair<size_t, string>& p : map)
    {
        cout << p.first << " " << p.second << endl;
    }
}

bool proteinPairsSetContainsId(const proteinsMap& map, size_t id)
{
    return map.find(id) != map.end();
}

const string& getProteinByIndex(const proteinsMap& map, size_t id)
{
    return map.at(id);
}

int firstStartCodoneIndex(const string& dna, const DNACToAAMap& dnacToAAMap)
{
    for (size_t i{0}; i < dna.size() - 2; ++i)
    {
        string currentTriplet = dna.substr(i, 3);
        
        if (isStartCodone(currentTriplet, dnacToAAMap) && dna.find(currentTriplet) != string::npos)
        {
            return dna.find(currentTriplet);
        }

    }

    return -1;
}

int firstStopCodoneIndex(const string& dna, const DNACToAAMap& dnacToAAMap)
{
    for (size_t i{ 0 }; i < dna.size() - 2; ++i)
    {
        string currentTripplet = dna.substr(i, 3);

        if (isStopCodone(currentTripplet, dnacToAAMap) && dna.find(currentTripplet) != string::npos)
        {
            return dna.find(currentTripplet);
        }

    }

    return -1;
}

int dnaContainsProtein(const string& dna, const string& protein)
{
    string proteinStartCodone = getFirstTriplet(protein);
    string proteinStopCodone = getLastTriplet(protein);

    if (dna.find(proteinStopCodone) != string::npos)
    {
        if (dna.find(proteinStartCodone) != string::npos)
        {
            return dna.find(proteinStartCodone);
        }
    }
   
    return -1;
}

string getCorrespondingAASequence(const string& protein, const DNACToAAMap& dnacToAAMap)
{
    string result;

    for (size_t i{ 0 }; i < protein.size() - 2; ++i)
    {
        string currentCodone = protein.substr(i, 3);
        result.push_back(dnacToAAMap.at(currentCodone));
    }

    return result;
}

bool isValidTextFileNameInputFormat(const string& filename)
{
    return  filename.size() > 4 && filename.substr(filename.size() - 4) == TEXTFILE_EXTENSION;
}

int main()
{
    string DNA_FILENAME;
    cin >> DNA_FILENAME;
    while (!isValidTextFileNameInputFormat(DNA_FILENAME))
    {
        DNA_FILENAME.clear();
        cout << "Wrong text filename format! Try again!" << endl;
        cin >> DNA_FILENAME;
    }

    string PROTEINS_FILENAME;
    cin >> PROTEINS_FILENAME;
    if (!isValidTextFileNameInputFormat(PROTEINS_FILENAME))
    {
        PROTEINS_FILENAME.clear();
        cout << "Wrong text filename format! Try again!" << endl;
        cin >> PROTEINS_FILENAME;
    }

    string DNAC_TO_AA_FILENAME;
    cin >> DNAC_TO_AA_FILENAME;
    if (!isValidTextFileNameInputFormat(DNAC_TO_AA_FILENAME))
    {
        DNAC_TO_AA_FILENAME.clear();
        cout << "Wrong text filename format! Try again!" << endl;
        cin >> DNAC_TO_AA_FILENAME;
    }

    string dna;
    DNACToAAMap dnacToAAMap;
    proteinsMap proteins;
    
    bool successfullyReadAllFiles = readDna(DNA_FILENAME, dna) && readDnacToAAMap(DNAC_TO_AA_FILENAME, dnacToAAMap) && readProteins(PROTEINS_FILENAME, proteins, dnacToAAMap);

    if (successfullyReadAllFiles)
    {
        //cout << "DNA cut: " << dna << endl;
        //printDnacToAAMap(dnacToAAMap);
        //printProteins(proteins);

        size_t requestsAmount;
        cin >> requestsAmount;
        vector<size_t> requests;
        requests.resize(requestsAmount);

        for (size_t i{0}; i < requests.size(); ++i)
        {
            size_t proteinIdInput;
            cin >> proteinIdInput;
            requests[i] = proteinIdInput;
        }

        for (const size_t& idRequest : requests)
        {
            if (!proteinPairsSetContainsId(proteins, idRequest))
            {
                cout << "No protein in " << PROTEINS_FILENAME <<  " with id " << idRequest << endl;
            }
            else
            {
                string proteinOfRequest = getProteinByIndex(proteins, idRequest);
                int proteinInDnaStartIndex = dnaContainsProtein(dna, proteinOfRequest);

                if (proteinInDnaStartIndex != -1)
                {
                    string correspondingAASequence = getCorrespondingAASequence(proteinOfRequest, dnacToAAMap);
                    cout << "Yes " << proteinInDnaStartIndex << " " << correspondingAASequence << endl;
                }
                else
                {
                    cout << "No" << endl;
                }
            }
        }
    }
    else
    {
        cout << "* Text file(s) not in the same folder! *" << endl;
        cout << "NOTE: Text files should be in the same folder as Homework3.cpp!" << endl;
    }  

    return 0;
}
