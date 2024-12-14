
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

using namespace std;

void print_vector(vector<int> list) {
    for (auto a : list) {
        cout << a << " ";
    }
    cout << "\n";
}

bool is_monotonic(vector<int> list, function<bool(int, int)> op, bool removed) {
    print_vector(list);
    if (list.size() < 2) {
        cout << "true\n";
        return true;
    }
    
    if (list.size() == 2) {
        cout << (op(list.at(0), list.at(1)) ? "true" : "false") << "\n";
        return op(list.at(0), list.at(1));
    }
    
        
    if (removed && !op(list.at(0), list.at(1))) {
        cout << "false\n";
        return false;
    }
    
    // remove 1 element but check 0 against 2
    
    vector<int> copy2 = list;
    copy2.erase(copy2.begin() + 2, copy2.begin() + 3);
    
    vector<int> copy1 = list;
    copy1.erase(copy1.begin() + 1, copy1.begin() + 2);
    
    vector<int> copy = list;
    copy.erase(copy.begin(), copy.begin() + 1);
    
    return (op(list.at(0), list.at(1)) && op(list.at(1), list.at(2)) && is_monotonic(copy, op, removed))
    || (!removed && op(list.at(0), list.at(2)) && is_monotonic(copy1, op, true))
    || (!removed && op(list.at(0), list.at(1)) && is_monotonic(copy2, op, true));
}

bool is_increasing(vector<int> list) {
    cout << "is_increasing\n";
    auto op = [](int a, int b) { return a < b && abs(a - b) <= 3 && abs(a - b) >= 1;};
    vector<int> copy = list;
    copy.erase(copy.begin(), copy.begin() + 1);
    return is_monotonic(list, op, false) || is_monotonic(copy, op, true);
}

bool is_decreasing(vector<int> list) {
    cout << "is_decreasing\n";
    auto op = [](int a, int b) { return a > b && abs(a - b) <= 3 && abs(a - b) >= 1;};
    vector<int> copy = list;
    copy.erase(copy.begin(), copy.begin() + 1);
    return is_monotonic(list, op, false) || is_monotonic(copy, op, true);
}

bool is_safe(vector<int> list) {
    return is_increasing(list) || is_decreasing(list);
}

void part1()
{
    int result = 0;
    for (string line; getline(std::cin, line);) {
        cout << "line: " << line << "\n";
        vector<int> list;
        int number;
        stringstream iss(line);
        while (iss >> number) {
            list.push_back(number);
        }
        result += is_safe(list);
    }
    cout << result;
}

int main()
{
    part1();
}