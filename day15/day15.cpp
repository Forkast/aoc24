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

bool at_wall(vector<string> & map, icoord field) {
    return map.at(field.x).at(field.y) == '#';
}

bool at_empty(vector<string> & map, icoord field) {
    return map.at(field.x).at(field.y) == '.';
}

bool box(vector<string> & map, icoord field) {
    return map.at(field.x).at(field.y) == 'O';
}

void put_box(vector<string> & map, icoord field) {
    map.at(field.x).at(field.y) = 'O';
}

void remove_box(vector<string> & map, icoord field) {
    map.at(field.x).at(field.y) = '.';
}

bool lbox(vector<string> & map, icoord field) {
    return map.at(field.x).at(field.y) == '[';
}

bool rbox(vector<string> & map, icoord field) {
    return map.at(field.x).at(field.y) == ']';
}

bool box2(vector<string> & map, icoord field) {
    return lbox(map, field) || rbox(map, field);
}

void put_box2(vector<string> & map, icoord field) {
    map.at(field.x).at(field.y) = '[';
    map.at(field.x).at(field.y + 1) = ']';
}

void remove_box2(vector<string> & map, icoord field) {
    map.at(field.x).at(field.y) = '.';
    map.at(field.x).at(field.y + 1) = '.';
}

int get_gps(vector<string> & map) {
    int res = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.at(i).size(); j++) {
            if (box(map, {i, j})) {
                res += 100 * i + j;
            }
        }
    }
    return res;
}

int get_gps2(vector<string> & map) {
    int res = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.at(i).size(); j++) {
            if (lbox(map, {i, j})) {
                res += 100 * i + j;
            }
        }
    }
    return res;
}

void show_warehouse(vector<string> & map) {
    int res = 0;
    for (int i = 0; i < map.size(); i++) {
        cout << map.at(i);
        cout << "\n";
    }
}

void part1() {
    long long result = 0;
    ifstream input("input.txt");
    vector<string> map;
    icoord robot;
    int i = 0;
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        map.push_back(line);
        for (int j = 0; j < line.size(); j++) {
            if (line.at(j) == '@') {
                robot.x = i;
                robot.y = j;
                map.at(i).at(j) = '.';
            }
        }
        i++;
        if (line == "") {
            break;
        }
    }

    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        icoord dir;
        for (auto d : line) {
            if (d == '<') {
                dir = {0, -1};
            } else if (d == '>') {
                dir = {0, 1};
            } else if (d == '^') {
                dir = {-1, 0};
            } else if (d == 'v') {
                dir = {1, 0};
            }

            icoord next = robot + dir;
            while (!at_wall(map, next) && !at_empty(map, next)) {
                next += dir;
            }
            if (at_empty(map, next)) {
                while (next != robot) {
                    if (box(map, next - dir)) {
                        put_box(map, next);
                    } else {
                        remove_box(map, next);
                    }
                    next -= dir;
                }
                robot += dir;
            }
        }
    }

    show_warehouse(map);
    result = get_gps(map);

    cout << result << "\n";

}

bool can_push(vector<string> &map, icoord field, icoord dir) {
    icoord next = field + dir;
    if (at_wall(map, field)) {
        return false;
    }
    if (dir.x != 0) {
        if (lbox(map, field)) {
            icoord next2 = field + dir;
            next2.y += 1;
            return can_push(map, next, dir) && can_push(map, next2, dir);
        } else if (rbox(map, field)) {
            icoord next2 = field + dir;
            next2.y -= 1;
            return can_push(map, next, dir) && can_push(map, next2, dir);
        }
        return true;
    } else if (box2(map, field)) {
        return can_push(map, next, dir);
    }
    return true;
}

void push(vector<string> &map, icoord field, icoord dir) {
    //cout << "pushing...\n";

    if (dir.x != 0) {
        //cout << "going vertival\n";
        if (lbox(map, field)) {
            //cout << "lbox\n";
            icoord next = field + dir;
            push(map, next, dir);
            next.y += 1;
            push(map, next, dir);
            remove_box2(map, field);
        } else if (rbox(map, field)) {
            //cout << "rbox\n";
            icoord next = field + dir;
            push(map, next, dir);
            next.y -= 1;
            push(map, next, dir);
            remove_box2(map, field - icoord{0, 1});
        }
    } else if (box2(map, field)) {
        //cout << "just box\n";
        icoord next = field + dir;
        push(map, next, dir);
    }
    icoord prev = field - dir;
    map.at(field.x).at(field.y) = map.at(prev.x).at(prev.y);
    //cout << "end pushing...\n";

}

void part2() {
    long long result = 0;
    ifstream input("input.txt");
    vector<string> map;
    icoord robot;
    int i = 0;
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        map.push_back({});
        
        for (int j = 0; j < line.size(); j++) {
            if (line.at(j) == '@') {
                robot.x = i;
                robot.y = j * 2;
                map.at(i).push_back('.');
                map.at(i).push_back('.');
            } else if (line.at(j) == '#') {
                map.at(i).push_back('#');
                map.at(i).push_back('#');
            } else if (line.at(j) == '.') {
                map.at(i).push_back('.');
                map.at(i).push_back('.');
            } else if (line.at(j) == 'O') {
                map.at(i).push_back('[');
                map.at(i).push_back(']');
            }
        }
        i++;
        if (line == "") {
            break;
        }
    }

    show_warehouse(map);

    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        icoord dir;
        for (auto d : line) {
            //cout << "direction: " << d << "\n";
            if (d == '<') {
                dir = {0, -1};
            } else if (d == '>') {
                dir = {0, 1};
            } else if (d == '^') {
                dir = {-1, 0};
            } else if (d == 'v') {
                dir = {1, 0};
            }

            if (can_push(map, robot + dir, dir)) {
                //cout << "can push\n";
                push(map, robot + dir, dir);
                //cout << "pushed\n";
                robot += dir;
            } else {
              //  cout << "cannot push\n";
            }
            //show_warehouse(map);
        }
    }

    show_warehouse(map);
    result = get_gps2(map);

    cout << result << "\n";

}

int main() {
    part2();
}