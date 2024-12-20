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

template<typename T>
struct coord {
    T x, y;

    coord() {
        this->x = 0;
        this->y = 0;
    }
    
    coord(T x, T y) {
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

    coord operator+(const coord &o) const {
        coord retval;
        retval.x = this->x + o.x;
        retval.y = this->y + o.y;
        return retval;
    }

    coord operator*(int mul) const {
        coord retval;
        retval.x = this->x * mul;
        retval.y = this->y * mul;
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

typedef coord<int> icoord;
typedef coord<long long> lcoord;

template<typename T>
struct coord_hash {
    inline size_t operator()(const coord<T> &v) const {
        return v.hash();
    }
};


struct pair_hash {
    inline size_t operator()(const pair<icoord,icoord> & v) const {
        return v.first.hash() * 31 + v.second.hash();
    }
};

template<typename T>
bool at_wall(const vector<T> &map, icoord field) {
    return map.at(field.x).at(field.y) == '#';
}

template<typename T>
bool at_visited(const vector<T> &map, icoord field) {
    return map.at(field.x).at(field.y) == 'O';
}

template<typename T>
bool within(const vector<T> &map, icoord field) {
    return field.x >= 0 && field.y >= 0 && field.x < map.size() && field.y < map.at(field.x).size();
}

template<typename T>
char at(const vector<T> &map, icoord field) {
    return map.at(field.x).at(field.y);
}

template<typename T>
void show(const T item) {
    if (item == 0) {
        cout << '.';
    } else if (item == 1) {
        cout << '#';
    } else if (item == 2) {
        cout << 'O';
    }
}

template<typename T>
void show(const vector<T> & map) {
    int res = 0;
    for (int i = 0; i < map.size(); i++) {
        show(map.at(i));
    }
    cout << "\n";
}

template<>
void show(const string item) {
    cout << item << "\n";
}

struct record {
    icoord field;
    int score = 0;

    bool operator<(const record &o) const {
        return score > o.score;
    }
};

bool impassable(const vector<string> &map, icoord field) {
    if (at(map, field) == '#' || at(map, field) == 'O') {
        return true;
    }
    return false;
}

vector<icoord> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

bool run_bfs(vector<string> &map, unordered_map<icoord, int, coord_hash<int>> &score, vector<vector<icoord>> &path, icoord start, icoord end) {
    queue<record> Q;
    Q.push({start, 0});

    while (!Q.empty()) {
        record curr = Q.front();
        Q.pop();
        if (curr.field == end) {
            break;
        }
        
        for (auto dir : directions) {
            icoord n = curr.field + dir;
            if (within(map, curr.field + dir) && !impassable(map, n)) {
                path.at(n.x).at(n.y) = curr.field;
                Q.push({n, curr.score + 1});
                score[n] = curr.score + 1;
                map.at(n.x).at(n.y) = 'O';
            }
        }
        // show(map);
    }
    return false;
}

void reconstruct(const vector<vector<icoord>> &path, unordered_set<icoord, coord_hash<int>> &on_path, icoord end, icoord start) {
    while (path.at(end.x).at(end.y) != start) {
        on_path.insert(end);
        end = path.at(end.x).at(end.y);
    } 
}

void part1() {
    long long result = 0;
    ifstream input("input.txt");
    vector<string> map;
    icoord start, end;
    int i = 0;
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        map.push_back(line);
        for (int j = 0; j < line.size(); j++) {
            if (line.at(j) == 'S') {
                start = {i, j};
            } else if (line.at(j) == 'E') {
                end = {i, j};
            }
        }
        i++;
    }
    show(map);
    unordered_map<icoord, int, coord_hash<int>> score;
    vector<vector<icoord>> path(map.size(), vector<icoord>(map.at(0).size(), icoord{}));

    run_bfs(map, score, path, start, end);
    show(map);

    unordered_set<icoord, coord_hash<int>> on_path;
    reconstruct(path, on_path, end, start);

    cout << "Reconstructed path\n";

    int normal_score = score.at(end);
    for (const auto &[field, s] : score) {
        for (const auto &dir : directions) {
            icoord cheat_field = field + (dir * 2);
            if (within(map, cheat_field) && on_path.contains(cheat_field)) {
                int cheat_score = score.at(cheat_field) - score.at(field) - 2;
                if (100 <= cheat_score && cheat_score > 0) {
                    // cout << "Found cheat with score: " << score.at(cheat_field) - score.at(field) << "\n";
                    // cout << "Found cheat with score: " << score.at(cheat_field) << " " << score.at(field) << "\n";
                    // cout << "Found cheat to -> from: (" << cheat_field.x << ", " << cheat_field.y << ") -> (" << field.x << ", " << field.y << ")\n";
                    result++;
                }
            }
        }
    }

    cout << result << "\n";

}

int distance(icoord start, icoord end) {
    return abs(start.x - end.x) + abs(start.y - end.y);
}

void part2() {
    long long result = 0;
    ifstream input("input.txt");
    vector<string> map;
    icoord start, end;
    int i = 0;
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        map.push_back(line);
        for (int j = 0; j < line.size(); j++) {
            if (line.at(j) == 'S') {
                start = {i, j};
            } else if (line.at(j) == 'E') {
                end = {i, j};
            }
        }
        i++;
    }
    show(map);
    unordered_map<icoord, int, coord_hash<int>> score;
    vector<vector<icoord>> path(map.size(), vector<icoord>(map.at(0).size(), icoord{}));

    run_bfs(map, score, path, start, end);
    show(map);

    unordered_set<icoord, coord_hash<int>> on_path;
    reconstruct(path, on_path, end, start);

    cout << "Reconstructed path\n";

    score[start] = 0;
    for (const auto &[field, s] : score) {
        for (const auto &[cheat_field, s2] : score) {
            if (distance(field, cheat_field) <= 20 && on_path.contains(cheat_field)) {
                int cheat_score = s2 - s - distance(field, cheat_field);
                if (100 <= cheat_score && cheat_score > 0) {
                    // cout << "Found cheat with score: " << score.at(cheat_field) - score.at(field) << "\n";
                    // cout << "Found cheat with score: " << score.at(cheat_field) << " " << score.at(field) << "\n";
                    // cout << "Found cheat to -> from: (" << cheat_field.x << ", " << cheat_field.y << ") -> (" << field.x << ", " << field.y << ")\n";
                    result++;
                }
            }
        }
    }

    cout << result << "\n";

}

int main() {
    part2();
}