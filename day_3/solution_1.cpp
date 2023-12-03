#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>

using namespace std;

typedef pair<int, int> coord;

class Part {
    public:
        string id = "";
        coord coordinate = { 0, 0 };

        Part(string id, coord coordinate) {
            this->id = id;
            this->coordinate = coordinate;
        }

        pair<coord, coord> bounds() {
            coord start = coordinate;
            coord end = { coordinate.first + id.length() - 1, coordinate.second };
            return { start, end };
        }

        int value() {
            return stoi(id);
        }
};

string get_line(fstream &file) {
    char raw[256];
    file.getline(raw, 256);
    return string(raw);
}

void extract(string &line, int row, vector<vector<Part*>> &parts, vector<coord> &symbols) {
    vector<Part*> row_parts = {};
    int num_start = -1;

    for (int i = 0; i < line.length(); i++) {
        char c = line[i];

        if (isdigit(c)) {
            if (num_start == -1) {
                num_start = i;
            }
        } else {
            if (num_start != -1) {
                row_parts.push_back(new Part(line.substr(num_start, i - num_start), { num_start, row }));
                num_start = -1;
            }

            if (c != '.') {
                symbols.push_back({ i, row });
            }
        }
    }

    if (num_start != -1) {
        row_parts.push_back(new Part(line.substr(num_start, line.length() - num_start), { num_start, row }));
    }

    parts.push_back(row_parts);
}

bool point_in_rect(coord point, pair<coord, coord> rect) {
    coord top_left = rect.first;
    coord bottom_right = rect.second;

    return (point.first >= top_left.first
        && point.first <= bottom_right.first
        && point.second >= top_left.second 
        && point.second <= bottom_right.second);
}

bool adjacent(coord origin, pair<coord, coord> other) {
    pair<coord, coord> rect = {
        { origin.first - 1, origin.second - 1 },
        { origin.first + 1, origin.second + 1 }
    };

    return (point_in_rect(other.first, rect) || point_in_rect(other.second, rect));
}

int main() {
    fstream file("input.txt");

    vector<vector<Part*>> parts = {};
    vector<coord> symbols = {};
    unordered_set<Part*> used = {};

    int row = 0;

    while (file.good()) {
        string line = get_line(file);
        extract(line, row, parts, symbols);
        row++;
    }

    int total = 0;

    for (coord &symbol : symbols) {
        for (int y = symbol.second - 1; y <= symbol.second + 1; y++) {
            if (y < 0 || y >= parts.size()) {
                continue;
            }

            for (Part* part : parts[y]) {
                if (auto found = used.find(part); found != used.end()) {
                    continue;
                }

                if (adjacent(symbol, part->bounds())) {
                    total += part->value();
                    used.insert(part);
                }
            }
        }
    }

    cout << "Total: " << total << endl;

    return 0;
}