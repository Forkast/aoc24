#include <algorithm>
#include <iostream>
#include <utility>
#include <fstream>
#include <string>
#include <unordered_set>
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

struct pair_hash {
    inline size_t operator()(const pair<coord,coord> &v) const {
        return v.first.hash() * 31 + v.second.hash();
    }
};

bool within(const vector<string> &map, coord field) {
    return field.x >= 0 && field.y >= 0 && field.x < map.size() && field.y < map.at(field.x).size();
}

bool obstacle(const vector<string> &map, coord field) {
    return map.at(field.x).at(field.y) == '#';
}
vector<vector<int>> possible_obstacles;

vector<coord> directions = {{-1, 0}, {0, 1}, {1, 0}, {0,-1}};
int direction = -1;

bool find_loop(vector<string> &map, coord start, int direction) {
    unordered_set<pair<coord, coord>, pair_hash> visited;
    cout << "loop searching\n";
    coord curr = start;
    coord barrier = start + directions.at(direction);
    for (int i = 1; true; i++) {
        coord dir = directions.at((direction + i) % directions.size());
        // cout << "turning ******** " << curr.x << " " << curr.y << " with direction " << dir.x << " " << dir.y << "\n";
        if (!visited.contains({curr, dir})) {
            visited.insert({curr, dir});
        } else {
            return true;
        }
        while (within(map, curr) && !obstacle(map, curr) && curr != barrier) {
            curr += dir;
        }
        if (!within(map, curr)) {
            return false;
        }
        curr -= dir;
    }
    return false;
}

coord find_obstacle(vector<string> &map, coord start, coord dir) {
    while (within(map, start) && !obstacle(map, start)) {
        if (within(map, start + dir) && map.at(start.x + dir.x).at(start.y + dir.y) != 'X' && find_loop(map, start, direction)) {
            cout << "found ******** loop " << start.x << " " << start.y << "\n";
            coord obst = start + dir;
            possible_obstacles.at(obst.x).at(obst.y) = 1;
        }
        map.at(start.x).at(start.y) = 'X';
        start += dir;
    }
    return start;
}

bool at_border(const vector<string> &map, coord start) {
    return start.x <= 0 || start.y <= 0 || start.x >= map.size() - 1 || start.y >= map.at(start.x).size() - 1;
}

coord rotate() {
    direction = (direction + 1) % directions.size();
    return directions.at(direction);
}

void part1() {
    vector<string> map;
    long long result = 0;
    bool do_mul = true;
    ifstream input("input.txt");
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        map.push_back(line);
    }
    coord start;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.at(i).size(); j++) {
            if (map.at(i).at(j) == '^') {
                start = {i, j};
            }
        }
    }
    coord dir = rotate();
    while (!at_border(map, start)) {
        cout << "direction: " << dir.x << " " << dir.y << "\n";
        coord field = find_obstacle(map, start, dir) - dir;
        cout << "found obstacle " << field.x << " " << field.y << "\n";
        dir = rotate();
        start = field;
    }

    result = 0;
    for (auto v : map) {
        for (auto c : v) {
            if (c == 'X') result++;
        }
    }
    cout << result << "\n";
}

void part2() {
    vector<string> map;
    long long result = 0;
    bool do_mul = true;
    ifstream input("input.txt");
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        map.push_back(line);
    }
    coord start;
    for (int i = 0; i < map.size(); i++) {
        possible_obstacles.push_back(vector<int>(map.at(i).size(), 0));
        for (int j = 0; j < map.at(i).size(); j++) {
            if (map.at(i).at(j) == '^') {
                start = {i, j};
            }
        }
    }
    coord dir = rotate();
    while (!at_border(map, start)) {
        cout << "direction: " << dir.x << " " << dir.y << "\n";
        coord field = find_obstacle(map, start, dir) - dir;
        cout << "found obstacle " << field.x << " " << field.y << "\n";
        dir = rotate();
        start = field;
    }
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.at(i).size(); j++) {
            result += possible_obstacles.at(i).at(j);
        }
    }
    cout << result << "\n";
}

int main() {
    part2();
}