#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

long calculate_arrangements(const string& pattern, vector<int> groups, unordered_map<string, long>& mem);

string unfold(const string& line, char delimiter, int count = 1) {
    string copied;
    for (int n = 0; n < count - 1; n++) {
        copied += line + delimiter;
    }
    copied += line;
    return copied;
}

vector<string> split_string(string line, char delimiter) {
    size_t pos = 0;
    vector<string> tokens;
    while ((pos = line.find(delimiter)) != string::npos) {
        tokens.push_back(line.substr(0, pos));
        line.erase(0, pos + 1);
    }
    tokens.push_back(line.substr(0, line.length()));
    return tokens;
}

vector<int> strings_to_int(const vector<string>& strings) {
    vector<int> nums;
    for (const string& s : strings) {
        nums.push_back(stoi(s));
    }
    return nums;
}

string generate_key(const string& pattern, const vector<int>& groups) {
    return pattern + '_' + to_string(groups.size());
}

bool can_be_damaged_group(const string& line) {
    for (int i = 0; i < line.length(); i++) {
        if (line[i] == '.') { return false; }
    }

    return true;
}

long handle_operational(const string& pattern, vector<int> groups, unordered_map<string, long>& mem) {
    return calculate_arrangements(pattern.substr(1), groups, mem);
}

long handle_damaged(const string& pattern, vector<int> groups, unordered_map<string, long>& mem) {
    if (groups.size() == 0) return 0;

    int group = groups.front();
    vector<int> next_groups(groups.begin() + 1, groups.end());
    string potential = pattern.substr(0, group);

    if (potential.length() < group || !can_be_damaged_group(potential)) return 0;
    if (pattern.length() == group) return calculate_arrangements("", next_groups, mem);
    if (pattern[group] == '#') return 0;
    
    return calculate_arrangements(pattern.substr(group + 1), next_groups, mem);
}

long handle_unknown(const string& pattern, vector<int> groups, unordered_map<string, long>& mem) {
    string next_pattern = pattern.substr(1);
    return calculate_arrangements('.' + next_pattern, groups, mem)
        + calculate_arrangements('#' + next_pattern, groups, mem);
}

long handle_end(const string& pattern, vector<int> groups, unordered_map<string, long>& mem) {
    return (groups.size() == 0) ? 1 : 0;
}

long handle_pattern(const string& pattern, vector<int> groups, unordered_map<string, long>& mem) {
    switch (pattern[0]) {
        case '.':
            return handle_operational(pattern, groups, mem);
        case '#':
            return handle_damaged(pattern, groups, mem);
        case '?':
            return handle_unknown(pattern, groups, mem);
        default:
            return handle_end(pattern, groups, mem);
    }
}

long calculate_arrangements(const string& pattern, vector<int> groups, unordered_map<string, long>& mem) {
    string key = generate_key(pattern, groups);

    if (mem.find(key) != mem.end()) {
        return mem.at(key);
    }

    long count = handle_pattern(pattern, groups, mem);
    mem.insert(make_pair(key, count));
    return count;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Missing file argument." << endl;
        return 1;
    }

    ifstream file(argv[1]);

    if (!file.good()) {
        cout << "Could not open file." << endl;
        return 1;
    }

    long total = 0;

    while (file.good()) {
        string line;
        getline(file, line);

        vector<string> parts = split_string(line, ' ');

        string pattern = unfold(parts[0], '?', 5);
        string number_str = unfold(parts[1], ',', 5);

        vector<string> numbers = split_string(number_str, ',');
        vector<int> groups = strings_to_int(numbers);

        unordered_map<string, long> mem;
        long possible = calculate_arrangements(pattern, groups, mem);
        total += possible;
    }

    cout << "Total: " << total << endl;

    return 0;
}