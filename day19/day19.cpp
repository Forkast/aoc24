#include <algorithm>
#include <iostream>
#include <utility>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;


struct stree {
    char c;
    vector<stree*> nodes;

    stree(char c) {
        this->c = c;
        nodes = vector<stree*>(27, nullptr);
    }

    stree()
    : stree('\0') {}

    void insert(const string &towel) {
        if (towel.size() == 0) {
            nodes.at(26) = new stree();
            return;
        }
        int d = towel.at(0) - 'a';
        if (!nodes.at(d)) {
            nodes.at(d) = new stree(d);
        }
        nodes.at(d)->insert(towel.substr(1));
    }

    bool fits(const string &towel) const {
        // cout << "compare \"" << towel << "\" with " << (char)('a' + c) << "\n";
        if (towel.empty()) {
            if (nodes.at(26)) {
                return true;
            }
            return false;
        }

        int d = towel.at(0) >= 'a' && towel.at(0) <= 'z' ? towel.at(0) - 'a' : 26;
        if (nodes.at(d)) {
            return nodes.at(d)->fits(towel.substr(1));
        }
        // cout << "not found\n";
        return false;
    }
};

bool can_arrange(const stree* towels, string pattern) {
    if (pattern.empty()) {
        return true;
    }
    // cout << "rec " << pattern << "\n";
    for (int i = 1; i <= pattern.size(); i++) {
        const string next = pattern.substr(0, i);
        if (towels->fits(next)) {
            if (can_arrange(towels, pattern.substr(i))) {
                return true;
            }
        }
    }
    return false;
}

void part1() {
    long long result = 0;
    ifstream input("input.txt");
    stree *towels = new stree();
    
    string line;
    getline(input, line);
    cout << "line: " << line << "\n";
    istringstream is(line);
    string towel;
    
    getline(input, line);
    while (getline(is, towel, ',')) {
        if (towel.at(0) == ' ') {
            towels->insert(towel.substr(1));
        } else {
            towels->insert(towel);
        }
    }
    

    for (string pattern; getline(input, pattern);) {
        cout << "pattern: " << pattern << "\n";
        if (can_arrange(towels, pattern)) {
            result++;
            continue;
        }
    }

    cout << result << "\n";
}

unordered_map<string, long long> cache;

long long can_arrange2(const stree*, string);

long long cached_arrange2(const stree* towels, string pattern) {
    if (!cache.contains(pattern)) {
        cache[pattern] = can_arrange2(towels, pattern);
    }
    return cache.at(pattern);
}

long long can_arrange2(const stree* towels, string pattern) {
    if (pattern.empty()) {
        return 1;
    }
    long long res = 0;
    for (int i = 1; i <= pattern.size(); i++) {
        const string next = pattern.substr(0, i);
        if (towels->fits(next)) {
            res += cached_arrange2(towels, pattern.substr(i));
        }
    }
    return res;
}

void part2() {
    long long result = 0;
    ifstream input("input.txt");
    stree *towels = new stree();
    
    string line;
    getline(input, line);
    cout << "line: " << line << "\n";
    istringstream is(line);
    string towel;
    
    getline(input, line);
    while (getline(is, towel, ',')) {
        if (towel.at(0) == ' ') {
            towels->insert(towel.substr(1));
        } else {
            towels->insert(towel);
        }
    }
    

    for (string pattern; getline(input, pattern);) {
        cout << "pattern: " << pattern << "\n";
        result += cached_arrange2(towels, pattern);
        cout << result << "\n"; 
    }

    cout << result << "\n";
}

int main() {
    part2();
}