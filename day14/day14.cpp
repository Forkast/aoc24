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

struct coord_hash {
    inline size_t operator()(const icoord &v) const {
        return v.hash();
    }
};


struct pair_hash {
    inline size_t operator()(const pair<icoord,icoord> & v) const {
        return v.first.hash() * 31 + v.second.hash();
    }
};

void part1() {
    long long result = 0;
    ifstream input("input.txt");
    vector<string> map;
    // int W = 11, H = 7;
    int W = 101, H = 103;
    vector<int> quadrants(4, 0);
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        icoord p, v;
        sscanf(line.c_str(), "p=%d,%d v=%d,%d", &p.y, &p.x, &v.y, &v.x);
        for (int i = 0; i < 100; i++) {
            p += v;
            if (p.x >= H)
                p.x %= H;
            if (p.y >= W)
                p.y %= W;
            if (p.x < 0)
                p.x += H;
            if (p.y < 0)
                p.y += W;
        }
        cout << p.x << " " << H / 2 << " : " << p.y << " " << W / 2 << "\n";
        if (p.x < H / 2 && p.y < W / 2) {
            quadrants.at(0)++;
        }
        if (p.x < H / 2 && p.y > W / 2) {
            quadrants.at(1)++;
        }
        if (p.x > H / 2 && p.y < W / 2) {
            quadrants.at(2)++;
        }
        if (p.x > H / 2 && p.y > W / 2) {
            quadrants.at(3)++;
        }
    }

    result = 1;
    for (int i = 0; i < quadrants.size(); i++) {
        cout << i << " " << quadrants.at(i) << "\n";
        result *= quadrants.at(i);
    }

    cout << result << "\n";

}

bool xmas_tree(vector<vector<int>> &map, int size) {
    int res = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.at(i).size(); j++) {
            res += map.at(i).at(j);
        }
    }
    cout << res << " >= " << size << "\n";
    if (res >= size) {
        return true;
    }
    return false;
}

void part2() {
    long long result = 0;
    ifstream input("input.txt");
    // int W = 11, H = 7;
    int W = 101, H = 103;
    vector<pair<icoord, icoord>> robots;
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        icoord p, v;
        sscanf(line.c_str(), "p=%d,%d v=%d,%d", &p.y, &p.x, &v.y, &v.x);
        robots.push_back({p, v});
    }

    vector<vector<int>> map(H, vector<int>(W, 0));
    int i = 0;
    do {
        for (auto &[p, v]: robots) {
            map.at(p.x).at(p.y) = 0;  
        }
        for (auto &[p, v]: robots) {
            p += v;
            if (p.x >= H)
                p.x %= H;
            if (p.y >= W)
                p.y %= W;
            if (p.x < 0)
                p.x += H;
            if (p.y < 0)
                p.y += W;
            map.at(p.x).at(p.y) = 1;
        }
        if (i % 100000 == 0) {
            cout << "**** " << i << " ****" << "\n";
        }

        i++;
    } while (!xmas_tree(map, robots.size()));

    cout << "**** " << i << " ****" << "\n";
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.at(i).size(); j++) {
            if (map.at(i).at(j) == 1) {
                cout << "#";
            } else {
                cout << "-";
            }
        }
        cout << "\n";
    }
    cout << "\n";

    cout << result << "\n";

}

int main() {
    part2();
}