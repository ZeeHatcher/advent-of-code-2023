#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>

using namespace std;

typedef pair<int, int> coord;

struct Move {
    coord pos;
    string direction;

    Move() {}
    
    Move(coord pos, string direction) {
        this->pos = pos;
        this->direction = direction;
    }

    bool operator==(const Move& other) const {
        return (pos.first == other.pos.first
            && pos.second == other.pos.second
            && direction == other.direction);
    }

    struct HashFunction {
        size_t operator()(const Move& mov) const {
            size_t a = hash<int>()(mov.pos.first);
            size_t b = hash<int>()(mov.pos.second) << 1;
            size_t c = hash<string>()(mov.direction) << 2;
            return a ^ b ^ c;
        }
    };
};

vector<Move> handle_dot(Move& mov) {
    if (mov.direction == "up") return { Move(make_pair(mov.pos.first, mov.pos.second - 1), "up") };
    if (mov.direction == "down") return { Move(make_pair(mov.pos.first, mov.pos.second + 1), "down") };
    if (mov.direction == "left") return { Move(make_pair(mov.pos.first - 1, mov.pos.second), "left") };
    if (mov.direction == "right") return { Move(make_pair(mov.pos.first + 1, mov.pos.second), "right") };
    return {};
}

vector<Move> handle_back_slash(Move& mov) {
    if (mov.direction == "up") return { Move(make_pair(mov.pos.first + 1, mov.pos.second), "right") };
    if (mov.direction == "down") return { Move(make_pair(mov.pos.first - 1, mov.pos.second), "left") };
    if (mov.direction == "left") return { Move(make_pair(mov.pos.first, mov.pos.second + 1), "down") };
    if (mov.direction == "right") return { Move(make_pair(mov.pos.first, mov.pos.second - 1), "up") };
    return {};
}

vector<Move> handle_forward_slash(Move& mov) {
    if (mov.direction == "up") return { Move(make_pair(mov.pos.first - 1, mov.pos.second), "left") };
    if (mov.direction == "down") return { Move(make_pair(mov.pos.first + 1, mov.pos.second), "right") };
    if (mov.direction == "left") return { Move(make_pair(mov.pos.first, mov.pos.second - 1), "up") };
    if (mov.direction == "right") return { Move(make_pair(mov.pos.first, mov.pos.second + 1), "down") };
    return {};
}

vector<Move> handle_pipe(Move& mov) {
    if (mov.direction == "left" || mov.direction == "right") return {
        Move(make_pair(mov.pos.first, mov.pos.second - 1), "up"),
        Move(make_pair(mov.pos.first, mov.pos.second + 1), "down") };
    return handle_dot(mov);
}

vector<Move> handle_bar(Move& mov) {
    if (mov.direction == "up" || mov.direction == "down") return {
        Move(make_pair(mov.pos.first - 1, mov.pos.second), "left"),
        Move(make_pair(mov.pos.first + 1, mov.pos.second), "right") };
    return handle_dot(mov);
}

long get_energized_count(vector<string>& map, Move start) {
    const int width = map[0].length();
    const int height = map.size();

    unordered_set<Move, Move::HashFunction> traversed;
    unordered_set<int> energized;
    queue<Move> q;
    q.push(start);

    while (!q.empty()) {
        Move curr = q.front();
        coord pos = curr.pos;
        vector<Move> next;

        switch (map[pos.second][pos.first]) {
            case '/':
                next = handle_back_slash(curr);
                break;
            case '\\':
                next = handle_forward_slash(curr);
                break;
            case '|':
                next = handle_pipe(curr);
                break;
            case '-':
                next = handle_bar(curr);
                break;
            default:
                next = handle_dot(curr);
        }

        for (Move mov : next) {
            if (mov.pos.first < 0 || mov.pos.first >= width || mov.pos.second < 0 || mov.pos.second >= height) continue;
            if (traversed.find(mov) != traversed.end()) continue;
            q.push(mov);
        }

        traversed.insert(curr);
        energized.insert(pos.first + (pos.second * width));
        q.pop();
    }

    return energized.size();
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

    vector<string> map;
    while (file.good()) {
        string line;
        getline(file, line);
        map.push_back(line);
    }

    const int width = map[0].length();
    const int height = map.size();
    long max = 0;

    for (int x = 0; x < width; x++) {
        Move start(make_pair(x, 0), "down");
        long count = get_energized_count(map, start);
        if (count > max) max = count;

        start = Move(make_pair(x, height - 1), "up");
        count = get_energized_count(map, start);
        if (count > max) max = count;
    }

    for (int y = 0; y < height; y++) {
        Move start(make_pair(0, y), "right");
        long count = get_energized_count(map, start);
        if (count > max) max = count;

        start = Move(make_pair(width - 1, y), "left");
        count = get_energized_count(map, start);
        if (count > max) max = count;
    }

    cout << "Count: " << max << endl;

    return 0;
}