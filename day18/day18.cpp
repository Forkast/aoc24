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

struct record {
    icoord field;
    int score = 0;

    bool operator<(const record &o) const {
        return score > o.score;
    }
};

vector<icoord> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

bool run_bfs(vector<vector<int>> map, const int W) {
    queue<record> Q;
    Q.push({});

    while (!Q.empty()) {
        record curr = Q.front();
        Q.pop();
        if (curr.field == icoord{W - 1, W - 1}) {
            cout << "Can go through " << curr.score << "\n";
            return true;
        }
        for (auto dir : directions) {
            icoord n = curr.field + dir;
            if (within(map, curr.field + dir) && map.at(n.x).at(n.y) == 0) {
                Q.push({n, curr.score + 1});
                map.at(n.x).at(n.y) = 2;
            }
        }
    }
    return false;
}

void part1() {
    long long result = 0;
    ifstream input("input.txt");
    int i = 0;
    int W = 71;
    vector<vector<int>> map(W, vector<int>(W, 0));

    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        istringstream is(line);
        string number_as_string;
        vector<int> coords;
        while (std::getline(is, number_as_string, ',')) {
            coords.push_back(stoi(number_as_string));
        }
        map.at(coords.at(0)).at(coords.at(1)) = 1;

        if (!run_bfs(map, W)) {
            cout << coords.at(0) << "," << coords.at(1) << "\n";
            return;
        }
        i++;
    }
    show(map);

    cout << result << "\n";

}

int main() {
    part1();
}