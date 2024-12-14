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

template<typename T>
bool within(const vector<T> &map, coord field) {
    return field.x >= 0 && field.y >= 0 && field.x < map.size() && field.y < map.at(field.x).size();
}

vector<coord> get_antinodes(coord a, coord b) {
    return {a + a - b, b + b - a};
}

template<typename T>
vector<coord> iterate(const vector<T> &map, coord curr, coord dir) {
    vector<coord> result;
    while (within(map, curr)) {
        result.push_back(curr);
        curr += dir;
    }
    return result;
}

template<typename T>
vector<coord> get_antinodes2(const vector<T> &map, coord a, coord b) {
    vector<coord> result_a = iterate(map, a, a - b);
    vector<coord> result_b = iterate(map, b, b - a);
    result_a.insert(result_a.end(), result_b.begin(), result_b.end());
    return result_a;
}

void part1() {
    long long result = 0;
    ifstream input("input.txt");
    vector<string> lines;
    unordered_map<int, vector<coord>> antennas;
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        lines.push_back(line);
    }

    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines.at(i).size(); j++) {
            char c = lines.at(i).at(j);
            if (c != '.') {
                int index = c - '0';
                if (!antennas.contains(index)) {
                    antennas[index] = {};
                }
                antennas[index].push_back({i, j});
            }
        }
    }
    vector<vector<int>> antinodes(lines.size(), vector<int>(lines.at(0).size(), 0));

    for (auto &[key, list] : antennas) {
        for (int i = 0; i < list.size(); i++) {
            for (int j = 0; j < list.size(); j++) {
                if (i != j) {
                    for (auto antinode : get_antinodes2(lines, list.at(i), list.at(j))) {
                        antinodes.at(antinode.x).at(antinode.y) = 1;
                    }
                }
            }
        }
    }

    for (auto &v : antinodes) {
        for (auto a : v) {
            if (a == 1) {
                result++;
            }
        }
    }


    cout << result << "\n";
}

int main() {
    part1();
}