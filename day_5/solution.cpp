#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

class Mapper {
    public:
        long source = 0;
        long destination = 0;
        long range = 0;

        Mapper(long source, long destination, long range) {
            this->source = source;
            this->destination = destination;
            this->range = range;
        }

        bool in_range(long val) {
            return val >= source && val < source + range;
        }

        long map(long val) {
            return val - source + destination;
        }
};

string get_line(fstream &file) {
    char raw_line[256];
    file.getline(raw_line, 256);
    return string(raw_line);
}

void skip_line(fstream &file, int n = 1) {
    char raw_line[256];
    for (int i = 0; i < n; i++) {
        file.getline(raw_line, 256);
    }
}

vector<long> extract_numbers(string line) {
    vector<long> numbers = {};
    int start = -1;
    for (int i = 0; i < line.length(); i++) {
        char c = line[i];

        if (isdigit(c)) {
            if (start == -1) {
                start = i;
            }
        } else if (start != -1) {
            numbers.push_back(stol(line.substr(start, i - start)));
            start = -1;
        }
    }

    if (start != -1) {
        numbers.push_back(stol(line.substr(start, line.length() - start)));
    }

    return numbers;
}

vector<vector<Mapper*>> setup_mappers(fstream &file) {
    vector<vector<Mapper*>> steps = {};
    vector<Mapper*> step = {};

    while (file.good()) {
        string line = get_line(file);

        if (line.length() == 0) {
            steps.push_back(step);
            step = {};
            continue;
        }

        if (!isdigit(line[0])) {
            continue;
        }

        vector<long> numbers = extract_numbers(line);
        Mapper* mapper = new Mapper(numbers[1], numbers[0], numbers[2]);
        step.push_back(mapper);
    }

    steps.push_back(step);

    return steps;
}

long process_seed(long seed, vector<vector<Mapper*>> steps) {
    long val = seed;

    for (vector<Mapper*> &step : steps) {
        for (Mapper* &mapper : step) {
            if (mapper->in_range(val)) {
                val = mapper->map(val);
                break;
            }
        }
    }

    return val;
}

int main() {
    fstream file("input.txt");

    string seed_line = get_line(file);
    vector<long> seeds = extract_numbers(seed_line.substr(seed_line.find(':') + 2));
    skip_line(file, 2);
    vector<vector<Mapper*>> steps = setup_mappers(file);

    long min = numeric_limits<long>::max();

    for (long &seed : seeds) {
        long location = process_seed(seed, steps);
        if (location < min) {
            min = location;
        }
    }

    cout << "MIN: " << min << endl;

    return 0;
}