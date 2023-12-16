#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

long tilt_line() {

}

long tilt(vector<string>& pattern) {
    long total = 0;

    for (int x = 0; x < pattern[0].length(); x++) {
        vector<pair<int, int>> sequences;
        int round_rock_count = 0;
        int block_index = -1;

        for (int y = 0; y < pattern.size(); y++) {
            char rock = pattern[y][x];

            if (rock == '.') continue;

            if (rock == 'O') {
                round_rock_count++;
                continue;
            }

            if (rock == '#') {
                if (round_rock_count > 0) {
                    sequences.push_back(make_pair(block_index, round_rock_count));
                }
                round_rock_count = 0;
                block_index = y;
            }
        }

        if (round_rock_count > 0) {
            sequences.push_back(make_pair(block_index, round_rock_count));
        }

        long load = 0;
        for (pair<int, int>& seq : sequences) {
            long base_load = pattern.size() - seq.first - 1;
            for (int n = 0; n < seq.second; n++) {
                load += base_load;
                base_load--;
            }
        }

        total += load;
    }

    return total;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Missing file name." << endl;
        return 1;
    }

    ifstream file(argv[1]);

    if (!file.good()) {
        cout << "Could not open file." << endl;
        return 1;
    }

    vector<string> pattern;

    while (file.good()) {
        string line;
        getline(file, line);

        pattern.push_back(line);
    }

    long load = tilt(pattern);
    cout << "Load: " << load << endl;

    return 0;
}
