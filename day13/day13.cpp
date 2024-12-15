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

inline bool compare(icoord A, icoord B, icoord P, int i) {
    int bt = (P.x - A.x * i) / B.x;
    return P.x - A.x * i == 0 || (
        P.x >= A.x * i && (P.x - A.x * i) % B.x == 0
        && P.y >= A.y * i && (P.y - A.y * i) == bt * B.y
    );
}

int min_tokens(icoord A, icoord B, icoord P) {
    int MAX = 100;
    int MAXT = MAX * 40;
    int AT = 3;
    int BT = 1;
    int tokens = MAXT;
    for (int i = 0; i <= MAX; i++) {
        if (compare(A, B, P, i)) {
            int b = (P.x - A.x * i) / B.x;
            //if (b <= 100) {
                int t = i * AT + b * BT;
                tokens = t < tokens ? t : tokens;
                cout << "A times " << i << " and B times " << b << " with tokens: " << t << "\n";
            //}
        }
    }
    int result = tokens != MAXT ? tokens : 0;
    cout << "result: " << result << "\n";
    return result;
}

void part1() {
    long long result = 0;
    ifstream input("input.txt");
    vector<string> map;
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        int xa, ya, xb, yb, xp, yp;
        sscanf(line.c_str(), "Button A: X+%d, Y+%d", &xa, &ya);
        for (int i = 0; i < 3; i++) {
            getline(input, line);
            if (i == 0) {
                sscanf(line.c_str(), "Button B: X+%d, Y+%d", &xb, &yb);
                cout << line << "\n";
            } else if (i == 1) {
                cout << line << "\n";
                sscanf(line.c_str(), "Prize: X=%d, Y=%d", &xp, &yp);
            }
        }
        result += min_tokens({xa, ya}, {xb, yb}, {xp, yp});
        cout <<"\n\n";
    }

    cout << result << "\n";

}

long long min_tokens2(lcoord A, lcoord B, lcoord P) {
    long long b = (A.x * P.y - P.x * A.y) / (A.x * B.y - B.x * A.y);
    long long a = (P.x - b * B.x) / A.x;
    cout << "X : " << B.x * b + A.x * a << " == " << P.x << "\n";
    cout << "Y : " << B.y * b + A.y * a << " == " << P.y << "\n";
    if (B.x * b + A.x * a == P.x && B.y * b + A.y * a == P.y) {
        return a * 3 + b;
    }
    return 0;
}

void part2() {
    long long result = 0;
    ifstream input("input.txt");
    vector<string> map;
    long long BFN = 10'000'000'000'000;
    // long long BFN = 0;
    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        long long xa, ya, xb, yb, xp, yp;
        sscanf(line.c_str(), "Button A: X+%Ld, Y+%Ld", &xa, &ya);
        for (int i = 0; i < 3; i++) {
            getline(input, line);
            if (i == 0) {
                sscanf(line.c_str(), "Button B: X+%Ld, Y+%Ld", &xb, &yb);
                cout << line << "\n";
            } else if (i == 1) {
                cout << line << "\n";
                sscanf(line.c_str(), "Prize: X=%Ld, Y=%Ld", &xp, &yp);
            }
        }
        result += min_tokens2({xa, ya}, {xb, yb}, {BFN + xp, BFN + yp});
        cout <<"\n\n";
    }

    cout << result << "\n";

}

int main() {
    part2();
}