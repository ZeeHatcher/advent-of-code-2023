#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

vector<string> rotate_clockwise(const vector<string>& pattern) {
    vector<string> rotated;
    int width = pattern[0].length();

    for (int x = 0; x < width; x++) {
        string col = "";

        for (int y = pattern.size() - 1; y >= 0; y--) {
            col += pattern[y][x];
        }

        rotated.push_back(col);
    }

    return rotated;
}

long pattern_value(const string& pattern) {
    string binary = "";
    for (const char& c : pattern) {
        if (c == '#') {
            binary += '1';
        } else {
            binary += '0';
        }
    }
    return stol(binary, nullptr, 2);
}

vector<string> unsmudge(vector<string> pattern, int x, int y) {
    if (pattern[y][x] == '#') {
        pattern[y][x] = '.';
    } else {
        pattern[y][x] = '#';
    }
    return pattern;
}

long scan(const vector<string>& pattern, long exclude = 0) {
    long vals[pattern.size()];

    for (int i = 0; i < pattern.size(); i++) {
        vals[i] = pattern_value(pattern[i]);
    }

    for (int i = 1; i < pattern.size(); i++) {
        int left_count = i;

        if (left_count == exclude) continue;

        int right_count = pattern.size() - i;

        int count = min(left_count, right_count);

        bool mirrored = true;
        for (int n = 0; n < count; n++) {
            if (vals[i-1-n] != vals[i+n]) {
                mirrored = false;
                break;
            }
        }

        if (mirrored) {
            return left_count;
        }
    }

    return 0;
}

long find_reflection(const vector<string>& pattern) {
    long horizontal_original = scan(pattern);
    long vertical_original = scan(rotate_clockwise(pattern));

    for (int y = 0; y < pattern.size(); y++) {
        for (int x = 0; x < pattern[y].length(); x++) {
            vector<string> unsmudged = unsmudge(pattern, x, y);
            long val = scan(unsmudged, horizontal_original);
            if (val > 0) return val * 100;

            vector<string> rotated = rotate_clockwise(unsmudged);
            val = scan(rotated, vertical_original);
            if (val > 0) return val;
        }
    }

    return (horizontal_original != 0) ? horizontal_original * 100 : vertical_original;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "File not provided." << endl;
        return 1;
    }

    ifstream file(argv[1]);

    if (!file.good()) {
        cout << "Could not open file." << endl;
        return 1;
    }

    vector<string> pattern;
    long total = 0;

    while (file.good()) {
        string line;
        getline(file, line);

        if (line.length() == 0) continue;

        pattern.push_back(line);

        char next = file.peek();

        if (next != '#' && next != '.') {
            long val = find_reflection(pattern);
            total += val;

            pattern.clear();
        }
    }

    cout << "Total: " << total << endl;

    return 0;
}