#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

const unordered_map<string, char> NUMBERS = {
    { "one", '1' },
    { "two", '2' },
    { "three", '3' },
    { "four", '4' },
    { "five", '5' },
    { "six", '6' },
    { "seven", '7' },
    { "eight", '8' },
    { "nine", '9' },
    { "eno", '1' },
    { "owt", '2' },
    { "eerht", '3' },
    { "ruof", '4' },
    { "evif", '5' },
    { "xis", '6' },
    { "neves", '7' },
    { "thgie", '8' },
    { "enin", '9' },
};

class Node {
    public:
        unordered_map<char, Node*> next;

        bool is_leaf() {
            return next.empty();
        }
};

unordered_map<char, Node*> generate_tree(vector<string> words) {
    unordered_map<char, Node*> map;
    unordered_map<char, Node*>* curr = &map;

    for (string &word : words) {
        for (char &c : word) {
            if (auto found = curr->find(c); found != curr->end()) {
                curr = &(found->second->next);
            } else {
                Node* node = new Node;
                curr->insert({ c, node });
                curr = &(node->next);
            }
        }

        curr = &map;
    }

    return map;
}

char parse_number_word(string line, unordered_map<char, Node*> map) {
    vector<char> chars;
    unordered_map<char, Node*>* curr = &map;

    for (char &c : line) {
        if (auto found = curr->find(c); found != curr->end()) {
            chars.push_back(c);
            if (found->second->is_leaf()) {
                string word(chars.begin(), chars.end());
                return NUMBERS.at(word);
            }
            curr = &(found->second->next);
            continue;
        }

        return 0;
    }
}

int main(int argc, char *argv[]) {
    unordered_map<char, Node*> forward = generate_tree({ "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" });
    unordered_map<char, Node*> reverse = generate_tree({ "eno", "owt", "eerht", "ruof", "evif", "xis", "neves", "thgie", "enin" });

    int total = 0;
    ifstream file("input.txt");

    while (file.good()) {
        char raw_line[256];
        file.getline(raw_line, 256);
        string line(raw_line);

        char left, right;

        for (int i = 0; i < line.length(); i++) {
            char c = line[i];

            if (isdigit(c)) {
                left = c;
                break;
            } else {
                char v = parse_number_word(line.substr(i), forward);

                if (v == 0) {
                    continue;
                }

                left = v;
                break;
            }
        }

        string reverse_line(line.rbegin(), line.rend());
        for (int i = 0; i < reverse_line.length(); i++) {
            char c = reverse_line[i];

            if (isdigit(c)) {
                right = c;
                break;
            } else {
                char v = parse_number_word(reverse_line.substr(i), reverse);

                if (v == 0) {
                    continue;
                }

                right = v;
                break;
            }
        }

        char number[3] = { left, right, 0 };
        total += atoi(number);
    }

    cout << "Total: " << total << endl;

    file.close();

    return 0;
}