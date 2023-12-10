#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

typedef pair<int, int> pos;
typedef pair<pos, string> mov;

string get_line(fstream &file) {
    char raw[144];
    file.getline(raw, 144);
    return string(raw);
}

vector<mov> get_starting_moves(vector<string> &grid, pos &origin) {
    vector<mov> connected;

    if (origin.second > 0) {
        char neighbour = grid[origin.second - 1][origin.first];
        if (neighbour == '|' || neighbour == '7' || neighbour == 'F') {
            pos target = { origin.first, origin.second - 1 };
            connected.push_back(make_pair(target, "north"));
        }
    }

    if (origin.second < grid.size() - 1) {
        char neighbour = grid[origin.second + 1][origin.first];
        if (neighbour == '|' || neighbour == 'L' || neighbour == 'J') {
            pos target = { origin.first, origin.second + 1 };
            connected.push_back(make_pair(target, "south"));
        }
    }

    if (origin.first > 0) {
        char neighbour = grid[origin.second][origin.first - 1];
        if (neighbour == '-' || neighbour == 'F' || neighbour == 'L') {
            pos target = { origin.first - 1, origin.second };
            connected.push_back(make_pair(target, "west"));
        }
    }

    if (origin.first < grid[0].length() - 1) {
        char neighbour = grid[origin.second][origin.first + 1];
        if (neighbour == '-' || neighbour == 'J' || neighbour == '7') {
            pos target = { origin.first + 1, origin.second };
            connected.push_back(make_pair(target, "east"));
        }
    }

    return connected;
}

int pos_id(pos &p, int l) {
    return p.first + (l * p.second);
}

mov traverse_pipe(char &pipe, mov &m) {
    pos p = m.first;
    string dir = m.second;

    switch (pipe) {
        case '|':
            if (dir == "north") {
                pos t = make_pair(p.first, p.second - 1);
                return make_pair(t, dir);
            } else if (dir == "south") {
                pos t = make_pair(p.first, p.second + 1);
                return make_pair(t, dir);
            }
            break;
        case '-':
            if (dir == "east") {
                pos t = make_pair(p.first + 1, p.second);
                return make_pair(t, dir);
            } else if (dir == "west") {
                pos t = make_pair(p.first - 1, p.second);
                return make_pair(t, dir);
            }
            break;
        case 'L':
            if (dir == "south") {
                pos t = make_pair(p.first + 1, p.second);
                return make_pair(t, "east");
            } else if (dir == "west") {
                pos t = make_pair(p.first, p.second - 1);
                return make_pair(t, "north");
            }
            break;
        case 'J':
            if (dir == "south") {
                pos t = make_pair(p.first - 1, p.second);
                return make_pair(t, "west");
            } else if (dir == "east") {
                pos t = make_pair(p.first, p.second - 1);
                return make_pair(t, "north");
            }
            break;
        case '7':
            if (dir == "north") {
                pos t = make_pair(p.first - 1, p.second);
                return make_pair(t, "west");
            } else if (dir == "east") {
                pos t = make_pair(p.first, p.second + 1);
                return make_pair(t, "south");
            }
            break;
        case 'F':
            if (dir == "north") {
                pos t = make_pair(p.first + 1, p.second);
                return make_pair(t, "east");
            } else if (dir == "west") {
                pos t = make_pair(p.first, p.second + 1);
                return make_pair(t, "south");
            }
            break;
    }
}

int get_distance_to_furthest_point(vector<string> &grid, pos &start) {
    int width = grid[0].length();
    vector<mov> connected = get_starting_moves(grid, start);
    queue<mov>* q = new queue<mov>();
    for (mov &m : connected) {
        q->push(m);
    }

    unordered_set<int> travelled;
    travelled.insert(pos_id(start, width));

    queue<mov>* buffer = new queue<mov>();
    int steps = 0;

    while (!q->empty()) {
        mov curr = q->front();
        if (travelled.find(pos_id(curr.first, width)) != travelled.end()) {
            steps++;
            break;
        }

        pos curr_pos = curr.first;
        mov next = traverse_pipe(grid[curr_pos.second][curr_pos.first], curr);
        pos next_pos = next.first;

        if (next_pos.first >= 0
            && next_pos.first < width
            && next_pos.second >= 0
            && next_pos.second < grid.size()) {

            char next_pipe = grid[next_pos.second][next_pos.first];

            if (next_pipe != '.') {
                buffer->push(next);
            }
        }

        travelled.insert(pos_id(curr_pos, width));
        q->pop();

        if (q->empty()) {
            q = buffer;
            buffer = new queue<mov>();
            steps++;
        }
    }

    return steps;
}

int main() {
    fstream file("input.txt");

    vector<string> grid;
    pos start = { string::npos, string::npos };
    int y = 0;

    while (file.good()) {
        string line = get_line(file);

        if (start.first == string::npos) {
            int start_x = line.find('S');

            if (start_x != string::npos) {
                start = { start_x, y };
            }
        }

        grid.push_back(line);
        y++;
    }

    int distance = get_distance_to_furthest_point(grid, start);

    cout << "Distance: " << distance << endl;

    return 0;
}