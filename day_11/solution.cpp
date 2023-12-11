#include <fstream>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

typedef pair<long, long> pos;

vector<pos> parse_image(ifstream& file, int expansion_factor = 1) {
    const int MAX_COL_COUNT = 144;
    vector<pos> galaxies;
    bool columns_occupied[MAX_COL_COUNT] = {};
    int y = 0;
    int y_expansion = 0;

    while (file.good()) {
        bool row_empty = true;
        string line;
        getline(file, line);

        for (int x = 0; x < line.length(); x++) {
            char c = line[x];

            if (c == '.') { continue; }

            galaxies.push_back(make_pair(x, y + y_expansion * (expansion_factor - 1)));
            columns_occupied[x] = true;
            row_empty = false;
        }

        if (row_empty) { y_expansion++; }

        y++;
    }

    for (pos &galaxy : galaxies) {
        int x_expansion = 0;
        for (int x = 0; x < galaxy.first; x++) {
            if (!columns_occupied[x]) { x_expansion++; }
        }
        galaxy.first += x_expansion * (expansion_factor - 1);
    }

    return galaxies;
}

int calculate_manhattan_distance(const pos& a, const pos& b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

int main() {
    ifstream file("input.txt");

    if (!file.good()) {
        cout << "Could not open file." << endl;
        return 1;
    }

    const vector<pos> galaxies = parse_image(file, 1000000);
    int galaxy_count = galaxies.size();
    long distance_sum = 0;

    for (int i = 0; i < galaxy_count; i++) {
        for (int j = i; j < galaxy_count; j++) {
            distance_sum += calculate_manhattan_distance(galaxies[i], galaxies[j]);
        }
    }

    cout << "Sum: " << distance_sum << endl;

    return 0;
}