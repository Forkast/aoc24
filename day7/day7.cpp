#include <algorithm>
#include <iostream>
#include <utility>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;

long long concat(long long first, long long second) {
    long long res;
    string val = to_string(first) + to_string(second);
    stringstream sstr(val);
    sstr >> res;
    return res;
}

bool solvable(long long res, long long start, vector<int> parts) {
    if (parts.size() == 0) {
        return res == start;
    }
    if (res < start) {
        return false;
    }
    vector<int> rest(parts.begin() + 1, parts.end());
    return solvable(res, start + parts.at(0), rest) || solvable(res, start * parts.at(0), rest) || solvable(res, concat(start, parts.at(0)), rest);
}

void part1() {
    long long result = 0;
    ifstream input("input.txt");
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        vector<int> list;
        int number;
        stringstream iss(line);
        long long res;
        iss >> res;
        char nul;
        iss >> nul;
        while (iss >> number) {
            list.push_back(number);
        }
        vector<int> equation(list.begin() + 1, list.end());
        result += solvable(res, list.at(0), equation) ? res : 0;
    }
    cout << result << "\n";
}

int main() {
    part1();
}