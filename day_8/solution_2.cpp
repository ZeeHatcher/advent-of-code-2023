#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <queue>

using namespace std;

typedef pair<string, string> choice;

string get_line(fstream &file) {
    char raw[288];
    file.getline(raw, 288);
    return string(raw);
}

long gcd(long a, long b) {
    if (a == 0 || b == 0) {
        return b == 0 ? a : b;
    }

    while (a != 0 && b != 0) {
        if (a >= b) {
            a = a % b;
        } else if (b >= a) {
            b = b % a;
        }
    }

    return (b == 0) ? a : b;
}

long lcm(long a, long b) {
    return a * b / gcd(a, b);
}

long lcm_multiple(vector<long> nums) {
    if (nums.size() == 0) { return 0; }
    if (nums.size() == 1) { return nums[0]; }

    long val = lcm(nums[0], nums[1]);

    for (int i = 2; i < nums.size(); i++) {
        val = lcm(val, nums[i]);
    }

    return val;
}

int main() {
    fstream file("input.txt");
    unordered_map<string, choice> map;

    string instructions = get_line(file);
    vector<string> nodes = {};
    get_line(file);

    while (file.good()) {
        string line = get_line(file);

        string node = line.substr(0, 3);
        string left = line.substr(line.find('(') + 1, 3);
        string right = line.substr(line.find(',') + 2, 3);

        choice ch = { left, right };
        map.insert(make_pair(node, ch));

        if (node[2] == 'A') {
            nodes.push_back(node);
        }
    }

    vector<long> cycles;

    for (int n = 0; n < nodes.size(); n++) {
        string node = nodes[n];
        long steps = 0;
        int idx = 0;

        while (node[2] != 'Z') {
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

            nodes[n] = node;
        }

        cycles.push_back(steps);
    }

    long steps_required = lcm_multiple(cycles);

    cout << "Steps Required: " << steps_required << endl;

    return 0;
}