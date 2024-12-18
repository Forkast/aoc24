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

void reconstruct(const unordered_map<icoord, vector<record>, coord_hash<int>> &path, unordered_set<icoord, coord_hash<int>> &distinct, icoord field, icoord end) {
    cout << "Reconstruct: (" << field.x << ", " << field.y << ")\n";
    distinct.insert(field);
    if (field == end) {
        cout << "Ending game: " << field.x << " " << field.y << "\n";
        return;
    }
    if (path.at(field).size() > 1) {
        cout << "Branching\n";
    }
    for (auto &r : path.at(field)) {
        if (!distinct.contains(r.start)) {
            reconstruct(path, distinct, r.start, end);
        }
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

    unordered_map<icoord, vector<record>, coord_hash<int>> path;

    priority_queue<record> Q;

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
    
    Q.push({start, dir, 0});
    while (!Q.empty()) {
        record curr = Q.top();
        Q.pop();
        // cout << "being at " << curr.start.x << ", " << curr.start.y << " facing " << curr.dir.x << ", " << curr.dir.y << " with score " << curr.score << "\n";
        if (at(map, curr.start) == 'E') {
            result = curr.score;
            break;
        }
        auto next = possible_steps(map, visited, curr);
        for (const auto &n : next) {
            Q.push(n);
            if (!path.contains(n.start) || !visited.contains({n.start, n.dir}) || visited.at({n.start, n.dir}) >= n.score) {
                visited[{n.start, n.dir}] = n.score;
                if (!path.contains(n.start) && n.start != curr.start) {
                    path[n.start] = {};
                }
                
                if (n.start != curr.start) {
                    path.at(n.start).push_back(curr);
                }
            }
        }
        // show(map);
    }
    show(map);
    cout << result << "\n";

    unordered_set<icoord, coord_hash<int>> distinct;
    reconstruct(path, distinct, end, start);
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