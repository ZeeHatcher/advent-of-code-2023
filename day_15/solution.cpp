#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Box {
    public:
        void insert(string label, int focal) {
            if (lenses.find(label) == lenses.end()) {
                lens_order.push_back(label);
            }
            lenses[label] = focal;
        }

        void remove(string label) {
            if (lenses.find(label) == lenses.end()) {
                return;
            }

            lenses.erase(label);
            for (int i = 0; i < lens_order.size(); i++) {
                if (lens_order[i] == label) {
                    lens_order.erase(lens_order.begin() + i);
                    break;
                }
            }
        }

        long focus_power() {
            long power = 0;
            for (int i = 0; i < lens_order.size(); i++) {
                string& lens = lens_order[i];
                power += (i + 1) * lenses.at(lens);
            }
            return power;
        }
    private:
        vector<string> lens_order;
        unordered_map<string, int> lenses;
};

vector<string> split(string line, char delim) {
    size_t i;
    vector<string> tokens;
    while ((i = line.find(delim)) != string::npos) {
        tokens.push_back(line.substr(0, i));
        line.erase(0, i + 1);
    }
    tokens.push_back(line);
    return tokens;
}

int custom_hash(string& str) {
    long val = 0;
    for (char& c : str) {
        val += c;
        val *= 17;
        val %= 256;
    }
    return val;
}

string extract_label(string& str) {
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];

        if (c == '=' || c == '-') return str.substr(0, i);
    }
    return str;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Not enough arguments." << endl;
        return 1;
    }

    ifstream file(argv[1]);

    if (!file.good()) {
        cout << "Could not open file." << endl;
        return 1;
    }

    string line;
    getline(file, line);

    vector<string> steps = split(line, ',');
    Box boxes[256];

    for (string& step : steps) {
        string label;
        char action;
        int action_index;

        for (int i = 0; i < step.length(); i++) {
            char c = step[i];

            if (c == '=' || c == '-') {
                label = step.substr(0, i);
                action = c;
                action_index = i;
                break;
            }
        }

        int box_index = custom_hash(label);
        Box& box = boxes[box_index];

        if (action == '=') {
            int focal = stoi(step.substr(action_index + 1));
            box.insert(label, focal);
        } else if (action == '-') {
            box.remove(label);
        }
    }

    long total = 0;

    for (int i = 0; i < 256; i++) {
        Box& box = boxes[i];
        total += (i + 1) * box.focus_power();
    }

    cout << "Total: " << total << endl;

    return 0;
}