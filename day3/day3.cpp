#include <iostream>
#include <string>
#include <regex>

using namespace std;

void part1() {
    long long result = 0;
    bool do_mul = true;
    for (string line; getline(std::cin, line);) {
        cout << "line: " << line << "\n";
        regex mul_regex("mul\\([0-9]+,[0-9]+\\)|do\\(\\)|don't\\(\\)", std::regex::extended);
        auto words_begin = 
            std::sregex_iterator(line.begin(), line.end(), mul_regex);
        auto words_end = std::sregex_iterator();
        for (sregex_iterator i = words_begin; i != words_end; ++i) {
            smatch match = *i;
            string match_str = match.str();
            cout << match_str << "\n";
            if (match_str == "do()") {
                do_mul = true;
            } else if (match_str == "don't()") {
                do_mul = false;
            } else {
                int a, b;
                sscanf(match_str.c_str(), "mul(%d, %d)", &a, &b);
                if (do_mul) {
                    result += a * b;
                }
            }
        }
    }
    cout << result << "\n";
}

int main() {
    part1();
}