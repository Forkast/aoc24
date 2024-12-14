#include <algorithm>
#include <iostream>
#include <utility>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;


struct pair_hash {
    inline size_t operator()(const pair<int,int> & v) const {
        return v.first*31+v.second;
    }
};

unordered_set<pair<int, int>, pair_hash> before;

struct less_than_key
{
    inline bool operator() (const int& first, const int& second)
    {
        return before.contains({first, second});
    }
};
int contains2(vector<int> &pages) {
    for (int i = pages.size() - 1; i >= 0; i--) {
        cout << "index " << i  << "\n";
        for (int j = i - 1; j >= 0; j--) {
            cout << "comparing " << pages.at(i) << " with " << pages.at(j) << "\n";
            if (before.contains({pages.at(i), pages.at(j)})) {
                cout << "that is not a correct pages " << pages.at(i) << " should be before " << pages.at(j) << "\n";
                sort(pages.begin(), pages.end(), less_than_key());
                cout << "correct ordering: ";
                for (int k = 0; k < pages.size(); k++) {
                    cout << pages.at(k) << " ";
                }
                cout << "\n";
                cout << "returning " << pages.at(pages.size() / 2) << "\n";
                return pages.at(pages.size() / 2);
            }
        }
    }
    cout << "returning " << 0 << "\n";
    return 0;
}

int contains(vector<int> &pages) {
    for (int i = pages.size() - 1; i >= 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            if (before.contains({pages.at(i), pages.at(j)})) {
                cout << "that is not a correct pages " << pages.at(i) << " should be before " << pages.at(j) << "\n";
                return 0;
            }
        }
    }
    cout << "returning " << pages.at(pages.size() / 2) << "\n";
    return pages.at(pages.size() / 2);
}

void part1() {
    long long result = 0;
    bool do_mul = true;
    for (string line; getline(std::cin, line);) {
        int a,b;
        sscanf(line.c_str(), "%d|%d", &a, &b);
        before.insert({a, b});
        if (line == "") {
            break;
        }
    }
    for (string line; getline(std::cin, line);) {
        cout << "line: " << line << "\n";
        string coma = ",";
        vector<int> pages;
        size_t pos = 0;
        while ((pos = line.find(coma)) != std::string::npos) {
            string page = line.substr(0, pos);
            pages.push_back(atoi(page.c_str()));
            line.erase(0, pos + coma.length());
        }
        pages.push_back(atoi(line.substr(0, pos).c_str()));
        result += contains2(pages);
    }
    cout << result << "\n";
}

int main() {
    part1();
}