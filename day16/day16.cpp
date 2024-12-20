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
void show(const vector<T> & map) {
    int res = 0;
    for (int i = 0; i < map.size(); i++) {
        cout << map.at(i);
        cout << "\n";
    }
    cout << "\n";
}


struct record {
    icoord start;
    icoord dir;
    int score = 0;

    bool operator<(const record &o) const {
        return score > o.score;
    }
};

icoord rotate(icoord dir, int i) {
    if (i == 1) {
        return {-dir.y, dir.x};
    } else {
        return {dir.y, -dir.x};
    }
}

bool lower(unordered_map<pair<icoord, icoord>, int, pair_hash> &visited, record f) {
    return visited.at({f.start, f.dir}) >= f.score;
}

template<typename T>
vector<record> possible_steps(vector<T> &map, unordered_map<pair<icoord, icoord>, int, pair_hash> &visited, record f) {
    vector<record> ret;
    if (within(map, f.start + f.dir) && !at_wall(map, f.start + f.dir)) {
        ret.push_back({f.start + f.dir, f.dir, f.score + 1});
    }
    if (!at_visited(map, f.start)) {
        icoord dir = rotate(f.dir, 1);
        if (within(map, f.start + dir) && !at_wall(map, f.start + dir)) {
            ret.push_back({f.start, dir, f.score + 1000});
        }
        dir = rotate(f.dir, -1);
        if (within(map, f.start + dir) && !at_wall(map, f.start + dir)) {
            ret.push_back({f.start, dir, f.score + 1000});
        }
        map.at(f.start.x).at(f.start.y) = 'O';
    }
    return ret;
}

void reconstruct(const unordered_map<pair<icoord, icoord>, vector<record>, pair_hash> &path, unordered_set<icoord, coord_hash<int>> &distinct, pair<icoord, icoord> field, pair<icoord, icoord> end) {
    // cout << "Reconstruct: (" << field.first.x << ", " << field.first.y << ")\n";
    distinct.insert(field.first);
    if (field.first == end.first) {
        // cout << "Ending game: " << field.first.x << " " << field.first.y << "\n";
        return;
    }
    // if (path.at(field).size() > 1) {
        // cout << "Branching\n";
    // }
    for (auto &r : path.at(field)) {
        reconstruct(path, distinct, {r.start, r.dir}, end);
    }
}

void run_bfs(const icoord &start, const icoord &dir, unordered_map<pair<icoord, icoord>, int, pair_hash> &visited, vector<string> &map, long long &result, unordered_map<pair<icoord, icoord>, vector<record>, pair_hash> &path) {
    priority_queue<record> Q;
    Q.push({start, dir, 0});
    while (!Q.empty()) {
        record curr = Q.top();
        Q.pop();

        if (visited.contains({curr.start, curr.dir}) && visited.at({curr.start, curr.dir}) >= curr.score) {
            visited[{curr.start, curr.dir}] = curr.score;
        } else if (visited.contains({curr.start, curr.dir}) && visited.at({curr.start, curr.dir}) < curr.score) {
            continue;
        }

        // cout << "being at " << curr.start.x << ", " << curr.start.y << " facing " << curr.dir.x << ", " << curr.dir.y << " with score " << curr.score << "\n";
        if (at(map, curr.start) == 'E') {
            result = curr.score;
            break;
        }

        auto next = possible_steps(map, visited, curr);
        for (const auto &n : next) {
            // cout << "possible step: (" << n.start.x << ", " << n.start.y << ") facing: (" << n.dir.x << ", " << n.dir.y << ") with score " << n.score << "\n";
            Q.push(n);
            // cout << "worked1 " << path.contains({n.start, n.dir}) << "\n";
            // if (path.contains({n.start, n.dir})) {
                // cout << "worked2 " << path.at({n.start, n.dir}).at(0).score << "\n";
            // }
            if (!path.contains({n.start, n.dir}) || path.at({n.start, n.dir}).at(0).score >= n.score) {
                // cout << "worked\n";
                if (!path.contains({n.start, n.dir})) {
                    path[{n.start, n.dir}] = {};
                }
                
                path.at({n.start, n.dir}).push_back(record{curr.start, curr.dir, n.score});
            }
        }
        // show(map);
    }
}

void part1() {
    long long result = 0;
    ifstream input("input.txt");
    vector<string> map;
    int i = 0;
    icoord start;
    icoord end;
    icoord dir{0, 1};

    unordered_map<pair<icoord, icoord>, vector<record>, pair_hash> path;

    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        for (int j = 0; j < line.size(); j++) {
            if (line.at(j) == 'S') {
                start = {i, j};
            } else if (line.at(j) == 'E') {
                end = {i, j};
            }
        }
        map.push_back(line);
        i++;
    }
    unordered_map<pair<icoord, icoord>, int, pair_hash> visited;
    
    run_bfs(start, dir, visited, map, result, path);
    show(map);
    cout << result << "\n";

    unordered_set<icoord, coord_hash<int>> distinct;
    reconstruct(path, distinct, {end, icoord{-1, 0}}, {start, icoord{-1, 0}});
    result = distinct.size();

    for (auto r : distinct) {
        map.at(r.x).at(r.y) = '+';
    }
    show(map);

    cout << result << "\n";

}

int main() {
    part1();
}