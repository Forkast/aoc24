
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

int main()
{
    part1();
}

void part2() {
    vector<int> first_list;
    unordered_map<int, int> second_list;
    for (string line; getline(std::cin, line);) {
        int a, b;
        sscanf(line.c_str(), "%d %d", &a, &b);
        first_list.push_back(a);
        if (!second_list.contains(b)) {
            second_list[b] = 0;
        } else {
            second_list++;
        }
    }
    int res = 0;
    for (auto it = first_list.begin(); it != first_list.end(); it++) {
        int a = *it;
        res += a * second_list[a];
    }
    cout << res;

}

void part1() {
    vector<int> a1, b1;
    for (string line; getline(std::cin, line);) {
        int a, b;
        sscanf(line.c_str(), "%d %d", &a, &b);
        a1.push_back(a);
        b1.push_back(b);
    }
    
    std::sort(a1.begin(), a1.end());
    std::sort(b1.begin(), b1.end());
    int res = 0;
    for (int i = 0; i < a1.size(); i++) {
        res += abs(a1.at(i) - b1.at(i));
    }
    cout << res;
}