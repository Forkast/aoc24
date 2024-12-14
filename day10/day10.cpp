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

struct coord {
    int x, y;

    coord() {
        this->x = 0;
        this->y = 0;
    }
    
    coord(int x, int y) {
        this->x = x;
        this->y = y;
    }

    coord operator-(const coord &o) {
        coord retval;
        retval.x = this->x - o.x;
        retval.y = this->y - o.y;
        return retval;
    }

    size_t hash() const {
        return x * 15 + y;
    }

    coord operator+(const coord &o) {
        coord retval;
        retval.x = this->x + o.x;
        retval.y = this->y + o.y;
        return retval;
    }

    const coord& operator+=(const coord &o) {
        x += o.x;
        y += o.y;
        return *this;
    }

    const coord& operator-=(const coord &o) {
        x -= o.x;
        y -= o.y;
        return *this;
    }

    bool operator==(const coord &o) const {
        return x == o.x && y == o.y;
    }

    bool operator!=(const coord &o) const {
        return !(*this == o);
    }
};

struct coord_hash {
    inline size_t operator()(const coord &v) const {
        return v.hash();
    }
};

template<typename T>
bool within(const vector<T> &map, coord field) {
    return field.x >= 0 && field.y >= 0 && field.x < map.size() && field.y < map.at(field.x).size();
}

unordered_set<coord, coord_hash> visit(vector<vector<int>> &map, vector<vector<int>> &visited, coord start, int val) {
    unordered_set<coord, coord_hash> retval;
    visited.at(start.x).at(start.y)++;
    cout << "searching for summit " << start.x << " " << start.y << " with value " << val << "\n";
    if (val == 9) {
        retval.insert(start);
        cout << "found summit " << start.x << " " << start.y << "\n";
        return retval;
    }
    vector<coord> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (int i = 0; i < directions.size(); i++) {
        coord curr = start + directions.at(i);
        if (within(map, curr) && map.at(curr.x).at(curr.y) == val + 1) {
            auto ends_part = visit(map, visited, curr, val + 1);
            retval.insert(ends_part.begin(), ends_part.end());
        }
    }
    return retval;
}

int get_score(vector<vector<int>> &map, coord start) {
    vector<vector<int>> visited(map.size(), vector<int>(map.at(0).size(), 0));
    int result = 0;
    auto ends = visit(map, visited, start, 0);
    for (auto &s : ends) {
        result += visited.at(s.x).at(s.y);
    }
    return result;
}

void part1() {
    long long result = 0;
    ifstream input("input.txt");
    vector<vector<int>> map;
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        map.push_back({});
        for (auto a : line) {
            int val = a - '0';
            map.at(map.size() - 1).push_back(val);
        }
    }
    cout << "searching...\n";

    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.at(i).size(); j++) {
            if (map.at(i).at(j) == 0) {
                result += get_score(map, {i, j});
            }
        }
    }

    cout << result << "\n";

}

int main() {
    part1();
}