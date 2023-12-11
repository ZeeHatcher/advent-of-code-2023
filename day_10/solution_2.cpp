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

bool dfs(vector<string> &grid, mov &m, pos &target, unordered_set<int> &path) {
    int width = grid[0].length();
    pos curr_pos = m.first;

    if (curr_pos == target) {
        return true;
    }

    mov next = traverse_pipe(grid[curr_pos.second][curr_pos.first], m);
    pos next_pos = next.first;

    if (next_pos.first >= 0
        && next_pos.first < width
        && next_pos.second >= 0
        && next_pos.second < grid.size()) {

        char next_pipe = grid[next_pos.second][next_pos.first];

        if (next_pipe != '.') {
            bool reached = dfs(grid, next, target, path);

            if (reached) {
                path.insert(pos_id(curr_pos, width));
            }

            return reached;
        }
    }

    return false;
}

unordered_set<int> generate_wall(vector<string> &grid, pos &start) {
    int width = grid[0].length();
    vector<mov> connected = get_starting_moves(grid, start);

    for (mov &conn : connected) {
        unordered_set<int> wall = { pos_id(start, width) };
        bool reached = dfs(grid, conn, start, wall);

        if (reached) {
            return wall;
        }
    }

    return {};
}

bool is_last_wall(vector<string> &grid, unordered_set<int> &wall, pos &start) {
    int width = grid[0].length();

    for (int x = start.first; x < width; x++) {
        pos curr = { x, start.second };
        if (wall.find(pos_id(curr, width)) != wall.end()) { return false; }
    }

    return true;
}

int calculate_enclosed_area(vector<string> &grid, unordered_set<int> &wall) {
    int width = grid[0].length();
    int count = 0;

    for (int y = 0; y < grid.size(); y++) {
        bool entered = false;

        for (int x = 0; x < width; x++) {
            pos curr = { x, y };
            int id = pos_id(curr, width);

            if (wall.find(id) != wall.end()) {
                char pipe = grid[y][x];
                if (pipe == '-') { continue; }

                if (pipe == 'F' || pipe == 'L') {
                    entered = true;
                } else if (pipe == '7' || pipe == 'J') {
                    entered = false;
                } else if (pipe == '|') {
                    pos check = { curr.first + 1, curr.second };
                    if (is_last_wall(grid, wall, check)) {
                        entered = false;
                    } else {
                        entered = !entered;
                    }
                }

            } else if (entered && grid[y][x] == '.') {
                count++;
            }
        }
    }

    return count;
}

int main() {
    fstream file("sample_4.txt");

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

    unordered_set<int> wall = generate_wall(grid, start);
    int area = calculate_enclosed_area(grid, wall);

    cout << "Size: " << wall.size() << endl;
    cout << "Furthest Point: " << wall.size() / 2 << endl;
    cout << "Area: " << area << endl;

    return 0;
}