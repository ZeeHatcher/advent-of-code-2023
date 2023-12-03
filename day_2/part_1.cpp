#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

vector<string> split(string line, char delim) {
    vector<string> tokens = vector<string>();
    int idx = line.find(delim);

    while (idx != -1) {
        tokens.push_back(line.substr(0, idx));
        line.erase(line.begin(), line.begin() + idx + 1);
        idx = line.find(delim);
    }

    tokens.push_back(line.substr(0));
    return tokens;
}

string trim(string line) {
    int start = line.find_first_not_of(' ');
    if (start == string::npos) {
        return line;
    }
    int end = line.find_last_not_of(' ');
    return line.substr(start, end - start + 1);
}

class Game {
    public:
        int id = 0;
        vector<unordered_map<string, int>> rounds = vector<unordered_map<string, int>>();

        Game(string line) {
            id = parse_game_id(line);
            rounds = parse_rounds(line);
        }

        bool is_possible(unordered_map<string, int> limits) {
            for (unordered_map<string, int> &round : rounds) {
                for (auto &draw : round) {
                    if (draw.second > limits[draw.first]) {
                        return false;
                    }
                }
            }

            return true;
        }

        unordered_map<string, int> minimum_required() {
            unordered_map<string, int> required = unordered_map<string, int>();

            for (unordered_map<string, int> &round : rounds) {
                for (auto &draw : round) {
                    if (auto found = required.find(draw.first); found != required.end()) {
                        if (required[draw.first] < draw.second) {
                            required[draw.first] = draw.second;
                        }
                    } else {
                        required[draw.first] = draw.second;
                    }
                }
            }

            return required;
        }
    
    private:
        int parse_game_id(string line) {
            const int PREFIX_LENGTH = 5;
            return stoi(line.substr(PREFIX_LENGTH, line.find(':') - PREFIX_LENGTH));
        }

        vector<unordered_map<string, int>> parse_rounds(string line) {
            line = line.substr(line.find(':') + 2);
            vector<string> rnds = split(line, ';');
            vector<unordered_map<string, int>> out = vector<unordered_map<string, int>>();
            
            for (string &rnd : rnds) {
                out.push_back(parse_round(rnd));
            }

            return out;
        }

        unordered_map<string, int> parse_round(string line) {
            vector<string> draws = split(line, ',');
            unordered_map<string, int> map = unordered_map<string, int>();

            for (string &draw : draws) {
                draw = trim(draw);
                vector<string> tokens = split(draw, ' ');
                map[tokens[1]] = stoi(tokens[0]);
            }

            return map;
        }
};

string next_line_from_file(fstream &file) {
    char raw_line[256];
    file.getline(raw_line, 256);
    return string(raw_line);
}

int main() {
    fstream file("input.txt");
    unordered_map<string, int> limits = {
        { "red", 12 },
        { "green", 13 },
        { "blue", 14 },
    };
    int total_1 = 0;
    int total_2 = 0;

    while (file.good()) {
        string line = next_line_from_file(file);
        Game game(line);
        if (game.is_possible(limits)) {
            total_1 += game.id;
        }

        unordered_map<string, int> required = game.minimum_required();
        int product = 1;

        for (auto &r : required) {
            product *= r.second;
        }

        total_2 += product;
    }

    cout << "Total: " << total_1 << endl;
    cout << "Total Power: " << total_2 << endl;
}