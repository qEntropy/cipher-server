#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;

vector<string> splitAtSpace(string pair);

int main() {

    ifstream file;
    file.open("keys20.txt");
    vector<string> vectorPair;
    unordered_map<string, string> emailHashMap;

    if (!file.is_open())
        return -1;

    string pair;

    while (getline(file, pair)) {
        vectorPair = splitAtSpace(pair);
        emailHashMap.insert(make_pair(vectorPair[1], vectorPair[2]));
    }

    file.close();
    return 0;
}

vector<string> splitAtSpace(string pair) {

    vector<string> vecPair;
    istringstream stringStream(pair);
    do {
        string token;
        stringStream >> token;
        vecPair.push_back(token);
    } while(stringStream);

    return vecPair;
}
