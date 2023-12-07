#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

string get_line(fstream &file) {
    char raw[64];
    file.getline(raw, 64);
    return string(raw);
}

long calculate_travel_distance(long t, long duration) {
    return t * (duration - t);
}

vector<long> parse_numbers(string line) {
    vector<long> numbers;
    int start_idx = -1;

    for (int i = 0; i < line.length(); i++) {
        char c = line[i];
        if (!isdigit(c) && start_idx != -1) {
            long n = stoi(line.substr(start_idx, i));
            numbers.push_back(n);
            start_idx = -1;
            continue;
        }

        if (start_idx == -1 && isdigit(c)) {
            start_idx = i;
        }
    }

    long n = stoi(line.substr(start_idx, line.length()));
    numbers.push_back(n);
    start_idx = -1;

    return numbers;
}

vector<long> parse_numbers_without_space(string line) {
    char raw[16];
    int i = 0;

    for (char &c : line) {
        if (isdigit(c)) {
            raw[i] = c;
            i++;
        }
    }

    raw[i] = 0;
    string str = string(raw);
    long n = stol(str);
    return { n };
}

int main() {
    fstream file("input.txt");

    string line = get_line(file);
    vector<long> durations = parse_numbers_without_space(line.substr(line.find(':')));
    
    line = get_line(file);
    vector<long> records = parse_numbers_without_space(line.substr(line.find(':')));

    long score = 1;

    for (int i = 0; i < durations.size(); i++) {
        long duration = durations[i];
        long record = records[i];

        for (long d = 0; d < ceil(duration / 2); d++) {
            if (calculate_travel_distance(d, duration) > record) {
                int ways = duration + 1 - d * 2;
                score *= ways;
                break;
            }
        }
    }

    cout << "Score: " << score << endl;

    return 0;
}