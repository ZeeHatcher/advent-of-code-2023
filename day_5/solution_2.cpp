#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <queue>

using namespace std;

typedef pair<long, long> seed_range;

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

        pair<seed_range, vector<seed_range>> map_range(seed_range original) {
            long source_end = source + range;

            if (
                (original.first < source && original.second <= source)
                || (original.first >= source_end && original.second > source_end)
            ) {
                return { { -1, -1 }, { original } };
            }

            vector<seed_range> unmapped = {};
            long map_start, map_end = 0;

            if (original.first < source && original.second > source) {
                unmapped.push_back({ original.first, source });
                map_start = source;
            } else {
                map_start = original.first;
            }

            if (original.first < source_end && original.second > source_end) {
                unmapped.push_back({ source_end, original.second });
                map_end = source_end;
            } else {
                map_end = original.second;
            }

            seed_range mapped = { map(map_start), map(map_end) };

            return { mapped, unmapped };
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

long process_seeds(seed_range seeds, vector<vector<Mapper*>> steps) {
    queue<seed_range> unmapped, mapped;
    unmapped.push(seeds);

    for (vector<Mapper*> &step : steps) {
        for (Mapper* &mapper : step) {
            queue<seed_range> buffer;

            while (!unmapped.empty()) {
                pair<seed_range, vector<seed_range>> res = mapper->map_range(unmapped.front());
                unmapped.pop();

                if (res.first.first != -1 && res.first.second != -1) {
                    mapped.push(res.first);
                }

                if (!res.second.empty()) {
                    for (seed_range &u : res.second) {
                        buffer.push(u);
                    }
                }
            }

            unmapped = buffer;
        }

        while (!mapped.empty()) {
            unmapped.push(mapped.front());
            mapped.pop();
        }
    }

    long min = numeric_limits<long>::max();

    while (!unmapped.empty()) {
        long local_min = unmapped.front().first;
        if (local_min < min) {
            min = local_min;
        }
        unmapped.pop();
    }

    return min;
}

int main() {
    fstream file("input.txt");

    string seed_line = get_line(file);
    vector<long> numbers = extract_numbers(seed_line.substr(seed_line.find(':') + 2));
    skip_line(file, 2);
    vector<vector<Mapper*>> steps = setup_mappers(file);

    long min = numeric_limits<long>::max();

    for (int i = 0; i < numbers.size(); i += 2) {
        seed_range seeds = { numbers[i], numbers[i] + numbers[i+1] };
        long local_min = process_seeds(seeds, steps);

        if (local_min < min) {
            min = local_min;
        }
    }

    cout << "MIN: " << min << endl;

    return 0;
}