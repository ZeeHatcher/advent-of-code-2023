#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

typedef pair<string, string> choice;

string get_line(fstream &file) {
    char raw[288];
    file.getline(raw, 288);
    return string(raw);
}

int main() {
    fstream file("input.txt");
    unordered_map<string, choice> map;

    string instructions = get_line(file);
    get_line(file);

    while (file.good()) {
        string line = get_line(file);

        string node = line.substr(0, 3);
        string left = line.substr(line.find('(') + 1, 3);
        string right = line.substr(line.find(',') + 2, 3);

        choice ch = { left, right };
        map.insert(make_pair(node, ch));
    }

    int steps = 0;
    int idx = 0;
    string node = "AAA";
    while (node != "ZZZ") {
        choice curr = map.at(node);

        switch (instructions[idx]) {
            case 'L':
                node = curr.first;
                break;
            case 'R':
                node = curr.second;
                break;
        }

        idx++;
        if (idx >= instructions.length()) {
            idx = 0;
        }
        steps++;
    }

    cout << "Steps taken: " << steps << endl;

    return 0;
}