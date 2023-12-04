#include <fstream>
#include <string>
#include <unordered_set>
#include <iostream>

using namespace std;

string get_line(fstream &file) {
    char raw_line[128];
    file.getline(raw_line, 128);
    return string(raw_line);
}

int main() {
    fstream file("input.txt");
    int total = 0;

    while (file.good()) {
        string line = get_line(file);
        bool past = false;
        unordered_set<string> winners = {};
        char buffer[3] = {0};
        int buffer_index = 0;
        int point = 0;

        for (int i = line.find(':') + 2; i < line.length(); i++) {
            char c = line[i];

            if (c == '|') {
                past = true;
                continue;
            }

            if (past) {
                if (isdigit(c)) {
                    buffer[buffer_index] = c;
                    buffer_index += 1;
                } else if (isdigit(buffer[0])) {
                    if (auto found = winners.find(string(buffer)); found != winners.end()) {
                        if (point == 0) {
                            point = 1;
                        } else {
                            point *= 2;
                        }
                    }

                    buffer[0] = 0;
                    buffer[1] = 0;
                    buffer[2] = 0;
                    buffer_index = 0;
                }
            } else {
                if (isdigit(c)) {
                    buffer[buffer_index] = c;
                    buffer_index += 1;
                } else if (isdigit(buffer[0])) {
                    winners.insert(string(buffer));

                    buffer[0] = 0;
                    buffer[1] = 0;
                    buffer[2] = 0;
                    buffer_index = 0;
                }
            }
        }

        if (isdigit(buffer[0])) {
            if (auto found = winners.find(string(buffer)); found != winners.end()) {
                if (point == 0) {
                    point = 1;
                } else {
                    point *= 2;
                }
            }

            buffer[0] = 0;
            buffer[1] = 0;
            buffer[2] = 0;
            buffer_index = 0;
        }

        total += point;
    }

    cout << "Total: " << total << endl;

    return 0;
}