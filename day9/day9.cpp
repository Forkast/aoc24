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

long long get_checksum(long long pos, long long value, long long id) {
    cout << (2 * pos + value - 1) * value / 2 << " of " << id << " = " << (2 * pos + value - 1) * value / 2 * id << "\n";
    return (2 * pos + value - 1) * value / 2 * id;
}

struct block {
    long long pos;
    long long id;
    long long val;
};

void part2() {
    long long result = 0;
    ifstream input("input.txt");
    string line;
    vector<block> blocks;
    vector<block> empty;
    for (; getline(input, line);) {
        cout << "line: " << line << "\n";
        bool is_block = true;
        long long pos = 0;
        for (int i = 0; i < line.size(); i++) {
            int num = line.at(i) - '0';
            if (is_block) {
                blocks.push_back({pos, i / 2, num});
            }
            else {
                empty.push_back({pos, i, num});
            }
            is_block = !is_block;
            pos += num;
        }
    }
    
    for (int i = blocks.size() - 1; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            if (empty.at(j).val >= blocks.at(i).val) {
                blocks.push_back({empty.at(j).pos, blocks.at(i).id, blocks.at(i).val});
                empty.at(j).val -= blocks.at(i).val;
                empty.at(j).pos += blocks.at(i).val;
                blocks.at(i).id = 0;
                break;
            }
        }
    }

    for (int i = 0; i < blocks.size(); i++) {
        result += get_checksum(blocks.at(i).pos, blocks.at(i).val, blocks.at(i).id);
    }
    cout << result << "\n";
}

void part1() {
    long long result = 0;
    ifstream input("input.txt");
    string line;
    vector<int> blocks;
    for (; getline(input, line);) {
        cout << "line: " << line << "\n";
        for (auto d : line) {
            int num = d - '0';
            blocks.push_back(num);
        }
    }

    bool block = true;
    int id = 0;
    int end = blocks.size() - 1;
    int pos = 0;
    int left = blocks.at(end);
    for (int i = 0; i <= end; i++) {
        long long checksum = 0;
        if (block) {
            cout << "it's a block of id: " << i / 2 << "\n";
            checksum = get_checksum(pos, blocks.at(i), i / 2);
            pos += blocks.at(i);
        } else {
            int space = blocks.at(i);
            while (space >= left && i < end) {
                cout << "filling space with " << blocks.at(end) << " blocks of id: " << end / 2 << "\n";
                checksum += get_checksum(pos, blocks.at(end), end / 2);
                pos += blocks.at(end);
                space -= left;
                end -= 2;
                left = blocks.at(end);
                cout << "left " << space << " spaces. start filling with " << end / 2 << "\n";
            }
            if (space > 0) {
                cout << "filling space with " << space << " blocks of id: " << end / 2 << "\n";
                checksum += get_checksum(pos, space, end / 2);
                pos += space;
                blocks.at(end) -= space;
                left = blocks.at(end);
            }
        }

        block = !block;
        id++;
        result += checksum;
    }
    cout << result << "\n";
}

int main() {
    part2();
}