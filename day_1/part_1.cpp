#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    int total = 0;
    ifstream file(argv[1]);

    while (file.good()) {
        char left, right;
        char line[256];

        file.getline(line, 256);
        int n = file.gcount();

        for (int i = 0; i < n; i++) {
            if (isdigit(line[i])) {
                left = line[i];
                break;
            }
        }

        for (int i = n - 1; i >= 0; i--) {
            if (isdigit(line[i])) {
                right = line[i];
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