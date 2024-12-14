#include <algorithm>
#include <iostream>
#include <utility>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

struct pair_hash {
    inline size_t operator()(const pair<int,int> & v) const {
        return v.first*31+v.second;
    }
};

vector<long long> split(long long number) {
    vector<long long> res;
    string n = to_string(number);
    res.push_back(stoi(n.substr(0, n.size() / 2)));
    res.push_back(stoi(n.substr(n.size() / 2)));
    return res;
}

bool even_digits(long long number) {
    return to_string(number).size() % 2 == 0;
}

unordered_map<pair<long long, int>, long long, pair_hash> cache;
long long engrave(long long, int);

long long engrave_cached(long long number, int i) {
    if (cache.contains({number, i})) {
        return cache.at({number, i});
    }
    long long a = engrave(number, i);
    cache[{number, i}] = a;
    return a;
}

long long engrave(long long number, int i) {
    if (i == 75) {
        return 1;
    }
    long long res = 0;
    if (number == 0) {
        res = engrave_cached(1, i + 1);
    } else if (even_digits(number)) {
        auto nums = split(number);
        for (auto n : nums) {
            res += engrave_cached(n, i + 1);
        }
    } else {
        res = engrave_cached(number * 2024, i + 1);
    }
    return res;
}

void part1() {
    long long result = 0;
    ifstream input("input.txt");
    vector<long long> list;
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        int number;
        stringstream iss(line);
        while (iss >> number) {
            list.push_back(number);
        }
    }

    for (int j = 0; j < list.size(); j++) {
        cout << j << " " << result << "\n";
        result += engrave(list.at(j), 0);
    }

    cout << result << "\n";
}

int main() {
    part1();
}