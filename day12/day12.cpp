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


struct pair_hash {
    inline size_t operator()(const pair<coord,coord> & v) const {
        return v.first.hash() * 31 + v.second.hash();
    }
};

template<typename T>
bool within(const vector<T> &map, coord field) {
    return field.x >= 0 && field.y >= 0 && field.x < map.size() && field.y < map.at(field.x).size();
}

vector<coord> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int get_region_perimeter(vector<string> &map, coord field) {
    int res = 0;
    map.at(field.x).at(field.y) = tolower(map.at(field.x).at(field.y));
    for (int i = 0; i < directions.size(); i++) {
        coord next = field + directions.at(i);
        if (within(map, next) && toupper(map.at(field.x).at(field.y)) == map.at(next.x).at(next.y)) {
            res += get_region_perimeter(map, next);
        } else if (!within(map, next) || map.at(field.x).at(field.y) != map.at(next.x).at(next.y)) {
            res += 1;
        }
    }
    return res;
}

int get_region_area(vector<string> &map, coord field) {
    int res = 1;
    map.at(field.x).at(field.y) = tolower(map.at(field.x).at(field.y));
    for (int i = 0; i < directions.size(); i++) {
        coord next = field + directions.at(i);
        if (within(map, next) && toupper(map.at(field.x).at(field.y)) == map.at(next.x).at(next.y)) {
            res += get_region_area(map, next);
        }
    }
    return res;
}

int get_region_sides(vector<string> &map, unordered_set<pair<coord, coord>, pair_hash> &sides, coord field) {
    map.at(field.x).at(field.y) = tolower(map.at(field.x).at(field.y));
    for (int i = 0; i < directions.size(); i++) {
        coord next = field + directions.at(i);
        if (within(map, next) && toupper(map.at(field.x).at(field.y)) == map.at(next.x).at(next.y)) {
            get_region_sides(map, sides, next);
        } else if (!within(map, next) || map.at(field.x).at(field.y) != map.at(next.x).at(next.y)) {
            if (i > 0 && i < 2) {
                sides.insert({{0, field.y}, directions.at(i)});
            } else {
                sides.insert({{field.x, 0}, directions.at(i)});
            }
        }
    }
}

int get_region_value2(vector<string> &map, vector<vector<bool>> &visited, coord field) {
    if (map.at(field.x).at(field.y) == 0) {
        return 0;
    }

    int area = get_region_area(map, field);
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.at(i).size(); j++) {
            if (islower(map.at(i).at(j))) {
                map.at(i).at(j) = toupper(map.at(i).at(j));
                visited.at(i).at(j) = true;
            }
        }
    }
    unordered_set<pair<coord, coord>, pair_hash> sides;
    get_region_sides(map, sides, field);
    int per = sides.size();

    cout << "field : " << map.at(field.x).at(field.y) << " has area : " << area << " and per : " << per << "\n";
    return area * per;
}

int get_region_value(vector<string> &map, vector<vector<bool>> &visited, coord field) {
    if (map.at(field.x).at(field.y) == 0) {
        return 0;
    }

    int area = get_region_area(map, field);
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.at(i).size(); j++) {
            if (islower(map.at(i).at(j))) {
                map.at(i).at(j) = toupper(map.at(i).at(j));
                visited.at(i).at(j) = true;
            }
        }
    }
    int per = get_region_perimeter(map, field);

    cout << "field : " << map.at(field.x).at(field.y) << " has area : " << area << " and per : " << per << "\n";
    return area * per;
}

void part1() {
    long long result = 0;
    ifstream input("input.txt");
    vector<string> map;
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        map.push_back(line);
    }
    vector<vector<bool>> visited(map.size(), vector<bool>(map.at(0).size(), false));

    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.at(i).size(); j++) {
            if (!visited.at(i).at(j)) {
                result += get_region_value(map, visited, {i, j});
            }
        }
    }

    cout << result << "\n";

}

void part2() {
    long long result = 0;
    ifstream input("input.txt");
    vector<string> map;
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        map.push_back(line);
    }
    vector<vector<bool>> visited(map.size(), vector<bool>(map.at(0).size(), false));

    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.at(i).size(); j++) {
            if (!visited.at(i).at(j)) {
                result += get_region_value2(map, visited, {i, j});
            }
        }
    }

    cout << result << "\n";

}

int main() {
    part2();
}