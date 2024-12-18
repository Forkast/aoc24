#include <algorithm>
#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <thread>

using namespace std;

long long combo(const vector<long long> &REG, int operand) {
    if (operand >= 0 && operand <= 3) {
        return operand;
    } else if (operand == 4) {
        return REG.at(0);
    } else if (operand == 5) {
        return REG.at(1);
    } else if (operand == 6) {
        return REG.at(2);
    }
    cout << "INVALID\n";
    return 0;
}

void run(vector<long long> &REG, int opcode, int operand, int &ptr, vector<int> &output) {
    // cout << "running instruction " << opcode << " operand " << operand << " pointer " << ptr << "\n"; 
    switch (opcode) {
    case 0:
        REG.at(0) = REG.at(0) / pow(2, combo(REG, operand));
        break;
    case 1:
        REG.at(1) ^= operand;
        break;
    case 2:
        REG.at(1) = combo(REG, operand) % 8;
        break;
    case 3:
        if (REG.at(0) != 0) {
            ptr = operand;
            return;
        }
        break;
    case 4:
        REG.at(1) ^= REG.at(2);
        break;
    case 5:
        output.push_back(combo(REG, operand) % 8);
        break;
    case 6:
        REG.at(1) = REG.at(0) / pow(2, combo(REG, operand));
        break;
    case 7:
        REG.at(2) = REG.at(0) / pow(2, combo(REG, operand));
        break;
    }
    ptr += 2;
}

int compare(const vector<int> &instr, const vector<int> &output) {
    if (output.size() > instr.size()) {
        return -1;
    }
    for (int i = 0; i < output.size(); i++) {
        if (output.at(output.size() - i - 1) != instr.at(instr.size() - i - 1)) {
            cout << "at i = " << i << " " << output.at(output.size() - i - 1) << " != " << instr.at(instr.size() - i - 1) << "\n";
            return i;
        }
    }
    return output.size();
}

int run_all(vector<long long> &REG, const vector<int> &instructions, int expected) {
    int ptr = 0;
    vector<int> output;

    while (ptr < instructions.size()) {
        run(REG, instructions.at(ptr), instructions.at(ptr + 1), ptr, output);
    }

    int a = compare(instructions, output);
    cout << "a = " << a << "\n";
    if (a == expected) {
        return a;
    }

    if (compare(instructions, output) == instructions.size()) {
        cout << "YEAH! \n";
        return instructions.size();
    }
    return -1;
}

bool iterate(long long A, const vector<int> &instructions, int depth, long long &result) {
    cout << "Trying with A = " << A << "\n";
    bool res = false;
    vector<long long> REG(3, 0);
    for (int i = 0; i < 8; i++) {
        long long B = 8 * A;
        B += i;
        cout << "For B = " << B << " j = " << i << "\n";
        REG.at(0) = B;
        REG.at(1) = 0;
        REG.at(2) = 0;
        int a = run_all(REG, instructions, depth);
        if (a == instructions.size()) {
            result = B;
            return true;
        }
        cout << "a == " << a << " == " << depth << "\n";
        if (a == depth) {
            cout << "Found candidate " << B << "\n";
            res |= iterate(B, instructions, depth + 1, result);
            if (res) {
                cout << "This is the result! " << B << "\n";
                return res;
            }
        }
    }
    return res;
}

void part1() {
    ifstream input("input.txt");
    vector<int> instructions;
    vector<int> output;
    vector<long long> REG(3, 0);

    for (string line; getline(input, line);) {
        cout << "line: " << line << "\n";
        if (line.starts_with("Register A: ")) {
            stringstream strim(line.substr(12));
            strim >> REG.at(0);
        } else if (line.starts_with("Register B: ")) {
            stringstream strim(line.substr(12));
            strim >> REG.at(1);
        } else if (line.starts_with("Register C: ")) {
            stringstream strim(line.substr(12));
            strim >> REG.at(2);
        } else if (line.starts_with("Program: ")) {
            istringstream is(line.substr(9));
            std::string number_as_string;
            while (std::getline(is, number_as_string, ','))
            {
                instructions.push_back(std::stoi(number_as_string));
            }
        }
    }
    
    long long A = 0;
    long long result = 0;
    if (iterate(A, instructions, 1, result));
}

int main() {
    part1();
}