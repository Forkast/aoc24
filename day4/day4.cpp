#include <iostream>
#include <utility>
#include <string>
#include <vector>

using namespace std;

int sub_search2(vector<string> puzzle, pair<int, int> start, pair<int, int> direction, vector<string> s) {
    for (int i = 0; i < s.size(); i++) {
        for (int j = 0; j < s.at(i).size(); j++) {
            int a = start.first + (i - 1) * direction.first;
            int b = start.second + (j - 1) * direction.second;
            //cout << a << ";" << b << "\n";
            //cout << puzzle.at(a).at(b) << " == " << s.at(i).at(j) << "\n";
            if (s.at(i).at(j) != '.' && puzzle.at(a).at(b) != s.at(i).at(j)) {
                //cout << "false\n";
                return 0;
            }
        }
    }
    //cout << "true\n";
    return 1;
}

int sub_search2T(vector<string> puzzle, pair<int, int> start, pair<int, int> direction, vector<string> s) {
    for (int i = 0; i < s.size(); i++) {
        for (int j = 0; j < s.at(i).size(); j++) {
            int a = start.first + (i - 1) * direction.first;
            int b = start.second + (j - 1) * direction.second;
            //cout << a << ";" << b << "\n";
            //cout << puzzle.at(a).at(b) << " == " << s.at(j).at(i) << "\n";
            if (s.at(i).at(j) != '.' && puzzle.at(a).at(b) != s.at(j).at(i)) {
                //cout << "false\n";
                return 0;
            }
        }
    }
    //cout << "true\n";
    return 1;
}

bool within2(vector<string> puzzle, int i, int j, pair<int, int> direction) {
    return i - 1 >= 0 && i + 1 < puzzle.size() && j - 1 >= 0 && j + 1 < puzzle.at(i).size();
}

const vector<string> X_MAS = {"M.S",
                              ".A.",
                              "M.S"};

int search2(vector<string> puzzle) {
    vector<pair<int, int>> directions = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    int result = 0;
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle.at(i).size(); j++) {
            if (puzzle.at(i).at(j) == 'A') {
                //cout << i << " " << j << "\n";
                for (pair<int, int> direction : directions) {
                    int a = within2(puzzle, i, j, direction) ? sub_search2(puzzle, {i, j}, direction, X_MAS) : 0;
                    if (a == 1) {
                        result++;
                        break;
                    }
                    a = within2(puzzle, i, j, direction) ? sub_search2T(puzzle, {i, j}, direction, X_MAS) : 0;
                    if (a == 1) {
                        result++;
                        break;
                    }
                }
            }
        }
    }
    return result;
}

const string XMAS = "XMAS";

int sub_search(vector<string> puzzle, pair<int, int> start, pair<int, int> direction, string s) {
    for (int i = 0; i < s.size(); i++) {
        if (puzzle.at(start.first + direction.first * i).at(start.second + direction.second * i) != s.at(i)) {
            return 0;
        }
    }
    return 1;
}

bool within(vector<string> puzzle, int i, int j, pair<int, int> direction) {
    i = i + direction.first * (XMAS.size() - 1);
    j = j + direction.second * (XMAS.size() - 1);
    return i >= 0 && i < puzzle.size() && j >= 0 && j < puzzle.at(i).size();
}

int search(vector<string> puzzle) {
    vector<pair<int, int>> directions = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    int result = 0;
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle.at(i).size(); j++) {
            if (puzzle.at(i).at(j) == XMAS.at(0)) {
                cout << i << " " << j << "\n";
                for (pair<int, int> direction : directions) {
                    result += within(puzzle, i, j, direction) ? sub_search(puzzle, {i, j}, direction, XMAS) : 0;
                }
            }
        }
    }
    return result;
}

void part2() {
    long long result = 0;
    bool do_mul = true;
    vector<string> puzzle;
    for (string line; getline(std::cin, line);) {
        puzzle.push_back(line);
    }
    cout << search2(puzzle) << "\n";
}


void part1() {
    long long result = 0;
    bool do_mul = true;
    vector<string> puzzle;
    for (string line; getline(std::cin, line);) {
        puzzle.push_back(line);
    }
    cout << search(puzzle) << "\n";
}

int main() {
    part2();
}