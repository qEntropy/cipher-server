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
    string temp;
    string key;
    if (!file.is_open())
        return -1;



    while (file >> email) {
        file >> hash;

    }

    file.close();
    return 0;
}
