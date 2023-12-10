#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

long extrapolate(vector<int> history) {
    bool all_zeroes = true;
    for (int &h : history) {
        if (h != 0) {
            all_zeroes = false;
            break;
        }
    }
    if (all_zeroes) { return 0; }

    vector<int> sub;

    for (int i = 1; i < history.size(); i++) {
        sub.push_back(history[i] - history[i-1]);
    }

    return history[history.size() - 1] + extrapolate(sub);
}

vector<int> parse_numbers(string line) {
    vector<int> numbers;
    int start = -1;
    for (int i = 0; i < line.length(); i++) {
        char c = line[i];
        bool is_num_char = isdigit(c) || c == '-';
        if (start == -1 && is_num_char) {
            start = i;
        }

        if (start != -1 && !is_num_char) {
            numbers.push_back(stoi(line.substr(start, i)));
            start = -1;
        }
    }

    numbers.push_back(stoi(line.substr(start)));

    return numbers;
}

string get_line(fstream &file) {
    char raw[128];
    file.getline(raw, 128);
    return string(raw);
}

int main() {
    fstream file("input.txt");

    int score = 0;
    while (file.good()) {
        string line = get_line(file);
        vector<int> history = parse_numbers(line);
        int forecast = extrapolate(history);
        score += forecast;
    }

    cout << "Score: " << score << endl;

    return 0;
}