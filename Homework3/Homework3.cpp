#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

const static string DNA_FILENAME = "dna.txt";
const static string DNAC_TO_AA_FILENAME = "DNACtoAA.txt";

struct DNAC_AA_PAIR
{
    string codone;
    char aa;

    DNAC_AA_PAIR(string _codone, char _aa)
    {
        if (_codone.size() > 0 && _codone.size() == 3)
            codone = _codone;

        if (isupper(_aa))
            aa = _aa;
        else
            aa = toupper(_aa);
    }
};

void readDna(const string& filenameDna, string& dna)
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
                dna.push_back(c);
            }
        }
    }
    inFile.close();
}

void readDnacToAAMap(const string& filenameDnacToAA, unordered_map<string, char>& dnacToAAMap)
{
    fstream inFile(filenameDnacToAA);
    if (inFile)
    {
        string line;

        while (getline(inFile, line))
        {
            istringstream iss (line);

            string  fileDnac;
            char    fileAA;

            if (iss >> fileDnac >> fileAA)
            {
                dnacToAAMap[fileDnac] = fileAA;
            }
        }       
    }
    inFile.close();
}

void printMap(const unordered_map<string, char>& map)
{
    for (const std::pair<string, char>& p : map)
    {
        cout << "[" << p.first << "] - " << p.second << endl;
    }
}

int main()
{
    string dna;
    readDna(DNA_FILENAME, dna);
    cout << "DNA cut: " << dna << endl;
    
    unordered_map<string, char> dnacToAAMap;
    readDnacToAAMap(DNAC_TO_AA_FILENAME, dnacToAAMap);
    printMap(dnacToAAMap);

    return 0;
}
